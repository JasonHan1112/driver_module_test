#include <linux/init.h>
#include <linux/module.h>
//定义字符设备结构体
#include <linux/cdev.h>
//MKDEV转换设备号数据类型的宏定义
#include<linux/kdev_t.h>
//定义分配内存空间函数头文件
#include<linux/slab.h>
//包含函数device_create结构体class等头文件
#include<linux/device.h>
/*驱动注册的头文件，包含驱动的结构体*/
#include <linux/fs.h>
/*包含copy_from_user*/
#include<asm/uaccess.h>
/*等待队列相关头文件*/
#include<linux/wait.h>
#include <linux/sched.h>



#define DEBUG(fmt,arg...)  printk(KERN_EMERG ":Function %s, Line %d:[DEBUG]"  fmt"\n",__FUNCTION__,__LINE__,##arg)
MODULE_LICENSE("Dual BSD/GPL");
/*新建设备如果权限不修改会出现permission denied的提示*/
/************定义主次设备号变量************/
#define DEV_MAJOR             0
#define DEV_MINOR             0
#define DEV_BUFF_MAX          128
#define DEVICE_MINOR_NUM      1
#define DEVICE_NAME           "HELLO_MODULE"


/*自定义设备结构体*/
typedef struct myDev
{
	/*必须有的成员*/
	struct cdev my_cdev;
	struct class *myDeviceClass;
	struct file_operations* fileOperation;
	
	/*自定义的成员*/
	int   iOpenCount;/*驱动设备节点打开计数*/
	
	char* pOutputData;/*输出驱动的缓冲区*/
	
	char* pInputData;/*输入驱动的缓冲区*/
	
	wait_queue_head_t my_queue;/*读阻塞*/
	
	int enable_read;
	
	
}MY_DEV;

/*定义自定义设备结构体*/
static MY_DEV  *myDev;

/*驱动函数定义*/
static ssize_t hello_write (struct file *file, const char __user *user, size_t size, loff_t *loff)
{
	int i = 0;
	DEBUG("hello_write");
	DEBUG("file->private_data = %lu",file->private_data);/*与open中的file->private_data相同*/

	if (size > DEV_BUFF_MAX)
	{
		DEBUG("size is too large!");
		return -1;
	}
	
	/*从用户区拷贝数据*/
	if(size > 128)
	{
		DEBUG("size too large!");
		return -1;
	}
	i = copy_from_user(myDev->pInputData,user,size);
	DEBUG("failed num = %d",i);
	
	/*唤醒读阻塞*/
	wake_up_interruptible(&(myDev->my_queue));
	myDev->enable_read = 1;
	
	
	for(i=0; i<size; i++)
	{
		DEBUG("user[%d] = %d",i,user[i]);
	}
	for(i=0; i<size; i++)
	{
		DEBUG("myDev->pInputData[%d] = %d",i,myDev->pInputData[i]);
	}
	
	return 0;
}

static ssize_t hello_read (struct file *file, char __user *user, size_t size, loff_t *loff)
{
	int i;
	DEBUG("hello_read");
	DEBUG("file->private_data = %lu",file->private_data);/*与open中的file->private_data相同*/
	
	if (size > DEV_BUFF_MAX)
	{
		DEBUG("size is too large!");
		return -1;
	}

/*测试阻塞读*/
	DEBUG("before wait_event_interruptible_timeout");
	wait_event_interruptible_timeout(myDev->my_queue, myDev->enable_read, 100000);
	myDev->enable_read = 0;
	DEBUG("after wait_event_interruptible_timeout");
	
	
/*test 处理将数值+2*/	
	for(i=0; i<size; i++)
	{
		myDev->pOutputData[i] =myDev->pInputData[i] + 2;
	}
	
/*将数据传到用户空间*/	
	i = copy_to_user(user,myDev->pOutputData,size);
	DEBUG("failed num = %d\n",i);

/*打印内核空间*/	
	for(i=0; i<size; i++)
	{
		DEBUG("user[%d] = %d",i,user[i]);
	}
	return 0;
}

static long hello_unlocked_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	printk(KERN_EMERG "hello_misc is unlocked ioctling\n");
	printk(KERN_EMERG "cmd is %d,arg is %lu\n",cmd,arg);
	printk(KERN_EMERG "numdev_major = %d\n",MAJOR((myDev->my_cdev).dev));
	printk(KERN_EMERG "numdev_minor = %d\n",MINOR((myDev->my_cdev).dev));
	
	return 0;
}

static int hello_release(struct inode *inode, struct file *file)
{
	printk(KERN_EMERG "hello_misc is releaseing!\n");
	DEBUG("file->private_data = %lu",file->private_data);
	myDev->iOpenCount--;
	DEBUG("myDev->iOpenCount = %d",myDev->iOpenCount);
	return 0;
}

