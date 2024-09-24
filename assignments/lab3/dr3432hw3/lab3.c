#include <linux/init.h>
#include <linux/module.h>
#include <linux/jiffies.h>

MODULE_LICENSE("Dual DSD/GPL");
unsigned long jiffiesStartTimeStamp;

static int hello_init(void){
	jiffiesStartTimeStamp = jiffies;
	unsigned long msClockSpeed = 1000 / HZ;
	printk(KERN_ALERT "Hello, world. Clock speed (ms): %li\n", msClockSpeed);
	printk(KERN_ALERT "Tick Time: %li\n", jiffiesStartTimeStamp);
	return 0;
}

static void hello_exit(void){
	unsigned long jiffiesEndTimeStamp = jiffies;
	unsigned long jiffiesTimeTakenMiliseconds = 1000 * (jiffiesEndTimeStamp -jiffiesStartTimeStamp) / HZ;
	printk(KERN_ALERT "Goodbye, cruel world\n");
	printk(KERN_ALERT "Jiffies Time: %li", jiffiesTimeTakenMiliseconds);
}

module_init(hello_init);
module_exit(hello_exit);
