#include <droneTMs.h>

volatile static droneTms_threads_t droneTms_threads;
volatile static uint8_t droneTms_comWdt = DRONE_TMS_WDT_RESET_VAL;
volatile static droneTms_navdataFrame_t droneTms_lastNavdataFrame;

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
uint32_t droneTms_lastImgDataLen = 0;

void* droneTms_tmReceiverThread(void *arg)
{
	static struct sockaddr_in si_me, si_other;
	static int s, i, slen=sizeof(si_other);
	static uint8_t buf[UTIL_UDP_BUFFLEN];
	char cmd[20];

	memset((void*) buf, 0, (size_t) UTIL_UDP_BUFFLEN);
	buf[0] = 0x01;

	ATCMD_CREATE_AT_SET_NAVDATA_DEMO_MODE(cmd, droneTcs_getNextSeqNmbr());
	droneTcs_sendAtCmd(cmd);
	ATCMD_CREATE_AT_FLAT_TRIM(cmd, droneTcs_getNextSeqNmbr());
	droneTcs_sendAtCmd(cmd);

	utilUdp_sendUdpMsg(buf, 4, DRONE_IP, DRONE_VIDEO_STREAM_PORT, DRONE_VIDEO_STREAM_PORT);
	utilUdp_sendUdpMsg(buf, 4, DRONE_IP, DRONE_NAVDATA_PORT, DRONE_NAVDATA_PORT);

	while( true )
	{
		droneTms_comWdt = DRONE_TMS_WDT_RESET_VAL; /* When this thread runs, some data has been received, so the communication is on. */

		memset((void*) buf, 0, (size_t) UTIL_UDP_BUFFLEN);
		utilUdp_receiveUdpMsg( buf, DRONE_NAVDATA_PORT);

		memcpy((void*) &droneTms_lastNavdataFrame, buf, sizeof(droneTms_navdataFrame_t)); /* Salva a ultima versao do frame */
		droneTms_lastTmData.alt = droneTms_lastNavdataFrame.navdataDemo.altitude;
		droneTms_lastTmData.bat = droneTms_lastNavdataFrame.navdataDemo.vbat_flying_percentage;
		droneTms_lastTmData.pitch = droneTms_lastNavdataFrame.navdataDemo.theta / 1000.0;
		droneTms_lastTmData.roll = droneTms_lastNavdataFrame.navdataDemo.phi / 1000.0;
		droneTms_lastTmData.yaw = droneTms_lastNavdataFrame.navdataDemo.psi / 1000.0;
		droneTms_lastTmData.vel_x = droneTms_lastNavdataFrame.navdataDemo.vx;
		droneTms_lastTmData.vel_y = droneTms_lastNavdataFrame.navdataDemo.vy;
		droneTms_lastTmData.vel_z = droneTms_lastNavdataFrame.navdataDemo.vz;

//		droneTms_printNavdataFrame(droneTms_lastNavdataFrame);
	}

	return NULL;
}

