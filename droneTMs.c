#include "droneTMs.h"

static droneTms_threads threads;

void* droneTms_tmReceiverThread(void *arg)
{
	static struct sockaddr_in si_me, si_other;
	static int s, i, slen=sizeof(si_other);
	static char buf[UTIL_UDP_BUFFLEN];

	printf("[LOG]: droneTms.droneTms_tmReceiverThread - Sending SOME BYTES to Drone.\n");
	while(!utilUdp_sendUdpMsg("SOME BYTES", DRONE_IP, DRONE_NAVDATA_PORT, DRONE_NAVDATA_PORT))
	{
		printf("Trying again in 10 seconds...\n");
		sleep(10);
	}

	while( true )
	{
		printf("\nReceive Loop\n\n\n");

		while( (s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1 )
		{
			printf("\n[ERROR]: droneTms.droneTms_tmReceiverThread - Can't open the socket.\n");
			printf("Trying again in 10 seconds...\n");
			sleep(10);
		}

		memset((char *) &si_me, 0, sizeof(si_me));
		si_me.sin_family = AF_INET;
		si_me.sin_port = htons(DRONE_NAVDATA_PORT);
		si_me.sin_addr.s_addr = htonl(INADDR_ANY);

		if( bind(s, (__CONST_SOCKADDR_ARG) &si_me, (socklen_t) sizeof(si_me)) == -1)
		{
			printf("[ERROR]: droneTms.droneTms_tmReceiverThread - Can't bind the socket.\n");
			printf("Trying again in 10 seconds...\n");
			sleep(10);
		}

		if( recvfrom(s, (void *__restrict) buf, (size_t) UTIL_UDP_BUFFLEN, 0, (__SOCKADDR_ARG) &si_other, (socklen_t *__restrict) &slen) == -1)
		{
			printf("[ERROR]: droneTms.droneTms_tmReceiverThread - Fail to receive packet.\n");
		}

		printf("Received packet from %s:%d\nData: %s\n\n", inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port), buf);

		close(s);
	}

	return NULL;
}

bool droneTms_init()
{
	int err;

	err = pthread_create(&threads.tmReceiverThread, NULL, &droneTms_tmReceiverThread, NULL);

	if(err != 0)
	{
		printf("\n[ERROR]: droneTms.droneTms_init - Can't create droneTms_tmReceiverThread: [%s]", strerror(err));
		return false;
	}
	else
	{
		printf("\n[LOG]: droneTms.droneTms_init - droneTms_tmReceiverThread created sucessfully\n");
		return true;
	}
}

droneTms_threads droneTms_getThreadsIds()
{
	return threads;
}