static int hello_open(struct inode *inode, struct file *file)
{
	printk(KERN_EMERG "hello_misc is opening!\n");
	file->private_data = myDev;
	DEBUG("file->private_data = %lu",file->private_data);
	
	/*计数自增*/
	myDev->iOpenCount++;
	DEBUG("myDev->iOpenCount = %d",myDev->iOpenCount);
	return 0;
}

/*驱动操作结构体*/
static struct file_operations hello_ops =
{
	.owner = THIS_MODULE,
	.open = hello_open,
	.release = hello_release,
	.unlocked_ioctl = hello_unlocked_ioctl,
	.read = hello_read,
	.write = hello_write

};



/*初始化自定义设备结构体*/
static void    InitMyDev(void)
{
	/*开辟空间*/
	myDev = (MY_DEV*)kmalloc(sizeof(MY_DEV),GFP_KERNEL);
	if(myDev == NULL)
	{
		DEBUG("kmalloc failed!");
		return ;
	}
	memset(myDev,0,sizeof(MY_DEV));
	DEBUG("initial myDev!");
	myDev->pOutputData = (char*)kmalloc(DEV_BUFF_MAX,GFP_KERNEL);
	if(myDev->pOutputData == NULL)
	{
		DEBUG("kmalloc failed!");
		return ;
	}
	memset(myDev->pOutputData,0,DEV_BUFF_MAX);
	DEBUG("initial outputBuff!");
	
	myDev->pInputData = (char*)kmalloc(DEV_BUFF_MAX,GFP_KERNEL);
	if(myDev->pInputData == NULL)
	{
		DEBUG("kmalloc failed!");
		return ;
	}
	memset(myDev->pInputData,0,DEV_BUFF_MAX);
	DEBUG("initial outputBuff!");
	
	/*初始化驱动打开计数*/
	myDev->iOpenCount = 0;
	
	/*初始化驱动函数*/
	myDev->fileOperation = &hello_ops;
	
	/*初始化等待队列*/
	init_waitqueue_head(&(myDev->my_queue));
	myDev->enable_read = 0;/*初始化读阻塞*/
	DEBUG("initial init_wait_queue!");
	
	return ;
}

/*模块加载入口*/
static int hello_init(void)
{
	int ret = 0;
	dev_t numDev = 0;
	printk(KERN_EMERG "Hello world! \n");
	/*初始化自定义设备结构体*/
	InitMyDev();
	
	ret = alloc_chrdev_region(&numDev,0,DEVICE_MINOR_NUM,DEVICE_NAME);//次设备号一般为零
	if(ret < 0)
	{
		DEBUG("alloc_chrdev_region failed!");	
	}	
	DEBUG("MAJOR(numDev) = %d",MAJOR(numDev));
	DEBUG("numDev = %d",numDev);

	/*需要注意class_create和后边的device_create完成设备节点的创建*/
	myDev->myDeviceClass = class_create(THIS_MODULE,DEVICE_NAME);
		
	/*初始化设备结构*/
	numDev = MKDEV(MAJOR(numDev),0);
	DEBUG("numDev = %d",numDev);
	
	cdev_init(&(myDev->my_cdev),myDev->fileOperation);//
	myDev->my_cdev.owner = THIS_MODULE;
	//myDev->my_cdev.ops = myDev->fileOperation;//用了cdev_init可以不用但是用cdev_alloc需要用
	/*添加设备*/
	ret = cdev_add(&(myDev->my_cdev),numDev,1);
	if(ret<0)
	{
		DEBUG("cdev_add failed!");
		return -1;
	}
	
	
	device_create(myDev->myDeviceClass,NULL,numDev,NULL,DEVICE_NAME"%d",0);
	DEBUG("device_create succeed!");
	return 0;
}
static void hello_exit(void)
{
	/*移除设备*/
	cdev_del(&(myDev->my_cdev));
	
	/*删除设备节点*/	
	device_destroy(myDev->myDeviceClass,myDev->my_cdev.dev);
	
	/*释放设备类*/
	class_destroy(myDev->myDeviceClass);
		
	/*释放设备号*/
	unregister_chrdev_region((myDev->my_cdev).dev,DEVICE_MINOR_NUM);
	
	/*释放设备申请的空间，只释放自己kmalloc的空间，其他的如果强制free会有问题*/
	kfree(myDev->pOutputData);
	kfree(myDev->pInputData);
	kfree(myDev);
	
	myDev->pOutputData = NULL;
	myDev->pInputData = NULL;
	myDev = NULL;
	printk(KERN_EMERG "Goodbye the cruel world! \n");
}
module_init(hello_init);
module_exit(hello_exit);
MODULE_AUTHOR("HAN XUEQING");
MODULE_DESCRIPTION("A simple hello world module");
//MODULE_ALLIAS("A simplest module");
