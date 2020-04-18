ifneq ($(KERNELRELEASE),) #test if run in the kernel Makefile 
  
obj-m += virtio_rpmsg_bus.o  
obj-m += rpmsg_char.o  
obj-m += rpmsg_core.o  

else  
#KDIR := /home/xueqing/Documents/kernel/linux-4.19 
KDIR := /usr/src/linux-headers-5.3.0-46-generic/
all:  
	echo "compiling..."
#make -C $(KDIR) M=$(PWD) modules ARCH=arm CROSS_COMPILE=arm-eabi-  
	make -C $(KDIR) M=$(PWD) modules  
clean:  
	rm -f *.ko *.o *.mod.o *.mod.c *.symvers *.order
  
endif 
