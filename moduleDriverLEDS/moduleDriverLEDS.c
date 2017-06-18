#include <linux/init.h>
#include <linux/module.h>

/*包含注册驱动、设备的结构体，注册卸载函数*/
#include <linux/platform_device.h>

/*注册杂项设备头文件*/
#include <linux/miscdevice.h>

/*驱动注册的头文件，包含驱动的结构体*/
#include <linux/fs.h>

/*定义linux中GPIO的头文件*/
#include<linux/gpio.h>

/*定义三星平台gpio相关的头文件*/
#include<plat/gpio-cfg.h>

/*三星平台EXYNOS系列平台，GPIO配置参数宏定义头文件*/
#include<mach/gpio.h>

/*三星平台4412，GPIO宏定义*/
#include<mach/gpio-exynos4.h>



//定义设备名称，要和驱动名和注册的设备名称匹配
#define  DRIVER_NAME "hello_ctl"

//定义杂项设备名称
#define  MISC_DEVICE_NAME "hello_misc_device"
MODULE_LICENSE("Dual BSD/GPL");

//int misc_register(struct miscdevice * misc);
//int misc_deregister(struct miscdevice *misc);
//杂项设备操作函数实现
static long hello_misc_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	printk(KERN_EMERG "hello_misc is unlocked ioctling\n");
	printk(KERN_EMERG "cmd is %d,arg is %lu\n",cmd,arg);
	
	if(cmd > 1)
	{
		 printk(KERN_EMERG "cmd must be 0 or 1\n");
	}
	if(arg > 1)
	{
	 printk(KERN_EMERG "arg is only 1\n");	
	}
	gpio_set_value(EXYNOS4_GPL2(0),cmd);
	return 0;
}


//杂项设备操作函数实现
static int hello_misc_release(struct inode *inode, struct file *file)
{
	printk(KERN_EMERG "hello_misc is releaseing!\n");
	return 0;
}
//杂项设备操作函数实现
static int hello_misc_open(struct inode *inode, struct file *file)
{
	printk(KERN_EMERG "hello_misc is opening!\n");
	return 0;
}
//杂项设备操作函数结构体
static struct file_operations hello_misc_ops =
{
	.owner = THIS_MODULE,
	.open = hello_misc_open,
	.release = hello_misc_release,
	.unlocked_ioctl = hello_misc_ioctl

};
//杂项设备结构体
static struct miscdevice hello_misc_device =
{
	.minor = MISC_DYNAMIC_MINOR,
	.name = MISC_DEVICE_NAME,
	.fops = &hello_misc_ops
};
//平台设备操作函数
static int hello_probe(struct platform_device* pdev)
{
	int ret = 0;
	printk(DRIVER_NAME  "hello is probing\n");
	ret = gpio_request(EXYNOS4_GPL2(0),"LEDS");
	if(ret < 0)
	{
		printk(KERN_EMERG "gpio_request EXYNOS4_GPL2(0) failed!\n");
		return ret;
	}
	s3c_gpio_cfgpin(EXYNOS4_GPL2(0),S3C_GPIO_OUTPUT);
	//UBOOT中默认点亮，现在拉低
	gpio_set_value(EXYNOS4_GPL2(0),0);

	misc_register(&hello_misc_device);
	
	
	printk(KERN_EMERG "hello misc device is registed\n");
	return 0;
}

//平台设备操作函数
static int hello_remove(struct platform_device* pdev)
{
	printk(DRIVER_NAME  "hello is removing\n");
	misc_deregister(&hello_misc_device);
	printk(KERN_EMERG "hello misc device is deregisted\n");
	return 0;
}

//平台设备操作函数
static void hello_shutdown(struct platform_device* pdev)
{
	return ;
}

//平台设备操作函数
static int hello_suspend(struct platform_device* pdev,pm_message_t pmt)
{
	return 0;
}

//平台设备操作函数
static int hello_resume(struct platform_device* pdev)
{
	return 0;
}

//平台设备驱动结构体
static struct platform_driver hello_driver=
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

//insmod加载模块是第一个执行的函数
static int hello_init(void)
{
	int DriverState;
	printk(KERN_EMERG "Hello world! hello driver init\n");

	DriverState = platform_driver_register(&hello_driver);
	printk(DRIVER_NAME "DriverState = %d\n",DriverState);
	return 0;
}
//rmmod移除模块执行的函数
static void hello_exit(void)
{
	printk(KERN_EMERG "Goodbye the cruel world! Goodbye hello driver \n");
	platform_driver_unregister(&hello_driver);
}
module_init(hello_init);
module_exit(hello_exit);
MODULE_AUTHOR("HAN XUEQING");
MODULE_DESCRIPTION("A simple hello world module");
