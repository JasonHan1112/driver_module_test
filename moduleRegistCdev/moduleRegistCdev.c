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

//定义分配内存空间函数头文件
#include<linux/slab.h>

//定义杂项设备名称
#define  DEVICE_NAME "myDCdev"

//定义连续的设备号的数量
//连续设备编号的总数. 注意, 如果 count 太大, 你要求的范围可能溢出到下一个主设备号
#define  DEVICE_MINOR_NUM 2

//定义缓存区大小
#define  REG_DEV_SIZE 1024

//定义初始设备号
#define  DEV_MAJOR 0
#define  DEV_MINOR 0

MODULE_LICENSE("Dual BSD/GPL");

/************定义主次设备号变量************/
static int numdev_major = DEV_MAJOR;
static int numdev_minor = DEV_MINOR;

/************定义主次设备号变量************/



/************定义次设备结构体************/
static struct reg_dev
{
	char* data;
	unsigned long size;
	struct cdev my_cdev; 
};
static struct reg_dev* my_devices;
/************定义次设备结构体************/


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

static int reg_init_cdev(struct reg_dev* dev,int index)
{
	int ret = 0;
	//初始化设备结构
	dev_t dev_num = MKDEV(numdev_major,index);
	cdev_init(&(dev->my_cdev),&hello_ops);
	dev->my_cdev.owner = THIS_MODULE;
	dev->my_cdev.ops = &hello_ops;
	//添加设备结构体
	ret = cdev_add(&(dev->my_cdev),dev_num,1);
	if(ret<0)
	{
		printk(KERN_EMERG "cdev_add failed!\n");
		return -1;
	}

	printk(KERN_EMERG "cdev_add succeed!\n");
	return 0;
}
static int hello_init(void)
{
	dev_t num_dev;//设备号变量，存放设备号
	int ret = 0;
	int device_count;//for循环遍历所有次设备
	printk(KERN_EMERG "Hello world! hello driver init\n");
	printk(KERN_EMERG "numdev_major = %d\n",numdev_major);
	printk(KERN_EMERG "numdev_minor = %d\n",numdev_minor);

	/**************注册设备号*****************/
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
		return -1;
	}

	/**************注册设备号*****************/
	my_devices = (struct reg_dev*)kmalloc(2*sizeof(struct reg_dev),GFP_KERNEL);
	if(my_devices == NULL)
	{
		unregister_chrdev_region(MKDEV(numdev_major,numdev_minor),DEVICE_MINOR_NUM);
		printk(KERN_EMERG "kmalloc failed!\n");
		return -1;
	}
	printk(KERN_EMERG "kmalloc successed!\n");

	memset(my_devices,0,DEVICE_MINOR_NUM*sizeof(struct reg_dev));
	//for循环遍历所有次设备
	for(device_count=0;device_count<DEVICE_MINOR_NUM;device_count++)
	{
		my_devices[device_count].data = (char*)kmalloc(REG_DEV_SIZE,GFP_KERNEL);
		memset(my_devices[device_count].data,0,REG_DEV_SIZE);
		my_devices[device_count].size = REG_DEV_SIZE; 
		reg_init_cdev(&my_devices[device_count],device_count);

	}
	
	return 0;
}
static void hello_exit(void)
{
	int i;
	printk(KERN_EMERG "Goodbye the cruel world! Goodbye hello driver \n");
	//删除字符设备
	for(i=0;i<DEVICE_MINOR_NUM;i++)
	{
		cdev_del(&(my_devices[i].my_cdev));
	}
	unregister_chrdev_region(MKDEV(numdev_major,numdev_minor),DEVICE_MINOR_NUM);
}
module_init(hello_init);
module_exit(hello_exit);
MODULE_AUTHOR("HAN XUEQING");
MODULE_DESCRIPTION("A simple hello world module");
