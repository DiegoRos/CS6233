// Load required libraries.
#include <linux/init.h>
#include <linux/module.h>
#include <linux/jiffies.h>
#include <linux/timekeeping.h>
#include <linux/ktime.h>

//Load module license
MODULE_LICENSE("Dual BSD/GPL");

//Global variables for init timestamps 
unsigned long long jiffiesStartTimeStamp;
unsigned long long kTimeStartTimeStamp;

static int hello_init(void){
	// Set both jiffies and kTime init stamps
	jiffiesStartTimeStamp = jiffies;
	kTimeStartTimeStamp = ktime_to_ms(ktime_get_boottime());
	
	// Calculate clockspeed in ms
	unsigned long msClockSpeed = 1000 / HZ;

	// Init print statement
	printk(KERN_ALERT "Hello, world. Clock speed (ms): %li\n", msClockSpeed);
	return 0;
}

static void hello_exit(void){
	// Get current time at unload and calculate module time loaded
	unsigned long long jiffiesTimeTakenMs = 1000 * (jiffies -jiffiesStartTimeStamp) / HZ;
	unsigned long long kTimeTakenMs = (ktime_to_ms(ktime_get_boottime()) - kTimeStartTimeStamp);

	// Exit and result print statements
	printk(KERN_ALERT "Goodbye, cruel world\n");
	printk(KERN_ALERT "Jiffies Insertion to Removal Time (ms): %lli\n", jiffiesTimeTakenMs);
	printk(KERN_ALERT "K_Time Insertion to Removal Time (ms): %lli\n", kTimeTakenMs);

}

// Module init and exit calling
module_init(hello_init);
module_exit(hello_exit);
