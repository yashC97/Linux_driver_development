#include<linux/ioctl.h>

#define MAGIC 0xA5

enum query{
set,
read
};

typedef struct{
	int bufferSize;
}bufferSizeStruct;

#define SET_BUFFER_SIZE _IOW(MAGIC, set, bufferSizeStruct *)
#define READ_BUFFER_SIZE _IOR(MAGIC, read, bufferSizeStruct *)

