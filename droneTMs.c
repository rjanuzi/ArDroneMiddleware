#include "droneTMs.h"

volatile static droneTms_threads_t droneTms_threads;
volatile static uint8_t droneTms_comWdt = DRONE_TMS_WDT_RESET_VAL;

/* TM and Photo DataPoll */
volatile static droneTms_tmData_t droneTms_lastTmData =
{
		.alt = 0.0,
		.lat = 0.0,
		.lon = 0.0,
		.vel_x = 0.0,
		.vel_y = 0.0,
		.vel_z = 0.0,
		.pitch = 0.0,
		.roll = 0.0,
		.yaw = 0.0,
		.temp = 0.0,
		.bat = 0.0,
		.press = 0.0
};

uint8_t droneTms_lastImgData[DRONE_PHOTO_MAX_LEN];
uint16_t droneTms_lastImgDataLen = 0;

void* droneTms_tmReceiverThread(void *arg)
{
	static struct sockaddr_in si_me, si_other;
	static int s, i, slen=sizeof(si_other);
	static uint8_t buf[UTIL_UDP_BUFFLEN];

	utilUdp_sendUdpMsg("SOME BYTES", DRONE_IP, DRONE_NAVDATA_PORT, DRONE_NAVDATA_PORT);

	droneTcs_sendAtCmd(ATCMD_AT_COM_WDT);

	utilUdp_sendUdpMsg("SOME BYTES", DRONE_IP, DRONE_NAVDATA_PORT, DRONE_NAVDATA_PORT);

	while( true )
	{
		droneTms_comWdt = DRONE_TMS_WDT_RESET_VAL; /* When this thread runs, some data has been received, so the communication is on. */

		printf("\nReceive Loop\n");

		memset((void*) buf, 0, (size_t) UTIL_UDP_BUFFLEN);
		utilUdp_receiveUdpMsg( buf, DRONE_NAVDATA_PORT);

		droneTms_hexDump(buf, 20);
	}

	return NULL;
}

void* droneTms_comWdtCheckerThread(void *arg)
{

	while( true )
	{
		/* If com wdt reaches 0, send the RESET WDT AT CMD */
		if(--droneTms_comWdt == 0)
		{
			droneTms_comWdt = DRONE_TMS_WDT_RESET_VAL;
			droneTcs_sendAtCmd(ATCMD_AT_COM_WDT);
		}
		sleep(1);
	}

	return NULL;
}

bool droneTms_init()
{
	int err;

	/* TM Receiver Thread */
	err = pthread_create(&droneTms_threads.tmReceiverThread, NULL, &droneTms_tmReceiverThread, NULL);
	if(err != 0)
	{
		printf("\n[ERROR]: droneTms.droneTms_init - Can't create droneTms_tmReceiverThread: [%s]", strerror(err));
		return false;
	}
	else
	{
		printf("\n[LOG]: droneTms.droneTms_init - droneTms_tmReceiverThread created sucessfully\n");
	}

	/* COM WDT Checker Thread */
	err = pthread_create(&droneTms_threads.comWdtCheckerThread, NULL, &droneTms_comWdtCheckerThread, NULL);
	if(err != 0)
	{
		printf("\n[ERROR]: droneTms_init - Can't create droneTms_comWdtCheckerThread: [%s]", strerror(err));
		return false;
	}
	else
	{
		printf("\n[LOG]: droneTms_init - droneTms_comWdtCheckerThread created sucessfully\n");
	}

	return true;
}

droneTms_threads_t droneTms_getThreadsIds()
{
	return droneTms_threads;
}

droneTms_tmData_t droneTms_getTmData()
{
#if(DRONE_TMS_MOCKED_DATA)
	droneTms_lastTmData.alt = 1.0;
	droneTms_lastTmData.lat = -23.210240;
	droneTms_lastTmData.lon = -45.875479;
	droneTms_lastTmData.vel_x = 5.0;
	droneTms_lastTmData.vel_y = 3.5;
	droneTms_lastTmData.vel_z = 1.2;
	droneTms_lastTmData.pitch = 0.35;
	droneTms_lastTmData.roll = 0.18;
	droneTms_lastTmData.yaw = -47.23;
	droneTms_lastTmData.temp = 25.0;
	droneTms_lastTmData.bat = 73.0;
	droneTms_lastTmData.press = 1.0;
#endif
	return droneTms_lastTmData;
}

uint16_t droneTms_getPhoto(uint8_t* photoDataOut)
{
#if(DRONE_TMS_MOCKED_DATA)
	droneTms_lastImgDataLen = utilImgs_getImgBin("droneMockedImg.jpg", droneTms_lastImgData);
	//droneTms_lastImgDataLen = utilImgs_getImgBin("droneMockedImg2.jpg", droneTms_lastImgData);
#endif

	memset(photoDataOut, 0, DRONE_PHOTO_MAX_LEN);
	memcpy(photoDataOut, droneTms_lastImgData, droneTms_lastImgDataLen);

	return droneTms_lastImgDataLen;
}

void droneTms_hexDump(uint8_t* buf, uint16_t len)
{
	printf("\nHex Dump: ");
	for(uint16_t i = 0; i < len; i++)
		printf(" 0x%x ", buf[i]);
}
