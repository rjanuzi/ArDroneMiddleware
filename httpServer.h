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
#include <droneTCs.h>

#define HTTP_SERVER_PORT		5001
#define HTTP_PAGE_FILE_MAX_LEN	4096

#define HTTP_SERVER_TM_URL		"/ardroneMiddleware/tm"
#define HTTP_SERVER_PHOTO_URL	"/ardroneMiddleware/photo"

#define HTTP_SERVER_CMD_URL		"/ardroneMiddleware/cmd"
#define HTTP_SERVER_CMD_PARAM	"cmd="
#define HTTP_SERVER_CMD_TAKEOFF	"takeoff"
#define HTTP_SERVER_CMD_LAND	"land"

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
void httpServer_headersJPG(int client, uint32_t imgLenBytes);

/**
 * @date 16/11/2016
 * @autho Rafael B. Januzi (rjanuzi@gmail.com)
 *
 * TODO
 */
void httpServer_sendCmdTestPage(int client, const char* pageFile);

/**
 * @date 16/11/2016
 * @autho Rafael B. Januzi (rjanuzi@gmail.com)
 *
 * TODO
 */
void httpServer_sendPage(int client, const char* pageFile);

/**
 * @date 16/11/2016
 * @autho Rafael B. Januzi (rjanuzi@gmail.com)
 *
 * TODO
 */
void httpServer_headersHtml(int client, uint16_t size);

/**
 * @date 16/11/2016
 * @autho Rafael B. Januzi (rjanuzi@gmail.com)
 *
 * TODO
 */
bool httpServer_strStartsWith(const char* pre, const char* str);

/**
 * @date 16/11/2016
 * @autho Rafael B. Januzi (rjanuzi@gmail.com)
 *
 * TODO
 */
void httpServer_getCmdParamVal(char* valOut, const char* cmdParam);

/**
 * @date 16/11/2016
 * @autho Rafael B. Januzi (rjanuzi@gmail.com)
 *
 * TODO
 */
void httpServer_execDroneCmd(const char* cmd);

/**
 * @date 19/11/2016
 * @autho Rafael B. Januzi (rjanuzi@gmail.com)
 *
 * TODO
 */
bool httpServer_isServerOk();

#endif /* HTTPSERVER_H_ */
