#include <util_udp.h>

bool utilUdp_sendUdpMsg(const char* msg, const char* hostIp, int destPort, int srcPort)
{
	struct sockaddr_in si_other, si_me;
	int s, i, slen=sizeof(si_other);
	char buf[UTIL_UDP_BUFFLEN];

	if ( (s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))==-1)
	{
		printf("\n[ERROR]: util_udp.utilUdp_sendUdpMsg - Fail to open socket.");
		goto err;
	}

	memset( (char *) &si_other, 0, sizeof(si_other));
	si_other.sin_family = AF_INET;
	si_other.sin_port = htons(destPort);

	memset( (char *) &si_me, 0, sizeof(si_me));
	si_me.sin_family = AF_INET;
	si_me.sin_port = htons(srcPort);


	if (inet_aton(hostIp, &si_other.sin_addr)==0)
	{
		printf("\n[ERROR]: util_udp.utilUdp_sendUdpMsg - inet_aton() failed.");
		goto err;
	}

	if( bind(s, (__CONST_SOCKADDR_ARG) &si_me, (socklen_t) sizeof(si_me)) == -1)
	{
		printf("\n[ERROR]: utilUdp.utilUdp_sendUdpMsg - Can't bind the socket.\n");
		printf("Trying again in 10 seconds...\n");
		sleep(10);
	}

	sprintf(buf, "%s", msg);
	if( sendto(s, (const void*) &msg[0], (size_t) strlen(msg), 0, (__CONST_SOCKADDR_ARG)&si_other, (socklen_t)slen) == -1)
	{
		printf("\n[ERROR]: util_udp.utilUdp_sendUdpMsg - Sending fail.");
		goto err;
	}

	close(s);

	return true;

	err:
	close(s);
	return false;

}
