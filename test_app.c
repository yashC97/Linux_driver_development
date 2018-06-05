#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
#include"ioctrl.h"
#include<sys/ioctl.h>
#include<signal.h>

bufferSizeStruct sizeStruct;

void my_notifier(int signo, siginfo_t *sigInfo, void *data)
{
	printf("Signal received from the driver expected %d got %d \n",SIGIO,signo);
}

int main()
{
	struct sigaction signalInfo;
	int flagInfo;

	signalInfo.sa_sigaction = my_notifier;
	signalInfo.sa_flags = SA_SIGINFO;
	sigemptyset(&signalInfo.sa_mask);
	
	sigaction(SIGIO, &signalInfo, NULL);
	

	int fp,i;
	char c[5];
	fp = open("/dev/myCharDevice",O_RDWR);
	if (fp<0)
		printf("Failed to open\n");

	/*New we will own the device so that we can get the signal from the device*/
	
	fcntl(fp, F_SETOWN, getpid());
	flagInfo = fcntl(fp, F_GETFL);
	fcntl(fp, F_SETFL, flagInfo|FASYNC);
	sleep(5);
	printf("Scheduling work queue\n");
	ioctl(fp, ADD_TO_QUEUE );
	//printf("Sending the drive to sleep for a while \n");
	//ioctl(fp, GO_TO_SLEEP);
	while(1);
	printf("attemptint to write 'YASH'\n");
	i = write(fp,"YASH",4);
	if (i < 4 )
	{
		printf("Error Writing!\n");
		return -1;
	}
	printf("Enter enter to continue");
	getchar();
	printf("Attempting to read!\n");
	i = read(fp,&c,4);
	printf("Data read : %s",c);
	printf("\nNow reading the buffer Size\n");
	i = ioctl(fp, READ_BUFFER_SIZE, &sizeStruct);
	printf("BufferSize : %d\nWaiting\n",sizeStruct.bufferSize);
	getchar();
	printf("Now Writing bufferSize to 300\n");
	sizeStruct.bufferSize = 30;
	i = ioctl(fp, SET_BUFFER_SIZE, &sizeStruct);
	printf("\nNow reading the buffer Size\n");
	i = ioctl(fp, READ_BUFFER_SIZE, &sizeStruct);
	printf("BufferSize : %d\nWaiting\n",sizeStruct.bufferSize);

	close(fp);
	return 0;
}	
