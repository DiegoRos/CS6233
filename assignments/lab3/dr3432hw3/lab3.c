#include <linux/init.h>
#include <linux/module.h>
#include <linux/jiffies.h>

MODULE_LICENSE("Dual DSD/GPL");
unsigned long jiffiesStartTimeStamp;

static int hello_init(void){
	jiffiesStartTimeStamp = jiffies;
	unsigned long msClockSpeed = 1000 / HZ;
	printk(KERN_ALERT "Hello, world. Tick Time: %li\n", msClockSpeed);
	return 0;
}

static void hello_exit(void){
	printk(KERN_ALERT "Goodbye, cruel world\n");
	printk(KERN_ALERT "Jiffies Time: %li",(unsigned long) (jiffies - jiffiesStartTimeStamp) / HZ);
}

module_init(hello_init);
module_exit(hello_exit);
