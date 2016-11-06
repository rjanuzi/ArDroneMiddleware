#ifndef ARDRONETCS_H_
#define ARDRONETCS_H_

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
#define arDroneTcs_VERBOSE       1


typedef struct {
	pthread_t tcRespReceiverThread;
} arDroneTcs_threads;

/**
 * @date 06/11/2016
 * @autho Rafael B. Januzi (rjanuzi@gmail.com)
 *
 * TODO
 */
bool arDroneTcs_init();

/**
 * @date 06/11/2016
 * @autho Rafael B. Januzi (rjanuzi@gmail.com)
 *
 * TODO
 */
arDroneTcs_threads arDroneTcs_getThreadsIds();

/**
 * @date 06/11/2016
 * @autho Rafael B. Januzi (rjanuzi@gmail.com)
 *
 * TODO
 */
bool arDroneTcs_sendAtCmd(const char* cmd);

#endif /* ARDRONETCS_H_ */
