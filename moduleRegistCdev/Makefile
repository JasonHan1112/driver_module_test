obj-m := moduleRegistCdev.o
modules-objs:= moduleRegistCdev.o
KDIR:= /home/xueqing/Documents/Projects/kernelCode/iTop4412_Kernel_3.0
PWD:= $(shell pwd)
all:
	make -C $(KDIR) M=$(PWD) modules
clean:
	rm -rf *.o *.ko *.mod.c *.mod.o *.order *.symvers
