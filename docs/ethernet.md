# Connection via Ethernet

As we will delegate all the ML processing to the Jetson Nano, while keeping the control of the car and the Instrument Cluster on the Raspberry, we needed a way to efficiently communicate between them. As CAN has a maximum rate of 500kbs, we chose to try to connect via Ethernet. This document describes the steps performed to achieve it.

## Setting up static IPs

The static IPs for the devices are:

* Raspberry Pi: 192.168.1.10/24
* Jetson Nano: 192.168.20/24

And the gateway IP is 192.168.1.1

Both devices have the Network Manager installed, which eased the process. To configure the static IPv4, the following commands were used (raspberry pi example):

```
sudo nmcli c mod "Wired connection 1" ipv4.addresses 192.168.1.10/24 ipv4.method manual
sudo nmcli con mod "Wired connection 1" ipv4.gateway 192.168.1.1
sudo nmcli c down "Wired connection 1" && sudo nmcli c up "Wired connection 1"
```

After running these commands, pinging those IPs from either machine start working.

## Installing ZeroMQ on Jetson Nano

```
echo 'deb http://download.opensuse.org/repositories/network:/messaging:/zeromq:/release-stable/xUbuntu_18.04/ /' | sudo tee /etc/apt/sources.list.d/network:messaging:zeromq:release-stable.list
curl -fsSL https://download.opensuse.org/repositories/network:messaging:zeromq:release-stable/xUbuntu_18.04/Release.key | gpg --dearmor | sudo tee /etc/apt/trusted.gpg.d/network_messaging_zeromq_release-stable.gpg > /dev/null
sudo apt update
sudo apt install libzmq3-dev
```

## ZeroMQ Test

Small POC programs were used just to test the functionality.

### Publisher (Jetson Nano)

```
#include <zmq.h>
#include <assert.h>

int main(int argc, char **argv)
{

        // Connect to zmq
        void *context = zmq_ctx_new ();
        void *publisher = zmq_socket (context, ZMQ_PUB);
        int rc = zmq_bind(publisher, "tcp://*:5556");
        assert (rc == 0);
        // Give some time for the subscribers to connect
        sleep(1);
        unsigned char message[4] = {1,0,0xE8,0x00};
        rc = zmq_send(publisher, message, 4, 0);
        assert(rc == 4);

        zmq_close(publisher);
        zmq_ctx_destroy(context);

        return 0;
}
```

### Subscriber (Raspberry Pi)

```
#include <stdio.h>
#include <assert.h>
#include <signal.h>
#include <zmq.h>

static volatile sig_atomic_t keep_running = 1;

static void sig_handler(int _)
{
    (void)_;
    keep_running = 0;
}

int main()
{
    signal(SIGINT, sig_handler);
    signal(SIGTERM, sig_handler);

    void *context = zmq_ctx_new();
    void *subscriber = zmq_socket(context, ZMQ_SUB);
    int rc = zmq_connect(subscriber, "tcp://192.168.1.20:5556");
    assert(rc == 0);
    rc = zmq_setsockopt(subscriber, ZMQ_SUBSCRIBE, "", 0);
    assert(rc == 0);
    printf("Waiting for messages\n");
    unsigned char message[8];

    while(keep_running)
    {
        rc = zmq_recv(subscriber, message, 8, 0);
        if (rc == -1) continue;

        // Extract speed
        unsigned char speed = message[0];

        // Extract unit
        //unsigned char unit = message[1];

        // Extract RPM (combine the two bytes)
        unsigned short rpm = (message[3] << 8) | message[2];

        // Print the extracted values
        printf("\rSpeed: %03d km/h -- RPM:%04d", speed, rpm);
        fflush(stdout);
    }

    zmq_close(subscriber);
    zmq_ctx_destroy(context);

    return 0;
}
```

### Compilation

To compile the programs run

```
gcc <filename> -o <output_name> -lzmq
```