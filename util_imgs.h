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
#define UTIL_IMGS_IMG_MOCKED_5_PATH	"imgs/droneMockedImg.jpg"

/**
 * @date 09/11/2016
 * @autho Rafael B. Januzi (rjanuzi@gmail.com)
 *
 * TODO
 */
bool utilImgs_getHexString(const char* imgFilePath, char* strOut);

/**
 * @date 14/11/2016
 * @autho Rafael B. Januzi (rjanuzi@gmail.com)
 *
 * TODO
 */
uint32_t utilImgs_getImgBin(const char* imgFilePath, uint8_t* outArray);

/**
 * @date 14/11/2016
 * @autho Rafael B. Januzi (rjanuzi@gmail.com)
 *
 * TODO
 */
bool utilImgs_hexToFile(const char* hexString, char* filePath);

/**
 * @date 14/11/2016
 * @autho Rafael B. Januzi (rjanuzi@gmail.com)
 *
 * TODO
 */
uint8_t utilImgs_hexToByte(const char* hexByteString);

/**
 * @date 14/11/2016
 * @autho Rafael B. Januzi (rjanuzi@gmail.com)
 *
 * TODO
 */
uint8_t utilImg_getHexCharVal(char hexChar);

#endif /* UTIL_IMGS_H_ */
