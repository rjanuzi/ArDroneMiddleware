#ifndef UTILUDP_H_
#define UTILUDP_H_

/* General Libs */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>

/* Sockets */
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>

#define UTIL_UDP_BUFFLEN	2048

/**
 * @date 06/11/2016
 * @autho Rafael B. Januzi (rjanuzi@gmail.com)
 *
 * TODO
 */
bool utilUdp_sendUdpMsg(const char* msg, const char* hostIp, int destPort, int srcPort);

#endif /* UTILUDP_H_ */
