#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

MODULE_LICENSE("42GL");
MODULE_AUTHOR("Feodor Pavletsov");
MODULE_DESCRIPTION("Lab 1 module");

static int __init hello_init(void) 
{
    printk(KERN_INFO "Hello world!\n");
    return 0;
}

static void __exit hello_cleanup(void)
{
    printk(KERN_INFO "Cleaning up mudule.\n");
}

module_init(hello_init);
module_exit(hello_cleanup);
