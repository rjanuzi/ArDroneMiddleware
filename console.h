#ifndef DRONETCS_H_
#define DRONETCS_H_

/* Bibliotecas basicas */
#include <stdio.h>
#include <stdlib.h>

/* Threads */
#include <pthread.h>

/* Drone */
#include <droneDefines.h>
#include <droneTCs.h>

/**
 * @date 16/11/2016
 * @autho Rafael B. Januzi (rjanuzi@gmail.com)
 *
 * TODO
 */
bool console_init();

/**
 * @date 16/11/2016
 * @autho Rafael B. Januzi (rjanuzi@gmail.com)
 *
 * TODO
 */
pthread_t console_getInputReceiverThread();

#endif /* DRONETCS_H_ */