void* droneTms_comKeepAlive(void *arg)
{
	char cmd[20];

	while( true )
	{
		droneTms_comWdt = DRONE_TMS_WDT_RESET_VAL;
		ATCMD_CREATE_AT_COM_WDT(cmd, droneTcs_getNextSeqNmbr());
		droneTcs_sendAtCmd(cmd);

		usleep(DRONE_TMS_COM_KEEP_ALIVE_THREAD_DELAY_US); /* Se nao houver trafego em mais de 50 ms 2 segundos o drone corta a comunicacao */
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
	err = pthread_create(&droneTms_threads.comKeepAliveThread, NULL, &droneTms_comKeepAlive, NULL);
	if(err != 0)
	{
		printf("\n[ERROR]: droneTms_init - Can't create droneTms_comKeepAlive: [%s]", strerror(err));
		return false;
	}
	else
	{
		printf("\n[LOG]: droneTms_init - droneTms_comKeepAlive created sucessfully\n");
	}

	return true;
}

droneTms_threads_t droneTms_getThreadsIds()
{
	return droneTms_threads;
}

droneTms_tmData_t droneTms_getTmData()
{
#if(DRONE_TMS_MOCKED_TM)
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

uint32_t droneTms_getPhoto(uint8_t* photoDataOut)
{
#if(DRONE_TMS_MOCKED_PHOTO)
	static uint8_t photoNmbr = 0;

	switch(photoNmbr)
	{
		case 0:
		droneTms_lastImgDataLen = utilImgs_getImgBin(UTIL_IMGS_IMG_MOCKED_1_PATH, droneTms_lastImgData);
		break;

		case 1:
		droneTms_lastImgDataLen = utilImgs_getImgBin(UTIL_IMGS_IMG_MOCKED_2_PATH, droneTms_lastImgData);
		break;

		case 2:
		droneTms_lastImgDataLen = utilImgs_getImgBin(UTIL_IMGS_IMG_MOCKED_3_PATH, droneTms_lastImgData);
		break;

		case 3:
		droneTms_lastImgDataLen = utilImgs_getImgBin(UTIL_IMGS_IMG_MOCKED_4_PATH, droneTms_lastImgData);
		break;
		
		case 4:
		droneTms_lastImgDataLen = utilImgs_getImgBin(UTIL_IMGS_IMG_MOCKED_5_PATH, droneTms_lastImgData);
		break;

		default:
		droneTms_lastImgDataLen = utilImgs_getImgBin(UTIL_IMGS_IMG_MOCKED_6_PATH, droneTms_lastImgData);
		break;
	}

	photoNmbr = ((photoNmbr+1)%5);
#endif

	memset(photoDataOut, 0, DRONE_PHOTO_MAX_LEN);
	memcpy(photoDataOut, droneTms_lastImgData, droneTms_lastImgDataLen);

	return droneTms_lastImgDataLen;
}

void droneTms_hexDump(uint8_t* buf, uint16_t len)
{
	uint16_t i;

	printf("\nHex Dump: ");
	for(i = 0; i < len; i++)
		printf(" 0x%x ", buf[i]);
}

void droneTms_printNavdataFrame(droneTms_navdataFrame_t frame)
{
	printf("\nNavdata Frame:");
	printf("\n\tHeader: 0x%x", frame.header);
	printf("\n\tDrone State: 0x%x", frame.droneState);
	printf("\n\tSequence Nmbr: %d", frame.seqNmbr);
	printf("\n\tVision flag: 0x%x", frame.visionFlag);
	droneTms_printNavdataDemo(frame.navdataDemo);
	droneTms_printNavdataCks(frame.navdataCks);

	printf("\n");
}

void droneTms_printNavdataDemo(droneTms_navdataDemo_t demo)
{
	printf("\n\tNavdata Demo:");
	printf("\n\t\tId: 0x%x", demo.id);
	printf("\n\t\tSize: %d", demo.size);
	printf("\n\t\tCtrl State: 0x%x", demo.ctrl_state);
	printf("\n\t\tVbat: %d perCent", demo.vbat_flying_percentage);
	printf("\n\t\tTheta (Pitch): %f", demo.theta / 1000.0);
	printf("\n\t\tPhi (Roll): %f", demo.phi / 1000.0);
	printf("\n\t\tPsi (Yaw): %f", demo.psi / 1000.0);
	printf("\n\t\tAltitude: %d", demo.altitude);
	printf("\n\t\tVelocity (x, y, z): (%f, %f, %f)", demo.vx, demo.vy, demo.vz);

	//The others fields is not important in the moment.
}

void droneTms_printNavdataCks(droneTms_navdataCks_t cks)
{
	printf("\n\tChecksum: ");
	printf("\n\t\tId: 0x%x", cks.id);
	printf("\n\t\tSize: %d", cks.size);
	printf("\n\t\tChecksum: 0x%x", cks.cks);
}
