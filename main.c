#include <main.h>

int main(int argc, char** argv)
{
	char atCmd[50];
	int seq;

	httpServer_init();
	droneTms_init();

	sleep(5);

	seq = 1;
	while(seq < 2)
	{
		ATCMD_CREATE_AT_REF_CMD(atCmd, seq, ATCMD_AT_REF_FIXED_BITS | ATCMD_AT_REF_TAKE_OF_FLAG);
		droneTcs_sendAtCmd(atCmd);
		seq++;
	}

	sleep(10);

	seq = 1;
	while(seq < 10)
	{
		ATCMD_CREATE_AT_REF_CMD(atCmd, seq, ATCMD_AT_REF_FIXED_BITS | ATCMD_AT_REF_LAND_FLAG);
		droneTcs_sendAtCmd(atCmd);
		seq++;
	}

	while(true)
	{
		sleep(60);
		printf("\nI'am Alive!");
	}

	return 0;
}
