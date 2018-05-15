/* @author : yash.bhatt
 * @Brief  : This module is an attempt to understand how the paramaters can be exposed to class fs and they can be used to modufy the behavior of the
 	     Code.
 */

#include<linux/module.h>
#include<linux/init.h>
#include<linux/fs.h>
#include<linux/device.h>
#include<linux/kernel.h>
#include<linux/slab.h>
#include<linux/uaccess.h>
#include<linux/stat.h>

#define DEVICE_NAME "myCharDevice"
#define MODULE_NAME "myCharDriver"
#define CLASS_NAME "myCharClass"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("YASH BHATT");
MODULE_VERSION(".01");

static char *bufferMemory;
static int bufferPointer;
static struct class *pmyCharClass;
static struct device *pmyCharDevice;
int majorNumber = 0;

static int charDriverOpen(struct inode *inodep, struct file *filep);
static int charDriverClose(struct inode *inodep, struct file *filep);
static ssize_t charDriverWrite(struct file *filep, const char *buffer, size_t len, loff_t *offset);
static ssize_t charDriverRead(struct file *filep, char *buffer, size_t len, loff_t *offset);
static int charDriverEntry(void);
static void charDriverExit(void);
static ssize_t attrShowData(struct device*, struct device_attribute*, char*);
static ssize_t attrStoreData(struct device*, struct device_attribute*, const char*, size_t);

/* The following function is called when the file placed on the sysfs is accessed for read*/
static ssize_t attrShowData(struct device* pDev, struct device_attribute* attr, char* buffer)
{
	printk(KERN_INFO "MESG: The data has been accessed through the entry in sysfs\n");
	if (bufferPointer == 0)
	{
		printk(KERN_WARNING "Thre is no data to read from  buffer!");
		return -1;
	}
	strncpy(buffer, bufferMemory, bufferPointer);
	/* Note : Here we can directly use strncpy because we are already in kernel space and do not need to translate address*/
	return bufferPointer;
}

static ssize_t attrStoreData(struct device* pDev, struct device_attribute* attr, const char* buffer, size_t length)
{
	printk(KERN_INFO "Writing to attribute");
	bufferPointer = length;
	strncpy(bufferMemory, buffer, length);
	return length;
}

/* These macros converts the function in to instances dev_attr_<_name>*/
/* Defination of the macro is as follows : DEVICE_ATTR(_name, _mode, _show, _store) */
/* Note the actual implementation of the macro makes an entry in the struct device_attribute. This macro does that for us */
static DEVICE_ATTR(ShowData, S_IRWXU, attrShowData, attrStoreData);	// S_IRUSR gives read access to the user 

static struct file_operations fops = 
{
	.open = charDriverOpen,
	.release = charDriverClose,
	.read = charDriverRead,
	.write = charDriverWrite,
};


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
	printk(KERN_INFO "Now We will create the attribute entry in sysfs");
	/* the function used is device_create_file(struct device *, struct device_attribute*) */
	device_create_file(pmyCharDevice, &dev_attr_ShowData);		// The second argumnet is the structure created by the DEVICE_ATTR macro
	return 0;
}

static void charDriverExit()
{
	device_remove_file(pmyCharDevice, &dev_attr_ShowData);
	device_destroy(pmyCharClass, MKDEV(majorNumber,0));
	class_unregister(pmyCharClass);
	class_destroy(pmyCharClass);
	unregister_chrdev(majorNumber,DEVICE_NAME);
}

static int charDriverOpen(struct inode *inodep, struct file *filep)
{
	if ((filep->f_flags & O_ACCMODE) != O_RDWR)
	{
		printk(KERN_ALERT "WARNING : This driver can only be opened in both read and write mode");
		return -1;
	}
	printk(KERN_INFO "INFO : CHARATER DRIVER OPENED\n");
	bufferMemory = kmalloc(15,GFP_KERNEL);
	bufferPointer = 0;
	return 0;	
}

static int charDriverClose(struct inode *inodep, struct file *filep)
{
	kfree(bufferMemory);
	printk(KERN_INFO "INFO : CHARACTER DRIVER CLOSED\n");
	return 0;
}

static ssize_t charDriverWrite(struct file *filep, const char *buffer, size_t len, loff_t *offset)
{
	// Here we will only allow to write one byte of data 
	if (len > 15)
	{
		printk(KERN_WARNING "Attempted to write data larger than 15 byte!\n");
		return 0;
	}
	//bufferMemory[bufferPointer] = *buffer;
	copy_from_user(bufferMemory, buffer, len);
	bufferPointer += len;
	return len;
}

static ssize_t charDriverRead(struct file *filep, char *buffer, size_t len, loff_t *offset)
{
	if(len > 15 || len > bufferPointer)
	{
		printk(KERN_WARNING "Attempting to read more than buffer size ! Deny");
		return 0;
	}
	copy_to_user(buffer, bufferMemory, len);
//	buffer[0] = bufferMemory[0];
	bufferPointer -= len;
	return len;
}


module_init(charDriverEntry);
module_exit(charDriverExit);

