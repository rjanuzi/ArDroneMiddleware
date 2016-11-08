#ifndef DRONETCS_H_
#define DRONETCS_H_

/* Bibliotecas basicas */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>

/* Util */
#include <util_udp.h>

/* Threads */
#include <pthread.h>

/* Sockets */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <errno.h>

/* Drone */
#include "droneDefines.h"

/* General defines */
#define droneTcs_VERBOSE       1


typedef struct {
	pthread_t tcRespReceiverThread;
} droneTcs_threads;

/**
 * @date 06/11/2016
 * @autho Rafael B. Januzi (rjanuzi@gmail.com)
 *
 * TODO
 */
bool droneTcs_init();

/**
 * @date 06/11/2016
 * @autho Rafael B. Januzi (rjanuzi@gmail.com)
 *
 * TODO
 */
droneTcs_threads droneTcs_getThreadsIds();

/**
 * @date 06/11/2016
 * @autho Rafael B. Januzi (rjanuzi@gmail.com)
 *
 * TODO
 */
bool droneTcs_sendAtCmd(const char* cmd);

#endif /* DRONETCS_H_ */
