#include "droneTMs.h"

volatile static droneTms_threads_t droneTms_threads;

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
	static char buf[UTIL_UDP_BUFFLEN];

	printf("[LOG]: droneTms.droneTms_tmReceiverThread - Sending SOME BYTES to Drone.\n");
	while(!utilUdp_sendUdpMsg("SOME BYTES", DRONE_IP, DRONE_NAVDATA_PORT, DRONE_NAVDATA_PORT))
	{
		printf("Trying again in 10 seconds...\n");
		sleep(10);
	}

	while( true )
	{
		printf("\nReceive Loop\n\n\n");

		while( (s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1 )
		{
			printf("\n[ERROR]: droneTms.droneTms_tmReceiverThread - Can't open the socket.\n");
			printf("Trying again in 10 seconds...\n");
			sleep(10);
		}

		memset((char *) &si_me, 0, sizeof(si_me));
		si_me.sin_family = AF_INET;
		si_me.sin_port = htons(DRONE_NAVDATA_PORT);
		si_me.sin_addr.s_addr = htonl(INADDR_ANY);

		if( bind(s, (__CONST_SOCKADDR_ARG) &si_me, (socklen_t) sizeof(si_me)) == -1)
		{
			printf("[ERROR]: droneTms.droneTms_tmReceiverThread - Can't bind the socket.\n");
			printf("Trying again in 10 seconds...\n");
			sleep(10);
		}

		if( recvfrom(s, (void *__restrict) buf, (size_t) UTIL_UDP_BUFFLEN, 0, (__SOCKADDR_ARG) &si_other, (socklen_t *__restrict) &slen) == -1)
		{
			printf("[ERROR]: droneTms.droneTms_tmReceiverThread - Fail to receive packet.\n");
		}

		printf("Received packet from %s:%d\nData: %s\n\n", inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port), buf);

		close(s);
	}

	return NULL;
}

bool droneTms_init()
{
	int err;

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
