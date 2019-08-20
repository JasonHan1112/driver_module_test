#include <linux/module.h>//与module相关的信息
 
#include <linux/kernel.h>
#include <linux/init.h>      //与init相关的函数

#include <linux/platform_device.h>


#if 0


struct platform_device {
	const char	*name;
	int		id;
	bool		id_auto;
	struct device	dev;
	u32		num_resources;
	struct resource	*resource;

	const struct platform_device_id	*id_entry;
	char *driver_override; /* Driver name to force a match */

	/* MFD cell pointer */
	struct mfd_cell *mfd_cell;

	/* arch specific additions */
	struct pdev_archdata	archdata;
};


#endif


#define dg_printf printk
#define nd_printf printk




static struct platform_device my_platform_dev = {
   .name = "platform_test",
};

static int add_my_platform_dev(struct platform_device* pdev)
{
    int ret = 0;
    
    dg_printf("in fun: %s\n", __FUNCTION__);

    ret = platform_device_register(pdev);
	if (ret) 
    {
        nd_printf("can't register device: %s error code: %d\n",pdev->name, ret);
        return -1;
	}

    return 0;
};

static int kill_my_platform_dev(struct platform_device* pdev)
{
    dg_printf("in fun: %s\n", __FUNCTION__);
    
    platform_device_unregister(pdev);
    
    return 0;
}

static int __init platform_dev_init(void)
{
        printk(KERN_INFO "Hello kernel!\n");
        add_my_platform_dev(&my_platform_dev);
        return 0;
}
 
static void __exit platform_dev_exit(void)
{
        printk(KERN_INFO "by kernel!\n");
        kill_my_platform_dev(&my_platform_dev);
}
 
 
module_init(platform_dev_init);
module_exit(platform_dev_exit);
 
 
MODULE_LICENSE("GPL");
MODULE_AUTHOR("xueqing");
