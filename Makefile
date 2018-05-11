#the make file for linux kernel module is a lot different than that we make for normal programs 

obj-m+=myCharDriver.o #obj-m defines lodable module goal

#obj-y - used to make built-in object goal

all:
	#make -C /usr/src/linux-headers-4.13.0-38/ M=$(PWD) modules
	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) modules

#we here use the make provided with the kernel and out modules is just appended to it . -C changes directory to that of first argument; M-where the project is and modueles is the name of the taret in the kernel make file 

clean:
	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) clean

