#include <linux/init.h>
#include <linux/module.h>
/*包含注册驱动、设备的结构体，注册卸载函数*/
#include <linux/platform_device.h>
/*注册杂项设备头文件*/
#include <linux/miscdevice.h>
/*驱动注册的头文件，包含驱动的结构体*/
#include <linux/fs.h>
//定义设备名称，要和驱动名和注册的设备名称匹配
#define  DRIVER_NAME "hello_ctl"

//定义杂项设备名称
#define  MISC_DEVICE_NAME "hello_only_misc_device"
MODULE_LICENSE("Dual BSD/GPL");

//int misc_register(struct miscdevice * misc);
//int misc_deregister(struct miscdevice *misc);
static long hello_misc_unlocked_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	printk(KERN_EMERG "hello_misc is unlocked ioctling\n");
	printk(KERN_EMERG "cmd is %d,arg is %d\n",cmd,arg);
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

/*
static int hello_probe(struct platform_device* pdev)
{
	printk(DRIVER_NAME  "hello is probing\n");
	misc_register(&hello_misc_device);
	printk(KERN_EMERG "hello misc device is registed\n");
	return 0;
}



static int hello_remove(struct platform_device* pdev)
{
	printk(DRIVER_NAME  "hello is removing\n");
	misc_deregister(&hello_misc_device);
	printk(KERN_EMERG "hello misc device is deregisted\n");
	return 0;
}


static void hello_shutdown(struct platform_device* pdev)
{
	return ;
}


static int hello_suspend(struct platform_device* pdev)
{
	return 0;
}

static int hello_resume(struct platform_device* pdev)
{
	return 0;
}
struct platform_driver hello_driver=
{
	.probe = hello_probe,
	.remove = hello_remove,
	.shutdown = hello_shutdown,
	.suspend = hello_suspend,
	.resume = hello_resume,
	.driver = 
	{
		.name = DRIVER_NAME,
		.owner = THIS_MODULE,
	}
};
*/
static int hello_init(void)
{
	int DriverState = 0;
	printk(KERN_EMERG "Hello world! hello driver init\n");
	misc_register(&hello_misc_device);
	//DriverState = platform_driver_register(&hello_driver);
	printk(DRIVER_NAME "DriverState = %d\n",DriverState);
	return 0;
}
static void hello_exit(void)
{
	printk(KERN_EMERG "Goodbye the cruel world! Goodbye hello driver \n");
	misc_deregister(&hello_misc_device);	
	//platform_driver_unregister(&hello_driver);
}
module_init(hello_init);
module_exit(hello_exit);
MODULE_AUTHOR("HAN XUEQING");
MODULE_DESCRIPTION("A simple hello world module");
//MODULE_ALLIAS("A simplest module");
