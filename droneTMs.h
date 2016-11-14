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
#define DRONE_TMS_VERBOSE		1
#define DRONE_TMS_MOCKED_DATA	1

typedef struct {
	pthread_t tmReceiverThread;
} droneTms_threads_t;

typedef struct {
	float alt;
	float lat;
	float lon;
	float vel_x;
	float vel_y;
	float vel_z;
	float pitch;
	float roll;
	float yaw;
	float temp;
	float bat;
	float press;
} droneTms_tmData_t;

typedef struct {
	char* imgHex;
} droneTms_photo_t;

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
droneTms_threads_t droneTms_getThreadsIds();

/**
 * @date 14/11/2016
 * @autho Rafael B. Januzi (rjanuzi@gmail.com)
 *
 * TODO
 */
droneTms_tmData_t droneTms_getTmData();

/**
 * @date 14/11/2016
 * @autho Rafael B. Januzi (rjanuzi@gmail.com)
 *
 * TODO
 */
droneTms_photo_t droneTms_getPhoto();

#endif /* droneTms_H_ */
