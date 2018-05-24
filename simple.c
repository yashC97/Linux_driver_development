/* This simple module is to see how simple insert and delete of driver module work,
   its make file and the required headers - what are they used for, when and how
   */

#include<linux/module.h>  /*Used for ins_module() and del_module() function*/
#include<linux/moduleparam.h>
#include<linux/init.h>

MODULE_LICENSE("GPL");              ///< The license type -- this affects runtime behavior
MODULE_AUTHOR("Yash Bhatt");      ///< The author -- visible when you use modinfo
MODULE_DESCRIPTION("A simple Linux driver");  ///< The description -- see modinfo
MODULE_VERSION("0.1");              ///< The version of the module

static char *name = "ModuleSimple";        ///< An example LKM argument -- default value is "world"
module_param(name, charp, S_IRUGO); ///< Param desc. charp = char ptr, S_IRUGO can be read/not changed
MODULE_PARM_DESC(name, "The name to display in /var/log/kern.log");  ///< parameter description

static int __init simple_init(void)
{
	/*This function is called when the insmod command is passed*/
	printk(KERN_NOTICE "The module is inserted\n");
	return 0;
}

static void __exit simple_remove(void)
{
	/*This function is called when the rmmod command is called*/
	printk(KERN_NOTICE "The module is removed\n");
	return ;
}

module_init(simple_init);	// This is tells the kernel the entery point of the drive 
module_exit(simple_remove);	// this tells the kernel of the exit point on remmod


