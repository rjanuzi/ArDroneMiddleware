#include <util_imgs.h>

bool utilImgs_getHexString(const char* imgFilePath, char* strOut)
{
	FILE* imgFile = fopen(imgFilePath, "r");
	uint8_t bytesBlock[UTIL_IMGS_MEMO_BLOCK_SIZE];
	uint32_t lenReaded;
	char charAux[2];

	strOut[0] = '\0';

	if(imgFile == NULL)
	{
		return false;
	}

	while( (lenReaded = fread((void*)bytesBlock, (size_t) 1, UTIL_IMGS_MEMO_BLOCK_SIZE, imgFile)) > 0)
	{
		for(int i = 0; i < lenReaded; i++)
		{
			if(bytesBlock[i] > 0xf)
			{
				sprintf(charAux, "%x", bytesBlock[i]);
			}
			else
			{
				sprintf(charAux, "0%x", bytesBlock[i]);
			}
			strcat(strOut, charAux);
		}
	}

	fclose(imgFile);

	return true;
}

int utilImgs_getImgBin(const char* imgFilePath, uint8_t* outArray)
{
	FILE* imgFile = fopen(imgFilePath, "r");
	uint16_t lenReaded;
	int index = 0;

	if(imgFile == NULL)
	{
		return -1;
	}

	while( (lenReaded = fread((void*)&outArray[index], (size_t) 1, UTIL_IMGS_MEMO_BLOCK_SIZE, imgFile)) > 0)
	{
		index += lenReaded;
	}

	fclose(imgFile);

	return index;
}

bool utilImgs_hexToFile(const char* hexString, char* filePath)
{
	FILE* fileToSave = fopen(filePath, "w");
	char byteInHex[3];
	uint8_t byteAux;
	uint32_t lenReaded;

	if(fileToSave == NULL)
	{
		printf("\n[ERROR]: util_imgs.utilImgs_hexToFile - Fail opening the output file");
		return false;
	}

	byteInHex[2] = '\0';

	for(int i = 0; hexString[i+1] != '\0'; i += 2) /* Le de 2 em 2 chars (1 byte) */
	{
		byteInHex[0] = hexString[i];
		byteInHex[1] = hexString[i+1];

		byteAux = utilImgs_hexToByte(byteInHex);

		if(fwrite((const void*) &byteAux, (size_t) 1, (size_t) 1, fileToSave) <= 0)
		{
			printf("\n[ERROR]: util_imgs.utilImgs_hexToFile - Fail writing in the output file");
			fclose(fileToSave);
			return false;
		}
	}

	fclose(fileToSave);

	return true;
}

uint8_t utilImgs_hexToByte(const char* hexByteString)
{
	uint8_t byte = 0x00;

	byte = ((utilImg_getHexCharVal(hexByteString[0])<<4)&0xF0);
	byte |= (utilImg_getHexCharVal(hexByteString[1])&0x0F);

	return byte;
}

uint8_t utilImg_getHexCharVal(char hexChar)
{
	char auxStr[5];
	if(hexChar >= '0' && hexChar <= '9')
	{
		sprintf(auxStr, "%c", hexChar);
		return (uint8_t) atoi(auxStr);
	}

	switch(hexChar)
	{
	case 'a':
	case 'A':
		return 10;
	case 'b':
	case 'B':
		return 11;
	case 'c':
	case 'C':
		return 12;
	case 'd':
	case 'D':
		return 13;
	case 'e':
	case 'E':
		return 14;
	case 'f':
	case 'F':
		return 15;
	default:
		return 0;
	}
}
