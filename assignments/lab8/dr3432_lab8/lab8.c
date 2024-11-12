#include <linux/init.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/sched.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <asm/errno.h>
#include <linux/cred.h>

//Load module license
MODULE_LICENSE("Dual BSD/GPL");

// Device name
#define DEVICE_NAME "lab8"

// Define max buffer size for reading
#define MAX_BUFFER 100

// Variable for device being open, prevents multiple opens
static int Device_Open = 0;

// Global variable with user id (max # of digits for kuid_t is 20, unsigned 64-bit int)
//	Therfore maximum size is 20 digits + null char
#define MAX_USER_ID 21
static char user_id[21];

// Static struct with misc device structure
static struct miscdevice lab8_device;

// Function prototypes
static int labOpen(struct inode *, struct file *);
static ssize_t labRead(struct file *, char *, size_t, loff_t *);

// Struct of file_operations for device
static struct file_operations fops = {
	.read = labRead,
	.open = labOpen
};


// Init function for driver
static int lab8_init_module(void){
	int retval;
	lab8_device.minor = MISC_DYNAMIC_MINOR;
	lab8_device.name = DEVICE_NAME;
	lab8_device.fops = &fops;
	retval = misc_register(&lab8_device);
	// If error return retval.
	if (retval) return retval;

	printk(KERN_ALERT "Lab8 got minor %i.\n", lab8_device.minor);

	return 0;
}

// Exit function for driver
static void lab8_exit_module(void){
	printk(KERN_ALERT "Goodbye, cruel world\n");


	misc_deregister(&lab8_device);
}

// Driver "open()" function, named labOpen()
static int labOpen(struct inode *inode, struct file *file){
	static int counter = 0;
	if (Device_Open) return -EBUSY;
	Device_Open++;

	struct cred process_credentials = 
	unsigned int current_user_id = current_uid();
	sprintf(user_id, "%u", (unsigned int)current_user_id);
	
	printk(KERN_ALERT "Driver opened, %u\n", current_user_id);

	return 0;
}

// Driver "read()" function, named labRead()
static ssize_t labRead(struct file *fp, char *buffer, size_t length, loff_t *offset){
	int bytes_read = 0;
	
	// If we are at end of message, return 0 signifying end of file
	
	return 0;
}



// Module init and exit calling
module_init(lab8_init_module);
module_exit(lab8_exit_module);
