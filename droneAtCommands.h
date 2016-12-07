/**
 * \file
 *
 * Este arquivo contem definicoes de constantes e macros para facilitar a 
 * criacao de comandos AT aceitados pelo Drone.
 *
 * @author Rafael B. Januzi (rjanuzi@gmail.com)
 * @date 06/11/2016
 */

#ifndef DRONEATCOMMANDS_H_
#define DRONEATCOMMANDS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ATCMD_AT_REF					"AT*REF=%d,%d\r"
#define ATCMD_AT_COM_WDT				"AT*COMWDG=%d\r"
#define ATCMD_AT_SET_NAVDATA_DEMO_MODE	"AT*CONFIG=%d,\"general:navdata_demo\",\"TRUE\"\r"
#define ATCMD_AT_FLAT_TRIM				"AT*FTRIM=%d\r"

#define ATCMD_CREATE_AT_REF_CMD(strOut, seqNbr, uint32CtrlMask)	{sprintf(strOut, ATCMD_AT_REF, seqNbr, uint32CtrlMask);}
#define ATCMD_CREATE_AT_COM_WDT(strOut, seqNbr)					{sprintf(strOut, ATCMD_AT_COM_WDT, seqNbr);}
#define ATCMD_CREATE_AT_SET_NAVDATA_DEMO_MODE(strOut, seqNbr)	{sprintf(strOut, ATCMD_AT_SET_NAVDATA_DEMO_MODE, seqNbr);}
#define ATCMD_CREATE_AT_FLAT_TRIM(strOut, seqNbr)				{sprintf(strOut, ATCMD_AT_FLAT_TRIM, seqNbr);}

#define ATCMD_AT_REF_FIXED_BITS		 0b00010001010101000000000000000000
#define ATCMD_AT_REF_TAKE_OF_FLAG	 0b00000000000000000000001000000000
#define ATCMD_AT_REF_LAND_FLAG		 0b00000000000000000000000000000000
#define ATCMD_AT_REF_EMERGENCY_ORDER 0b00000000000000000000000100000000

#endif /* DRONEATCOMMANDS_H_ */
