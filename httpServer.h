#ifndef HTTPSERVER_H_
#define HTTPSERVER_H_

#include <stdio.h>
#include <stdbool.h>
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

#define HTTP_SERVER_PORT	5000

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

#endif /* HTTPSERVER_H_ */
