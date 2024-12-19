#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>

#include <linux/can.h>
#include <linux/can/raw.h>

#include <signal.h>

#include <zmq.h>
#include <assert.h>

static volatile sig_atomic_t keep_running = 1;

static void sig_handler(int _)
{
    (void)_;
    keep_running = 0;
}

int main(int argc, char **argv)
{
	signal(SIGINT, sig_handler);
	signal(SIGTERM, sig_handler);
	int s, i;
	int nbytes;
	struct sockaddr_can addr;
	struct ifreq ifr;
	struct can_frame frame;

	printf("CAN Sockets Receive Demo\r\n");

	if ((s = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0) {
		perror("Socket");
		return 1;
	}

	strcpy(ifr.ifr_name, "can0" );
	ioctl(s, SIOCGIFINDEX, &ifr);

	memset(&addr, 0, sizeof(addr));
	addr.can_family = AF_CAN;
	addr.can_ifindex = ifr.ifr_ifindex;

	if (bind(s, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
		perror("Bind");
		return 1;
	}

	// Connect to zmq
	void *context = zmq_ctx_new ();
  void *publisher = zmq_socket (context, ZMQ_PUB);
  int rc = zmq_bind(publisher, "ipc:///tmp/speed.ipc");
	assert (rc == 0);

	while(keep_running) {
		nbytes = read(s, &frame, sizeof(struct can_frame));

 		if (nbytes < 0) {
			perror("Read");
			continue;
		}

		rc = zmq_send(publisher, frame.data, nbytes, 0);
		if(rc != nbytes) {
			perror("Error sending");
		}
	}

	// Closing ZeroMQ
	zmq_close(publisher);
	zmq_ctx_destroy(context);

  // Closing CAN Socket
	if (close(s) < 0) {
		perror("Close");
		return 1;
	}

	return 0;
}
