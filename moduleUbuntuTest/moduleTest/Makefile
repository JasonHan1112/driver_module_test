#call from kernel build system
ifneq ($(KERNELRELEASE),)

#最终生成的.ko文件所依赖的最终的.o文件
obj-m := moduleUbuntuHello.o
#最终的.o文件依赖的所有的.o文件
moduleUbuntuHello-objs:= moduleTest.o

else

#源码目录，其中需要有Makefile
KDIR:= /usr/src/linux-headers-4.2.0-27-generic 
#模块源码所在的目录
PWD:= $(shell pwd)
all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules
clean:
	rm -rf *.o *.ko *.order *.symvers *.mod.c

endif
