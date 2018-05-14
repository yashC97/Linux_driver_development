#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
int main()
{
	int fp,i;
	char c[5];
	fp = open("/dev/myCharDevice",O_RDWR);
	if (fp<0)
		printf("Failed to open\n");
	printf("attemptint to write 'A'\n");
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
	close(fp);
	return 0;
}	
