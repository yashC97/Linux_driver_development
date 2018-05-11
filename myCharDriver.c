#include<linux/modules.h>
#include<linux/init.h>
#include<linux/fs.h>
#include<linux/device.h>
#include<linux/kernel.h>
#include<linux/slab.h>

DEVICE_NAME = "myCharDevice"
MODULE_NAME = "myCharDriver"
CLASS_NAME = "myCharClass"


static char *bufferMemory;
static int bufferPointer;
static struct class *pmyCharClass;
static struct device *pmyCharDevice;

static struct file_operations fops = 
{
	.open = charDriverOpen;
	.release = charDriverClose;
	.read = charDriverRead;
	.write = charDriverWrite;
}

int majorNumber = 0;

static int charDriverEntry()
{
	majorNumber = register_chrdev(0, DEVICE_NAME, &fops);
	if (majorNumber < 0)
	{
		printk(KERN_ALERT "ERROR : Can not register device. Did not get major number!\n");
		return majorNumber;
	}
	printk(KERN_INFO "Aquired Major Number!\n");
	// Now we will create class for this device
	pmyCharClass = class_create(THIS_MODULE,CLASS_NAME);
	printk(KERN_INFO "Class created!\n");
// check for error | Not added here will add later 
	pmyCharDevice = device_create(pmyCharClass, NULL, MKDEV(majorNumber,0),NULL,DEVICE_NAME);
	printk(KERN_INFO "Device created!\n");
	return 0;
}

static void charDriverExit()
{
	device_destroy(pmyCharClass, MKDEV(majorNumber,0));
	class_unregister(pmyCharClass);
	class_destroy(pmyCharClass);
	unregister_chrdev(majorNumber,DEVICE_NAME);
}

static int charDriverOpen(struct inode *inodep, struct file *filep)
{
	printk(KERN_INFO "INFO : CHARATER DRIVER OPENED\n");
	bufferMemory = kmalloc(5,GFP_KERNEL);
	bufferPointer = 0;	
}

static int charDriverClose(struct inode *inodep, struct file *filep)
{
	kfree(bufferMemory);
	printk(KERN_INFO "INFO : CHARACTER DRIVER CLOSED\n");
}

static ssize_t charDriverWrite(struct file *filep, const char *buffer, size_t len, loff_t *offset)
{
	// Here we will only allow to write one byte of data 
	if (len != 1)
	{
		printk(KERN_WARNING "Attempted to write data larger or smaller than 1 byte!\n");
		return 0;
	}
	if(bufferPointer > 5)
	{
		printk(KERN_WARNING "No space in Buffer to write data!\n");
		return 0;
	}
	bufferMemory[bufferPointer] = *buffer;
	bufferPointer++;
	return len;
}

static ssize_t charDriverRead(struct file *filep, const char *buffer, size_t len, loff_t *offset)
{
	if(len > 4 || len > bufferPointer)
	{
		printk(KERN_WARNING "Attempting to read more than buffer size ! Deny");
		return 0;
	}
	copy_to_user(buffer, bufferMemory, len);
	bufferPointer -= len;
	return len;
}
