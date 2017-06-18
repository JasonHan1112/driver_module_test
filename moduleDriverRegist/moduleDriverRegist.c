#include <linux/init.h>
#include <linux/module.h>
/*包含注册驱动、设备的结构体，注册卸载函数*/
#include <linux/platform_device.h>



//定义设备名称，要和驱动名和注册的设备名称匹配
#define  DRIVER_NAME "hello_ctl"
MODULE_LICENSE("Dual BSD/GPL");

//struct platform_device *
static int hello_probe(struct platform_device* pdev)
{
	printk(DRIVER_NAME  "hello is probing\n");
	return 0;
}
static int hello_remove(struct platform_device* pdev)
{
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

static int hello_init(void)
{
	int DriverState;
	printk(KERN_EMERG "Hello world! hello driver init\n");

	DriverState = platform_driver_register(&hello_driver);
	printk(DRIVER_NAME "DriverState = %d\n",DriverState);
	return 0;
}
static void hello_exit(void)
{
	printk(KERN_EMERG "Goodbye the cruel world! Goodbye hello driver \n");
}
module_init(hello_init);
module_exit(hello_exit);
MODULE_AUTHOR("HAN XUEQING");
MODULE_DESCRIPTION("A simple hello world module");
//MODULE_ALLIAS("A simplest module");
