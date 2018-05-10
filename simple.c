/* This simple module is to see how simple insert and delete of driver module work,
   its make file and the required headers - what are they used for, when and how
   */

#include<linux/module.h>  /*Used for ins_module() and del_module() function*/
#include<linux/moduleparam.h>
#include<linux/init.h>


static int simple_init(void)
{
	/*This function is called when the insmod command is passed*/
	printk(KERN_NOTICE "The module is inserted\n");
	return 0;
}

static void simple_remove(void)
{
	/*This function is called when the rmmod command is called*/
	printk(KERN_NOTICE "The module is removed\n");
	return ;
}

module_init(simple_init);	// This is tells the kernel the entery point of the drive 
module_exit(simple_remove);	// this tells the kernel of the exit point on remmod


