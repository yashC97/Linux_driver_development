#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
int main()
{
	int fp,i;
	char c;
	fp = open("/dev/myCharDevice",O_RDWR);
	if (fp<0)
		printf("Failed to open\n");
	printf("attemptint to write 'A'\n");
	i = write(fp,"A",1);
	if (i != 1)
	{
		printf("Error Writing!\n");
		return -1;
	}
	printf("Attempting to read!\n");
	i = read(fp,&c,1);
	printf("Data read : %c",c);
	close(fp);
	return 0;
}	
