/**
 * \file
 *
 * Este arquivo, juntamente com o seu .h relacionado, possui funcionalidades
 * utilitarias para se trabalhar com os arquivos de imagens disponiveis
 *
 * @author Rafael B. Januzi (rjanuzi@gmail.com)
 * @date 06/11/2016
 *
 */

#include <util_udp.h>

bool utilUdp_sendUdpMsg(const uint8_t* msg, uint16_t len, const char* hostIp, int destPort, int srcPort)
{
	struct sockaddr_in si_other, si_me;
	int s, i, slen=sizeof(si_other);
	uint8_t buf[UTIL_UDP_BUFFLEN];

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
	if( sendto(s, (const void*) &msg[0], (size_t) len, 0, (__CONST_SOCKADDR_ARG)&si_other, (socklen_t)slen) == -1)
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

bool utilUdp_receiveUdpMsg(uint8_t* buf, int destPort)
{
	struct sockaddr_in si_other, si_me;
	int s, slen=sizeof(si_other);

	while( (s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1 )
	{
		printf("\n[ERROR]: utilUdp_receiveUdpMsg - Can't open the socket.\n");
		printf("Trying again in 10 seconds...\n");
		sleep(10);
	}

	memset((char *) &si_me, 0, sizeof(si_me));
	si_me.sin_family = AF_INET;
	si_me.sin_port = htons(destPort);
	si_me.sin_addr.s_addr = htonl(INADDR_ANY);

	if( bind(s, (__CONST_SOCKADDR_ARG) &si_me, (socklen_t) sizeof(si_me)) == -1)
	{
		printf("[ERROR]: utilUdp_receiveUdpMsg - Can't bind the socket.\n");
		printf("Trying again in 10 seconds...\n");
		sleep(10);
	}

	if( recvfrom(s, (void *__restrict) buf, (size_t) UTIL_UDP_BUFFLEN, 0, (__SOCKADDR_ARG) &si_other, (socklen_t *__restrict) &slen) == -1)
	{
		printf("[ERROR]: utilUdp_receiveUdpMsg - Fail to receive packet.\n");
	}

	close(s);

//	printf("[LOG]: utilUdp_receiveUdpMsg - Received packet from %s:%d\n", inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port));

	return true;
}
