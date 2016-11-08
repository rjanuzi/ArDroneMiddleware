#ifndef DRONETMS_H_
#define DRONETMS_H_

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
#define droneTms_VERBOSE       1


typedef struct {
	pthread_t tmReceiverThread;
} droneTms_threads;

/**
 * @date 06/11/2016
 * @autho Rafael B. Januzi (rjanuzi@gmail.com)
 *
 * TODO
 */
bool droneTms_init();

/**
 * @date 06/11/2016
 * @autho Rafael B. Januzi (rjanuzi@gmail.com)
 *
 * TODO
 */
droneTms_threads droneTms_getThreadsIds();

#endif /* droneTms_H_ */
