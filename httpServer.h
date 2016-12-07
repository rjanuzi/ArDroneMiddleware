/**
 * \file
 *
 * @author Rafael B. Januzi (rjanuzi@gmail.com)
 * @date 07/11/2016
 *
 */

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
 * Envia uma String para um client HTTP.
 *
 * @param client: Codigo do cliente aberto para enviar a String.
 * @param strToSend: String para enviar via HTTP.
 */
void sendString(int client, const char* strToSend);


/**
 * @date 07/11/2016
 * @autho Rafael B. Januzi (rjanuzi@gmail.com)
 *
 * Inicializa o server HTTP.
 *
 * @return true caso a inicializacao ocorra com sucesso e false caso contrario.
 */
bool httpServer_init(void);

/**
 * @date 09/11/2016
 * @autho Rafael B. Januzi (rjanuzi@gmail.com)
 *
 * Envia as telemetrias disponibilizadas pelo modulo droneTMs para o client.
 *
 * @param client: Codigo do cliente aberto para enviar a String.
 */
void httpServer_sendTms(int client);

/**
 * @date 14/11/2016
 * @autho Rafael B. Januzi (rjanuzi@gmail.com)
 *
 * Envia a imagem disponibilizada pelo modulo droneTMs para o client.
 *
 * @param client: Codigo do cliente aberto para enviar a String.
 */
void httpServer_sendPhotoData(int client);

/**
 * @date 14/11/2016
 * @autho Rafael B. Januzi (rjanuzi@gmail.com)
 *
 * Envia o header de uma imagem JPG para o client HTTP.
 *
 * @param client: Codigo do cliente aberto para enviar a String.
 * @param imgLenBytes: Numero de bytes da imagem que sera envia em seguida.
 */
void httpServer_headersJPG(int client, uint32_t imgLenBytes);

/**
 * @date 16/11/2016
 * @autho Rafael B. Januzi (rjanuzi@gmail.com)
 *
 * Envia a pagina de teste de telecomandos para o client HTTP.
 *
 * @param client: Codigo do cliente aberto para enviar a String.
 * @param pageFile: Caminho do arquivo no HD para enviar.
 */
void httpServer_sendCmdTestPage(int client, const char* pageFile);

/**
 * @date 16/11/2016
 * @autho Rafael B. Januzi (rjanuzi@gmail.com)
 *
 * Envia uma pagina HTML para o client HTTP.
 *
 * @param client: Codigo do cliente aberto para enviar a String.
 * @param pageFile: Caminho do arquivo no HD para enviar.
 */
void httpServer_sendPage(int client, const char* pageFile);

/**
 * @date 16/11/2016
 * @autho Rafael B. Januzi (rjanuzi@gmail.com)
 *
 * Envia o cabecalho HTTP para uma pagina HTML.
 *
 * @param client: Codigo do cliente aberto para enviar a String.
 * @param size: Tamanho da pagina HTML a ser enviada em seguida.
 */
void httpServer_headersHtml(int client, uint16_t size);

/**
 * @date 16/11/2016
 * @autho Rafael B. Januzi (rjanuzi@gmail.com)
 *
 * Funcao utilitaria para verificacao se uma String comeca uma outra String.
 *
 * @param pre: Prefixo a ser buscado na outra String.
 * @param str: String para buscar o prefixo.
 *
 * @return true se o prefixo foi encontrado na String e false caso contrario.
 */
bool httpServer_strStartsWith(const char* pre, const char* str);

/**
 * @date 16/11/2016
 * @autho Rafael B. Januzi (rjanuzi@gmail.com)
 *
 * Descobre o parametro do cmd envia no POST da pagina de teste de telecomandos.
 *
 * @param [out] valOut: Valor do parametro obtido.
 * @param cmdParam: String com o parametro envio no POST.
 */
void httpServer_getCmdParamVal(char* valOut, const char* cmdParam);

/**
 * @date 16/11/2016
 * @autho Rafael B. Januzi (rjanuzi@gmail.com)
 *
 * Executa um comando enviado por POST para o servidor.
 *
 * @param cmd: Comando enviado como parametro no POST.
 */
void httpServer_execDroneCmd(const char* cmd);

/**
 * @date 19/11/2016
 * @autho Rafael B. Januzi (rjanuzi@gmail.com)
 *
 * Funcao para verificar se o Server estah ON e sem erros.
 *
 * @return true se o server estah ONLINE e false caso contrario.
 */
bool httpServer_isServerOk();

#endif /* HTTPSERVER_H_ */
