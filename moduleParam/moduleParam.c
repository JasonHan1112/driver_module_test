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

//定义设备名称，要和驱动名和注册的设备名称匹配
#define  DRIVER_NAME "hello_ctl"

//定义杂项设备名称
#define  MISC_DEVICE_NAME "hello_only_misc_device"



MODULE_LICENSE("Dual BSD/GPL");


/*************模块参数传递***************/
static int module_arg1,module_arg2;//整型参数1，参数2
static int int_array[50];//数组参数
static int array_num;//数组参数个数

module_param(module_arg1,int,S_IRUSR);
module_param(module_arg2,int,S_IRUSR);
module_param_array(int_array,int,&array_num,S_IRUSR);
/*************模块参数传递***************/




static long hello_misc_unlocked_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	int i;
	printk(KERN_EMERG "hello_misc is unlocked ioctling\n");
	printk(KERN_EMERG "cmd is %d,arg is %lu\n",cmd,arg);
	printk(KERN_EMERG "module_arg1 = %d\n",module_arg1);
	printk(KERN_EMERG "module_arg2 = %d\n",module_arg2);
	for(i=0; i<array_num; i++)
	{
		printk(KERN_EMERG "int_array[%d] = %d\n",i,int_array[i]);
	}
	
	return 0;
}

static int hello_misc_release(struct inode *inode, struct file *file)
{
	printk(KERN_EMERG "hello_misc is releaseing!\n");
	return 0;
}

static int hello_misc_open(struct inode *inode, struct file *file)
{
	printk(KERN_EMERG "hello_misc is opening!\n");
	return 0;
}

static struct file_operations hello_misc_ops =
{
	.owner = THIS_MODULE,
	.open = hello_misc_open,
	.release = hello_misc_release,
	.unlocked_ioctl = hello_misc_unlocked_ioctl

};

static struct miscdevice hello_misc_device =
{
	.minor = MISC_DYNAMIC_MINOR,
	.name = MISC_DEVICE_NAME,
	.fops = &hello_misc_ops
};

static int hello_init(void)
{
	int DriverState = 0;
	int i;
	printk(KERN_EMERG "Hello world! hello driver init\n");
	misc_register(&hello_misc_device);
	printk(DRIVER_NAME "DriverState = %d\n",DriverState);
	printk(KERN_EMERG "module_arg1 = %d\n",module_arg1);
	printk(KERN_EMERG "module_arg2 = %d\n",module_arg2);
	for(i=0; i<array_num; i++)
	{
		printk(KERN_EMERG "int_array[%d] = %d\n",i,int_array[i]);
	}
	return 0;
}
static void hello_exit(void)
{
	printk(KERN_EMERG "Goodbye the cruel world! Goodbye hello driver \n");
	misc_deregister(&hello_misc_device);	
}
module_init(hello_init);
module_exit(hello_exit);
MODULE_AUTHOR("HAN XUEQING");
MODULE_DESCRIPTION("A simple hello world module");
