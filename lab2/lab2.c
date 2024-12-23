#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/sched.h>
#include <linux/uaccess.h>
#include <linux/slab.h>

#define MSGSIZE 10

int len,temp;
char *msg;

MODULE_AUTHOR("Feodor Pavletsov");

MODULE_DESCRIPTION("GPL");

MODULE_LICENSE("lab2 with 42 Universe License");

MODULE_VERSION("v0.1");
 
ssize_t read_proc(struct file *filp, char *buf, size_t count, loff_t *offp );
ssize_t write_proc(struct file *filp, const char *buf, size_t count, loff_t *offp);
void create_new_proc_entry(void);

ssize_t read_proc(struct file *filp, char *buf, size_t count, loff_t *offp ) {
    if(count > temp) {
        count = temp;
    }
    temp = temp - count;
    if (copy_to_user(buf, msg, count)) {
        printk(KERN_WARNING "Не удалось скопировать данные в пользовательскую память\n");
    }
    if(count == 0)
        temp = len;
    return count;
}
 
ssize_t write_proc(struct file *filp, const char *buf, size_t count, loff_t *offp) {
    if (copy_from_user(msg, buf, count)) {
        printk(KERN_WARNING "Не удалось скопировать данные из пользовательской памяти\n");
    }
    len = count;
    temp = len;
    return count;
}
 
static const struct proc_ops proc_fops = {
    .proc_read = read_proc,
    .proc_write = write_proc
};
 
void create_new_proc_entry(void) { //use of void for no arguments is compulsory now
    proc_create("proc", 0, NULL, &proc_fops);
    msg = kmalloc(MSGSIZE * sizeof(char), GFP_KERNEL);
}

static int __init lab2_init(void)
{
	printk(KERN_INFO "The module lab2 has been successfully initialized!\n");
	create_new_proc_entry();
	return 0;
}

static void __exit lab2_exit(void)
{   
    remove_proc_entry("proc", NULL);
	kfree(msg);
	printk(KERN_INFO "The module lab2 has been successfully cleared!\n");
}

module_init(lab2_init);
module_exit(lab2_exit);