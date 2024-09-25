#include <linux/init.h>
#include <linux/module.h>
#include <linux/jiffies.h>
#include <linux/timekeeping.h>
#include <linux/ktime.h>

MODULE_LICENSE("Dual DSD/GPL");
unsigned long jiffiesStartTimeStamp;
s64 kTimeStartTimeStamp;

static int hello_init(void){
	jiffiesStartTimeStamp = jiffies;
	kTimeStartTimeStamp = ktime_to_ms(ktime_get_boottime());
	unsigned long msClockSpeed = 1000 / HZ;
	printk(KERN_ALERT "Hello, world. Clock speed (ms): %li\n", msClockSpeed);
	return 0;
}

static void hello_exit(void){
	unsigned long jiffiesTimeTakenMs = 1000 * (jiffies -jiffiesStartTimeStamp) / HZ;
	//long long kTimeTakenMs = (ktime_get_boottime() - kTimeStartTimeStamp);
	printk(KERN_ALERT "Goodbye, cruel world\n");
	printk(KERN_ALERT "Jiffies Insertion to Removal Time (ms): %li\n", jiffiesTimeTakenMs);
	//printk(KERN_ALERT "K_Time Insertion to Removal Time (ms): %lli\n", kTimeTakenMs);

}

module_init(hello_init);
module_exit(hello_exit);
