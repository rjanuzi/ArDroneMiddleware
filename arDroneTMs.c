#include <arDroneTMs.h>

static arDroneTms_threads threads;

void* arDroneTms_tmReceiverThread(void *arg)
{
	static struct sockaddr_in si_me, si_other;
	static int s, i, slen=sizeof(si_other);
	static char buf[UTIL_UDP_BUFFLEN];

	printf("arDroneTMs.arDroneTms_tmReceiverThread: LOG - Sending SOME BYTES to Drone.\n");
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
			printf("arDroneTMs.arDroneTms_tmReceiverThread: ERROR - Can't open the socket.\n");
			printf("Trying again in 10 seconds...\n");
			sleep(10);
		}

		memset((char *) &si_me, 0, sizeof(si_me));
		si_me.sin_family = AF_INET;
		si_me.sin_port = htons(DRONE_NAVDATA_PORT);
		si_me.sin_addr.s_addr = htonl(INADDR_ANY);

		if( bind(s, (__CONST_SOCKADDR_ARG) &si_me, (socklen_t) sizeof(si_me)) == -1)
		{
			printf("arDroneTMs.arDroneTms_tmReceiverThread: ERROR - Can't bind the socket.\n");
			printf("Trying again in 10 seconds...\n");
			sleep(10);
		}

		if( recvfrom(s, (void *__restrict) buf, (size_t) UTIL_UDP_BUFFLEN, 0, (__SOCKADDR_ARG) &si_other, (socklen_t *__restrict) &slen) == -1)
		{
			printf("arDroneTMs.arDroneTms_tmReceiverThread: ERROR - Fail to receive packet.\n");
		}

		printf("Received packet from %s:%d\nData: %s\n\n", inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port), buf);

		close(s);
	}

	return NULL;
}

bool arDroneTms_init()
{
	int err;

	err = pthread_create(&threads.tmReceiverThread, NULL, &arDroneTms_tmReceiverThread, NULL);

	if(err != 0)
	{
		printf("\narDroneTms.arDroneTms_init - ERROR - Can't create arDroneTms_tmReceiverThread: [%s]", strerror(err));
		return false;
	}
	else
	{
		printf("\narDroneTms.arDroneTms_init - LOG - arDroneTms_tmReceiverThread created sucessfully\n");
		return true;
	}
}

arDroneTms_threads arDroneTms_getThreadsIds()
{
	return threads;
}
