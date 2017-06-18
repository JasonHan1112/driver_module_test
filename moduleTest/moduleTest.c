#include <linux/init.h>
#include <linux/module.h>
MODULE_LICENSE("Dual BSD/GPL");
static int hello_init(void)
{
	printk(KERN_EMERG "Hello world! \n");
	return 0;
}
static void hello_exit(void)
{
	printk(KERN_EMERG "Goodbye the cruel world! \n");
}
module_init(hello_init);
module_exit(hello_exit);
MODULE_AUTHOR("HAN XUEQING");
MODULE_DESCRIPTION("A simple hello world module");
//MODULE_ALLIAS("A simplest module");
