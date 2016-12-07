/**
 * \file
 *
 * @author Rafael B. Januzi (rjanuzi@gmail.com)
 * @date 06/11/2016
 */

#ifndef UTILUDP_H_
#define UTILUDP_H_

/* General Libs */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>

/* Sockets */
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>

#define UTIL_UDP_BUFFLEN	4096

/**
 * @date 06/11/2016
 * @autho Rafael B. Januzi (rjanuzi@gmail.com)
 *
 * Envia uma mensagem UDP via conexao de rede.
 *
 * @param msg: Mensagem a ser enviada (bytes).
 * @param len: Tamanho da mensagem para enviar.
 * @hostIp: IP do host destino (111.111.111.111).
 * @destPort: Porta de detino no host de destino.
 * @srcPort: Porta de saida do pacote.
 *
 * @return true se a mensagem foi enviada corretamente e false caso contrario.
 */
bool utilUdp_sendUdpMsg(const uint8_t* msg, uint16_t len, const char* hostIp, int destPort, int srcPort);

/**
 * @date 15/11/2016
 * @autho Rafael B. Januzi (rjanuzi@gmail.com)
 *
 * Le uma mensagem UDP (se disponivel) do buffer de recepcao.
 *
 * @param buf [out]: Buffer de saida.
 * @param destPort: Porta de destino do pacote UDP que deseja receber.
 *
 * @return true caso leia um pacote UDP com sucesso e false caso contrario.
 */
bool utilUdp_receiveUdpMsg(uint8_t* buf, int destPort);

#endif /* UTILUDP_H_ */
