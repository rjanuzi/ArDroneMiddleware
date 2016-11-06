/*
 * atCommands.h
 *
 *  Created on: 6 de nov de 2016
 *      Author: rafael
 */

#ifndef ATCOMMANDS_H_
#define ATCOMMANDS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ATCMD_AT_REF	"AT*REF=%d,%d\r"

#define ATCMD_CREATE_AT_REF_CMD(strOut, seqNbr, uint32CtrlMask)	{sprintf(strOut, ATCMD_AT_REF, seqNbr, uint32CtrlMask);}

#define ATCMD_AT_REF_FIXED_BITS		 0b00010001010101000000000000000000
#define ATCMD_AT_REF_TAKE_OF_FLAG	 0b00000000000000000000001000000000
#define ATCMD_AT_REF_LAND_FLAG		 0b00000000000000000000000000000000
#define ATCMD_AT_REF_EMERGENCY_ORDER 0b00000000000000000000000100000000

#endif /* ATCOMMANDS_H_ */
