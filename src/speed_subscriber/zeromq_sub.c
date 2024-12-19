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
    int rc = zmq_connect(subscriber, "ipc:///tmp/speed.ipc");
    //int rc = zmq_connect(subscriber, "tcp://localhost:5556");
    assert(rc == 0);
    rc = zmq_setsockopt(subscriber, ZMQ_SUBSCRIBE, "", 0);
    assert(rc == 0);
    printf("Waiting for messages\n");
    unsigned char message[8];

    while(keep_running)
    {
        rc = zmq_recv(subscriber, message, 8, 0);
        if (rc == -1) continue;

        unsigned char speed = message[0];

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
