obj-m := moduleDriverRegist.o
modules-objs:= moduleDriverRegist.o
KDIR:= /home/xueqing/Documents/Projects/kernelCode/iTop4412_Kernel_3.0
PWD:= $(shell pwd)
all:
	make -C $(KDIR) M=$(PWD) modules
clean:
	rm -rf *.o
