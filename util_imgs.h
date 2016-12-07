/**
 * \file
 *
 * @author Rafael B. Januzi (rjanuzi@gmail.com)
 * @date 09/11/2016
 *
 */

#ifndef UTIL_IMGS_H_
#define UTIL_IMGS_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#define UTIL_IMGS_MEMO_BLOCK_SIZE	65536 /* Bytes */

#define UTIL_IMGS_IMG_MOCKED_1_PATH	"imgs/1.jpg"
#define UTIL_IMGS_IMG_MOCKED_2_PATH	"imgs/2.jpg"
#define UTIL_IMGS_IMG_MOCKED_3_PATH	"imgs/3.jpg"
#define UTIL_IMGS_IMG_MOCKED_4_PATH	"imgs/4.jpg"
#define UTIL_IMGS_IMG_MOCKED_5_PATH	"imgs/5.jpg"
#define UTIL_IMGS_IMG_MOCKED_6_PATH	"imgs/droneMockedImg.jpg"

/**
 * @date 09/11/2016
 * @autho Rafael B. Januzi (rjanuzi@gmail.com)
 *
 * Recupera os bytes (em hexadecimais) de uma imagem salva em arquivo para um array.
 *
 * @param imgFilePath: String contendo o caminho ateh o arquivo de imagem.
 * @param [out] strOut: String de hexadecimais recuperados do arquivo (AABBCC, onde AA & BB & CC sao caracteres em hex)
 *
 * @return true caso a recuperacao tenha ocorrido com sucesso e false caso contrario.
 */
bool utilImgs_getHexString(const char* imgFilePath, char* strOut);

/**
 * @date 014/11/2016
 * @autho Rafael B. Januzi (rjanuzi@gmail.com)
 *
 * Recupera os bytes de uma imagem salva em arquivo para um array.
 *
 * @param imgFilePath: String contendo o caminho ateh o arquivo de imagem.
 * @param [out] outArray: Array de bytes com os valores obtidos do arquivo de imagem.
 *
 * @return Numero de bytes recuperados do arquivo.
 */
uint32_t utilImgs_getImgBin(const char* imgFilePath, uint8_t* outArray);

/**
 * @date 14/11/2016
 * @autho Rafael B. Januzi (rjanuzi@gmail.com)
 *
 * Salva um arquivo binario utilizando uma String de hexadecimais como entrada.
 *
 * @param hexString: String de hexadecimais para para criacao do arquivo em binario.
 * @param filePath: String com o caminho ateh o arquivo a ser criado.
 *
 * @return true caso o salvamento tenha acontecido corretamente e false caso contrario.
 */
bool utilImgs_hexToFile(const char* hexString, char* filePath);

/**
 * @date 14/11/2016
 * @autho Rafael B. Januzi (rjanuzi@gmail.com)
 *
 * Converte uma String em hexadecimal (1 Byte) para o seu valor real.
 *
 * @param hexByteString: String hexadecimal contendo 1 Byte.
 *
 * @return Valor do byte em hexadecimal passado por parametro.
 */
uint8_t utilImgs_hexToByte(const char* hexByteString);

/**
 * @date 14/11/2016
 * @autho Rafael B. Januzi (rjanuzi@gmail.com)
 *
 * Converte um caracter hexadecimal (4 bits) para seu valor real.
 *
 * @param hexChar: Caracter representando 4 bits em hexadecimal.
 *
 * @return Valor dos 4 bits convertidos de um caracter hexadecimal.
 */
uint8_t utilImg_getHexCharVal(char hexChar);

#endif /* UTIL_IMGS_H_ */
