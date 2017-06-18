#include <linux/init.h>
#include <linux/module.h>

/*包含注册驱动、设备的结构体，注册卸载函数*/
#include <linux/platform_device.h>

/*注册杂项设备头文件*/
#include <linux/miscdevice.h>

/*驱动注册的头文件，包含驱动的结构体*/
#include <linux/fs.h>

//定义模块参数头文件
#include<linux/moduleparam.h>

//MKDEV转换设备号数据类型的宏定义
#include<linux/kdev_t.h>

//定义字符设备结构体
#include<linux/cdev.h>

//定义杂项设备名称
#define  DEVICE_NAME "myDCdev"

//定义连续的设备号的数量
//连续设备编号的总数. 注意, 如果 count 太大, 你要求的范围可能溢出到下一个主设备号
#define  DEVICE_MINOR_NUM 2

//定义初始设备号
#define  DEV_MAJOR 0
#define  DEV_MINOR 0

MODULE_LICENSE("Dual BSD/GPL");

/************定义主次设备号变量************/
static int numdev_major = DEV_MAJOR;
static int numdev_minor = DEV_MINOR;

/************定义主次设备号变量************/


/*************模块参数传递***************/
module_param(numdev_major,int,S_IRUSR);
module_param(numdev_minor,int,S_IRUSR);



static long hello_unlocked_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	printk(KERN_EMERG "hello_misc is unlocked ioctling\n");
	printk(KERN_EMERG "cmd is %d,arg is %lu\n",cmd,arg);
	printk(KERN_EMERG "numdev_major = %d\n",numdev_major);
	printk(KERN_EMERG "numdev_minor = %d\n",numdev_minor);
	
	return 0;
}

static int hello_release(struct inode *inode, struct file *file)
{
	printk(KERN_EMERG "hello_misc is releaseing!\n");
	return 0;
}

static int hello_open(struct inode *inode, struct file *file)
{
	printk(KERN_EMERG "hello_misc is opening!\n");
	return 0;
}

static struct file_operations hello_ops =
{
	.owner = THIS_MODULE,
	.open = hello_open,
	.release = hello_release,
	.unlocked_ioctl = hello_unlocked_ioctl

};


static int hello_init(void)
{
	dev_t num_dev;//设备号变量，存放设备号
	int ret = 0;
	printk(KERN_EMERG "Hello world! hello driver init\n");
	printk(KERN_EMERG "numdev_major = %d\n",numdev_major);
	printk(KERN_EMERG "numdev_minor = %d\n",numdev_minor);

	/**************注册设备*****************/
	if(numdev_major)//如果在insmod中不指定传递的参数那么该值为0,执行else表示动态生成主设备号
	{
		num_dev = MKDEV(numdev_major,numdev_minor);
		ret = register_chrdev_region(num_dev,DEVICE_MINOR_NUM,DEVICE_NAME);
	}
	else
	{
		ret = alloc_chrdev_region(&num_dev,numdev_minor,DEVICE_MINOR_NUM,DEVICE_NAME);//次设备号一般为零
		numdev_major = MAJOR(num_dev);
		printk(KERN_EMERG "alloc_chrdev_region: numdev_major = %d\n",numdev_major);
	}
	if(ret < 0)
	{
		printk(KERN_EMERG "register_chrdev_region failed!\n");
	}

	/**************注册设备*****************/
	
	return 0;
}
static void hello_exit(void)
{
	printk(KERN_EMERG "Goodbye the cruel world! Goodbye hello driver \n");
	unregister_chrdev_region(MKDEV(numdev_major,numdev_minor),DEVICE_MINOR_NUM);
}
module_init(hello_init);
module_exit(hello_exit);
MODULE_AUTHOR("HAN XUEQING");
MODULE_DESCRIPTION("A simple hello world module");
