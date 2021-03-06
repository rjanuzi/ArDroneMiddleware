/**
 * \file
 *
 * @author Rafael B. Januzi (rjanuzi@gmail.com)
 * @date 14/11/2016
 *
 */

#ifndef DRONETMS_H_
#define DRONETMS_H_

/* Bibliotecas basicas */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>

/* Util */
#include <util_udp.h>
#include <util_imgs.h>

/* Threads */
#include <pthread.h>

/* Sockets */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <errno.h>

/* Drone */
#include <droneDefines.h>
#include <droneAtCommands.h>
#include <droneTCs.h>

/* General defines */
#define DRONE_TMS_COM_KEEP_ALIVE_THREAD_DELAY_US	50000
#define DRONE_TMS_WDT_RESET_VAL						10
#define DRONE_PHOTO_MAX_LEN							5000000

#define DRONE_TMS_VERBOSE				1
#define DRONE_TMS_MOCKED_TM				1
#define DRONE_TMS_MOCKED_PHOTO			1

typedef float float32_t;

typedef struct {
	pthread_t tmReceiverThread;
	pthread_t comKeepAliveThread;
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

typedef struct __attribute__((packed)) {
	uint32_t m11;
	uint32_t m12;
	uint32_t m13;
	uint32_t m21;
	uint32_t m22;
	uint32_t m23;
	uint32_t m31;
	uint32_t m32;
	uint32_t m33;
} matrix33_t;

typedef struct __attribute__((packed)) {
	union __attribute__((packed)){
		uint32_t v[3];
		struct __attribute__((packed)){
			uint32_t x;
			uint32_t y;
			uint32_t z;
		};
	};
} vector31_t;

typedef struct __attribute__((packed)) {
	uint16_t id;
	uint16_t size;
	uint32_t ctrl_state;             	/*!< Flying state (landed, flying, hovering, etc.) defined in CTRL_STATES enum. */
	uint32_t vbat_flying_percentage; 	/*!< battery voltage filtered (mV) */

	float32_t theta;                  	/*!< UAV's pitch in milli-degrees */
	float32_t phi;                    	/*!< UAV's roll  in milli-degrees */
	float32_t psi;                    	/*!< UAV's yaw   in milli-degrees */

	int32_t  altitude;               	/*!< UAV's altitude in centimeters */

	float32_t vx;                     	/*!< UAV's estimated linear velocity */
	float32_t vy;                     	/*!< UAV's estimated linear velocity */
	float32_t vz;                     	/*!< UAV's estimated linear velocity */

	uint32_t num_frames;			  	/*!< streamed frame index */ // Not used -> To integrate in video stage.

	// Camera parameters compute by detection
	matrix33_t detection_camera_rot;   	/*!<  Deprecated ! Don't use ! */
	vector31_t detection_camera_trans;	/*!<  Deprecated ! Don't use ! */
	uint32_t detection_tag_index;    	/*!<  Deprecated ! Don't use ! */

	uint32_t detection_camera_type;  	/*!<  Type of tag searched in detection */

	// Camera parameters compute by drone
	matrix33_t drone_camera_rot;		/*!<  Deprecated ! Don't use ! */
	vector31_t drone_camera_trans;		/*!<  Deprecated ! Don't use ! */
} droneTms_navdataDemo_t;

typedef struct __attribute__((packed)) {
	uint16_t  id;
	uint16_t  size;
	uint32_t  cks;
} droneTms_navdataCks_t;

typedef struct __attribute__((packed)) {
	uint32_t header;
	uint32_t droneState;
	uint32_t seqNmbr;
	uint32_t visionFlag;
	droneTms_navdataDemo_t navdataDemo;
	droneTms_navdataCks_t navdataCks;
} droneTms_navdataFrame_t;

/**
 * @date 06/11/2016
 * @autho Rafael B. Januzi (rjanuzi@gmail.com)
 *
 * Thread para a recepcao e decodificacao parcial dos frames de telemetria
 * enviados pelo Drone.
 *
 * @param arg: Nao utilizado. Mantido apenas por padronizacao de Threads.
 */
void* droneTms_tmReceiverThread(void *arg);

/**
 * @date 06/11/2016
 * @autho Rafael B. Januzi (rjanuzi@gmail.com)
 *
 * Thread para manter a conexao de telemetrias com o Drone ativa, enviando o comando para 
 * resetar o Watchdog do Drone a cada 50 ms.
 *
 * @param arg: Nao utilizado. Mantido apenas por padronizacao de Threads.
 */
void* droneTms_comKeepAlive(void *arg);

/**
 * @date 06/11/2016
 * @autho Rafael B. Januzi (rjanuzi@gmail.com)
 *
 * Inicializa o modulo de recepcao de telemetrias do Drone. Basicamente cria as
 * threads do modulo.
 */
bool droneTms_init();

/**
 * @date 06/11/2016
 * @autho Rafael B. Januzi (rjanuzi@gmail.com)
 *
 * Recupera os handles das Threads do modulo.
 *
 * @return Estrutura de dados contendo os handles das threads do modulo.
 */
droneTms_threads_t droneTms_getThreadsIds();

/**
 * @date 14/11/2016
 * @autho Rafael B. Januzi (rjanuzi@gmail.com)
 *
 * Recupera a ultima versao dos dados basicos de telemetria recebidos do Drone.
 *
 * @return Estrutura de dados com as ultimas telemetrias basicas recebidas.
 */
droneTms_tmData_t droneTms_getTmData();

/**
 * @date 14/11/2016
 * @autho Rafael B. Januzi (rjanuzi@gmail.com)
 *
 * Recupera a ultima imagem disponivel da camera do Drone.
 * Obs.: Nessa versao a imagem nao eh de fato recebida do Drone, e sim
 * sao utilizadas imagens "mockadas".
 *
 * @param [out] photoDataOut: Array de bytes onde devem ser armazenados os bytes da imagem.
 *
 * @return Quantidade de bytes armazenados no array de saida.
 */
uint32_t droneTms_getPhoto(uint8_t* photoDataOut);

/**
 * @date 15/11/2016
 * @autho Rafael B. Januzi (rjanuzi@gmail.com)
 *
 * Funcao para imprimir no terminal os hexadecimais a partir de um endereco na memoria.
 *
 * @param buf: Ponteiro para endereco inicial da memoria.
 * @param len: Numero de bytes para imprimir.
 */
void droneTms_hexDump(uint8_t* buf, uint16_t len);

/**
 * @date 15/11/2016
 * @autho Rafael B. Januzi (rjanuzi@gmail.com)
 *
 * Funcao auxiliar para imprimir frames de telemetrias decodificadas.
 *
 * @param frame: Frame de telemetrias para imprimir no terminal.
 */
void droneTms_printNavdataFrame(droneTms_navdataFrame_t frame);

/**
 * @date 15/11/2016
 * @autho Rafael B. Januzi (rjanuzi@gmail.com)
 *
 * Funcao auxiliar para imprimir frames de telemetrias decodificadas.
 *
 * @param demo: Frame de telemetrias para imprimir no terminal.
 */
void droneTms_printNavdataDemo(droneTms_navdataDemo_t demo);

/**
 * @date 15/11/2016
 * @autho Rafael B. Januzi (rjanuzi@gmail.com)
 *
 * Funcao auxiliar para imprimir frames de telemetrias decodificadas.
 *
 * @param cks: Frame de telemetrias para imprimir no terminal.
 */
void droneTms_printNavdataCks(droneTms_navdataCks_t cks);

#endif /* droneTms_H_ */
