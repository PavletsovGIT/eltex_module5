#include <linux/module.h>
#include <linux/configfs.h>
#include <linux/init.h>
#include <linux/tty.h>          /* For fg_console, MAX_NR_CONSOLES */
#include <linux/kd.h>           /* For KDSETLED */
#include <linux/vt.h>
#include <linux/console_struct.h>       /* For vc_cons */
#include <linux/vt_kern.h>
#include <linux/timer.h>
#include <linux/printk.h> 
#include <linux/kobject.h> 
#include <linux/sysfs.h> 
#include <linux/fs.h>
#include <linux/string.h>

MODULE_AUTHOR("Feodor Pavletsov");
MODULE_DESCRIPTION("Blinds on keyboard!:)");
MODULE_LICENSE("GPL");

// For sys
#define PERMS 0660

// For blinding lighs
#define BLINK_DELAY   5
#define ALL_LEDS_ON   0x07
#define RESTORE_LEDS  0xFF

// For sys
static struct kobject *sfs_kobject; /* kobject to read and write into sysfs */
static int fds; /* file descriptor */

// For blinding lighs
struct timer_list lab3_timer;
struct tty_driver *lab3_driver;
static int _kbledstatus = 0;
static int lig_mode = 0; /* mode of lights */

// For sys
static ssize_t sfs_show(struct kobject *kobj, struct kobj_attribute *attr,
                      char *buf);
static ssize_t sfs_store(struct kobject *kobj, struct kobj_attribute *attr,
                      const char *buf, size_t count);

static struct kobj_attribute sfs_attribute = __ATTR(fds, PERMS, sfs_show, sfs_store);

// For blinding lighs
/*
 * Function timer_func blinks the keyboard LEDs periodically by invoking
 * command KDSETLED of ioctl() on the keyboard driver. To learn more on virtual
 * terminal ioctl operations, please see file:
 *     /usr/src/linux/drivers/char/vt_ioctl.c, function vt_ioctl().
 *
 * The argument to KDSETLED is alternatively set to 7 (thus causing the led
 * mode to be set to LED_SHOW_IOCTL, and all the leds are lit) and to 0xFF
 * (any value above 7 switches back the led mode to LED_SHOW_FLAGS, thus
 * the LEDs reflect the actual keyboard status). To learn more on this,
 * please see file:
 *     /usr/src/linux/drivers/char/keyboard.c, function setledstate().
 *
 */
//static void timer_func(unsigned long ptr)
static void lab3_timer_func(struct timer_list *ptr);

static void lab3_restart_bleds(void);

static int __init lab3_init(void);
static void __exit lab3_exit(void);

module_init(lab3_init);
module_exit(lab3_exit);

static ssize_t sfs_show(struct kobject *kobj, struct kobj_attribute *attr,
                      char *buf)
{
    return sprintf(buf, "%d\n", fds);
}

static ssize_t sfs_store(struct kobject *kobj, struct kobj_attribute *attr,
                      const char *buf, size_t count)
{
    sscanf(buf, "%du", &fds);
    // Change mode of blinding lights
    lig_mode = count;
    lab3_restart_bleds();

    return count;
}

static void lab3_timer_func(struct timer_list *ptr)
{
        //int *pstatus = (int *)ptr;
        int *pstatus = &_kbledstatus;
        if (*pstatus == lig_mode) {
            *pstatus = RESTORE_LEDS;
        }
        else {
            *pstatus = lig_mode;
        }
        (lab3_driver->ops->ioctl) (vc_cons[fg_console].d->port.tty, KDSETLED,
                                   *pstatus);
        lab3_timer.expires = jiffies + BLINK_DELAY;
        add_timer(&lab3_timer);
}

static void lab3_restart_bleds(void)
{
    int i;
    printk(KERN_INFO "kbleds: loading\n");
    printk(KERN_INFO "kbleds: fgconsole is %x\n", fg_console);
    for (i = 0; i < MAX_NR_CONSOLES; i++) {
        if (!vc_cons[i].d)
            break;
        printk(KERN_INFO "poet_atkm: console[%i/%i] #%i, tty %lx\n", i,
               MAX_NR_CONSOLES, vc_cons[i].d->vc_num,
               (unsigned long)vc_cons[i].d->port.tty);
    }
    printk(KERN_INFO "kbleds: finished scanning consoles\n");
    lab3_driver = vc_cons[fg_console].d->port.tty->driver;
    printk(KERN_INFO "kbleds: tty driver magic %x\n", lab3_driver->magic);
    /*
     * Set up the LED blink timer the first time
     */
        
    //init_timer(&lab3_timer);
    timer_setup(&lab3_timer, lab3_timer_func, 0);
    //lab3_timer.function = lab3_timer_func;
    //lab3_timer.data = (unsigned long)&kbledstatus;
    lab3_timer.expires = jiffies + BLINK_DELAY;
    add_timer(&lab3_timer);
}

static int __init lab3_init (void)
{
    int error = 0;
 
    pr_debug("Module initialized successfully \n");
 
    sfs_kobject = kobject_create_and_add("lab3test",
                                                 kernel_kobj);

    if(!sfs_kobject)
        return -ENOMEM;
 
    error = sysfs_create_file(sfs_kobject, &sfs_attribute.attr);
    if (error) {
        pr_debug("failed to create the foo file in /sys/kernel/systest \n");
    }
 
    return error;
}
 
static void __exit lab3_exit (void)
{
    kobject_put(sfs_kobject);
    pr_debug ("Module un initialized successfully \n");
}