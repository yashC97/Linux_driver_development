#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
#include"ioctrl.h"
#include<sys/ioctl.h>

bufferSizeStruct sizeStruct;

int main()
{
	int fp,i;
	char c[5];
	fp = open("/dev/myCharDevice",O_RDWR);
	if (fp<0)
		printf("Failed to open\n");
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
	sizeStruct.bufferSize = 300;
	i = ioctl(fp, SET_BUFFER_SIZE, &sizeStruct);
	close(fp);
	return 0;
}	
