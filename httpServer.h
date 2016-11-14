#ifndef HTTPSERVER_H_
#define HTTPSERVER_H_

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <ctype.h>
#include <strings.h>
#include <string.h>
#include <sys/stat.h>
#include <pthread.h>
#include <sys/wait.h>
#include <stdlib.h>

#include <droneTMs.h>

#define HTTP_SERVER_PORT		5000

#define HTTP_SERVER_TM_URL		"/ardroneMiddleware/tm"
#define HTTP_SERVER_PHOTO_URL	"/ardroneMiddleware/photo"

#define HTTP_SERVER_TM_JSON_TEMPLATE	"{\"alt\":%f,\"lat\":%f,\"lon\":%f,\"vel_x\":%f,\"vel_y\":%f,\"vel_z\":%f,\"pitch\":%f,\"roll\":%f,\"yaw\":%f,\"temp\":%f,\"bat\":%f,\"press\":%f}"
#define HTTP_SERVER_TM_JSON_CREATE(tmData, jsonOut) { sprintf(jsonOut, HTTP_SERVER_TM_JSON_TEMPLATE, tmData.alt, tmData.lat, tmData.lon, tmData.vel_x, tmData.vel_y, tmData.vel_z, tmData.pitch, tmData.roll, tmData.yaw, tmData.temp, tmData.bat, tmData.press); };

/**
 * @date 07/11/2016
 * @autho Rafael B. Januzi (rjanuzi@gmail.com)
 *
 * TODO
 */
void sendString(int client, const char* strToSend);


/**
 * @date 07/11/2016
 * @autho Rafael B. Januzi (rjanuzi@gmail.com)
 *
 * TODO
 */
bool httpServer_init(void);

/**
 * @date 09/11/2016
 * @autho Rafael B. Januzi (rjanuzi@gmail.com)
 *
 * TODO
 */
void httpServer_sendTms(int client);

/**
 * @date 14/11/2016
 * @autho Rafael B. Januzi (rjanuzi@gmail.com)
 *
 * TODO
 */
void httpServer_sendPhotoData(int client);

/**
 * @date 14/11/2016
 * @autho Rafael B. Januzi (rjanuzi@gmail.com)
 *
 * TODO
 */
void httpServer_headersJPG(int client, uint16_t imgLenBytes);

#endif /* HTTPSERVER_H_ */
