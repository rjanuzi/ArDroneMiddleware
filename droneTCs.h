/**
 * \file
 *
 * @author Rafael B. Januzi (rjanuzi@gmail.com)
 * @date 16/11/2016
 */

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
 * Thread para receber e tratar as respostas enviadas pelo Drone para telecomandos
 * executados.
 *
 * @param arg: Nao utilizado, apenas existe por padronizacao de Threads.
 */
void* droneTcs_tcRespReceiverThread(void *arg);

/**
 * @date 06/11/2016
 * @autho Rafael B. Januzi (rjanuzi@gmail.com)
 *
 * Inicializa o modulo de envio de telecomandos ao Drone, basicamente
 * cria a Thread de recepcao de respostas de telecomandos.
 *
 * @return true caso inicializacao concluida com sucess e false caso contrario.
 */
bool droneTcs_init();

/**
 * @date 06/11/2016
 * @autho Rafael B. Januzi (rjanuzi@gmail.com)
 *
 * Recupera os handles das Threads do modulo de telecomandos.
 *
 * @return Estrutura de dados com o handle das Threads do modulo de telecomandos.
 */
droneTcs_threads droneTcs_getThreadsIds();

/**
 * @date 06/11/2016
 * @autho Rafael B. Januzi (rjanuzi@gmail.com)
 *
 * Envia um comando AT ao Drone.
 * Dica: Utilizar as macros definidas no arquivo \ref droneAtCommands.h (ATCMD_CREATE_AT_*) para 
 * a criacao dos comandos AT corretamente.
 *
 * @param cmd: String com o comando AT para enviar.
 *
 * @return true se o comando foi enviado corretamente e false caso contrario.
 */
bool droneTcs_sendAtCmd(const char* cmd);

/**
 * @date 21/11/2016
 * @autho Rafael B. Januzi (rjanuzi@gmail.com)
 *
 * O Drone necessita que os comandos AT enviado possuam um valor crescente de Sequence Number,
 * essa funcao fornece o proximo Sequence Number a ser utilizado, baseando-se na quantidade de 
 * comandos AT enviados utilizando o modulo.
 *
 * @return O proximo Sequence Number a ser utilizados nos comandos AT.
 */
uint32_t droneTcs_getNextSeqNmbr();

#endif /* DRONETCS_H_ */
