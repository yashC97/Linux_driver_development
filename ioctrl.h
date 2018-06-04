#ifndef IOCTRLHEADER
#include<linux/ioctl.h>
#define IOCTRLHEADER
#endif

#define MAGIC 0xA5

enum query{
setdata,
readdata,
sleepdriver,
};

typedef struct{
	int bufferSize;
}bufferSizeStruct;

#define SET_BUFFER_SIZE _IOW(MAGIC, setdata, bufferSizeStruct *)
#define READ_BUFFER_SIZE _IOR(MAGIC, readdata, bufferSizeStruct *)
#define GO_TO_SLEEP	_IO(MAGIC, sleepdriver)
