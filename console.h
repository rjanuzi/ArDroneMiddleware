/**
 * \file
 *
 * @author Rafael B. Januzi (rjanuzi@gmail.com)
 * @date 16/11/2016
 *
 */

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
 * Thread para recepcao e execucao de entradas pelo console.
 *
 * @param arg: Parametros opcionais para threads. (Nao utilizado nessa versao, apenas adicionado 
 * por padronizacao de Threads)
 */
void* console_inputReceiverThread (void *arg);

/**
 * @date 16/11/2016
 * @autho Rafael B. Januzi (rjanuzi@gmail.com)
 *
 * Inicializa o sistema de console, basicamente cria a Thread de recepcao e execucao de entradas.
 *
 * @return true caso a inicializacao tenha sido realizada com sucesso e false caso contrario
 */
bool console_init();

/**
 * @date 16/11/2016
 * @autho Rafael B. Januzi (rjanuzi@gmail.com)
 *
 * Recupera o handle da Thread, de recepcao e execucao de entradas pelo console, criada com a funcao
 * de inicializacao.
 *
 * @return Handle da Thread de recepcao e execucao de comandos via console.
 */
pthread_t console_getInputReceiverThread();

#endif /* DRONETCS_H_ */
