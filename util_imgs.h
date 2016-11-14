#ifndef UTIL_IMGS_H_
#define UTIL_IMGS_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#define UTIL_IMGS_MEMO_BLOCK_SIZE	65536 /* Bytes */

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
