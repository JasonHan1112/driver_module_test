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
#if 0

struct platform_driver {
	int (*probe)(struct platform_device *);
	int (*remove)(struct platform_device *);
	void (*shutdown)(struct platform_device *);
	int (*suspend)(struct platform_device *, pm_message_t state);
	int (*resume)(struct platform_device *);
	struct device_driver driver;
	const struct platform_device_id *id_table;
	bool prevent_deferred_probe;
};


#endif

#define dg_printf printk
#define nd_printf printk


#define DRVNAME "platform_test"
int my_platform_probe(struct platform_device * pdev)
{
    dg_printf("in fun: %s\n", __FUNCTION__);
    dg_printf("platform_dev name: %s\n", pdev->name); 
    return 0;
}
int my_platform_remove(struct platform_device * pdev)
{
    dg_printf("in fun: %s\n", __FUNCTION__);
    dg_printf("platform_dev name: %s\n", pdev->name); 
    
    return 0;
}
static struct platform_driver my_platform_drv = {
	.probe = my_platform_probe,
    .remove = my_platform_remove,   
	.driver	= {
		.name = DRVNAME,
	},
};

static int add_my_platform_drv(struct platform_driver* pdrv)
{
    int ret = 0;
    
    dg_printf("in fun: %s\n", __FUNCTION__);

    ret = platform_driver_register(pdrv);
	if (ret) 
    {
        nd_printf("can't register driver: %s error code: %d\n", pdrv->driver.name, ret);
        return -1;
	}

    return 0;
};

static int kill_my_platform_drv(struct platform_driver* pdrv)
{
    dg_printf("in fun: %s\n", __FUNCTION__);
    
    platform_driver_unregister(pdrv);
    
    return 0;
}

static int __init platform_test_init(void)
{
        printk(KERN_INFO "Hello kernel!\n");
        add_my_platform_drv(&my_platform_drv);
        return 0;
}
 
static void __exit platform_test_exit(void)
{
        printk(KERN_INFO "by kernel!\n");
        kill_my_platform_drv(&my_platform_drv);
}
 
 
module_init(platform_test_init);
module_exit(platform_test_exit);
 
 
MODULE_LICENSE("GPL");
MODULE_AUTHOR("xueqing");
