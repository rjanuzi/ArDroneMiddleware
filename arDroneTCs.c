#include <arDroneTCs.h>

static arDroneTcs_threads threads;

void* arDroneTcs_tcRespReceiverThread(void *arg)
{
	static struct sockaddr_in si_me, si_other;
	static int s, i, slen=sizeof(si_other);
	static char buf[UTIL_UDP_BUFFLEN];

	while( true )
	{
		printf("\nReceive Loop (TCs) \n\n\n");

		while( (s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1 )
		{
			printf("arDroneTcs_tcRespReceiverThread: ERROR - Can't open the socket.\n");
			printf("Trying again in 10 seconds...\n");
			sleep(10);
		}

		memset((char *) &si_me, 0, sizeof(si_me));
		si_me.sin_family = AF_INET;
		si_me.sin_port = htons(DRONE_ATCMD_PORT);
		si_me.sin_addr.s_addr = htonl(INADDR_ANY);

		if( bind(s, (__CONST_SOCKADDR_ARG) &si_me, (socklen_t) sizeof(si_me)) == -1)
		{
			printf("arDroneTcs_tcRespReceiverThread: ERROR - Can't bind the socket.\n");
			printf("Trying again in 10 seconds...\n");
			sleep(10);
		}

		if( recvfrom(s, (void *__restrict) buf, (size_t) UTIL_UDP_BUFFLEN, 0, (__SOCKADDR_ARG) &si_other, (socklen_t *__restrict) &slen) == -1)
		{
			printf("arDroneTcs_tcRespReceiverThread: ERROR - Fail to receive packet.\n");
		}

		printf("Received packet from %s:%d\nData: %s\n\n", inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port), buf);

		close(s);
	}

	return NULL;
}

bool arDroneTcs_init()
{
	int err;

	err = pthread_create(&threads.tcRespReceiverThread, NULL, &arDroneTcs_tcRespReceiverThread, NULL);

	if(err != 0)
	{
		printf("\narDroneTcs.arDroneTcs_init - ERROR - Can't create arDroneTcs_tcRespReceiverThread: [%s]", strerror(err));
		return false;
	}
	else
	{
		printf("\narDroneTcs.arDroneTcs_init - LOG - arDroneTcs_tcRespReceiverThread created sucessfully\n");
		return true;
	}
}

arDroneTcs_threads arDroneTcs_getThreadsIds()
{
	return threads;
}

bool arDroneTcs_sendAtCmd(const char* cmd)
{
	printf("arDroneTCs.arDroneTcs_sendAtCmd: LOG - Sending at cmd (%s) to Drone.\n", cmd);
	if(!utilUdp_sendUdpMsg(cmd, DRONE_IP, DRONE_ATCMD_PORT, DRONE_ATCMD_PORT))
		return false;

	return true;
}