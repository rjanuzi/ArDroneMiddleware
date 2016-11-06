#ifndef ARDRONETMS_H_
#define ARDRONETMS_H_

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
#include <arDroneDefines.h>

/* General defines */
#define arDroneTms_VERBOSE       1


typedef struct {
	pthread_t tmReceiverThread;
} arDroneTms_threads;

/**
 * @date 06/11/2016
 * @autho Rafael B. Januzi (rjanuzi@gmail.com)
 *
 * TODO
 */
bool arDroneTms_init();

/**
 * @date 06/11/2016
 * @autho Rafael B. Januzi (rjanuzi@gmail.com)
 *
 * TODO
 */
arDroneTms_threads arDroneTms_getThreadsIds();

#endif /* ARDRONETMS_H_ */