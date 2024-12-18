#include <linux/init.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/sched.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <asm/errno.h>
#include <linux/sched.h>
#include <linux/cred.h>

//Load module license
MODULE_LICENSE("Dual BSD/GPL");

// Device name
#define DEVICE_NAME "lab8"

// Define max buffer size for reading
#define MAX_BUFFER 21

// Variable for device being open, prevents multiple opens
static int Device_Open = 0;

// Global variable with user id (max # of digits for kuid_t is 20, unsigned 64-bit int)
//	Therfore maximum size is 20 digits + null char
#define MAX_USER_SIZE 21
static char user_id[21];

// Static struct with misc device structure
static struct miscdevice lab8_device;

// Function prototypes
static int labOpen(struct inode *, struct file *);
static ssize_t labRead(struct file *, char *, size_t, loff_t *);

// Struct of file_operations for device
static struct file_operations fops = {
	.owner = THIS_MODULE,
	.read = labRead,
	.open = labOpen,
};


// Init function for driver
static int lab8_init_module(void){
	int retval;
	lab8_device.minor = MISC_DYNAMIC_MINOR;
	lab8_device.name = DEVICE_NAME;
	lab8_device.fops = &fops;
	lab8_device.mode = 0666;
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
	if (Device_Open) return -EBUSY;
	Device_Open++;

	kuid_t current_user_id = current->cred->uid;

	sprintf(user_id, "%d", __kuid_val(current_user_id));

	return 0;
}

// Driver "read()" function, named labRead()
static ssize_t labRead(struct file *fp, char *buffer, size_t length, loff_t *offset){
	// If we are at end of message, return 0 signifying end of file
	if (*offset >= MAX_USER_SIZE)
		return 0;
	
	// Limit the length to the size of the remaining buffer (aka. size of buffer - offset).
	if (length > MAX_USER_SIZE - *offset)
		length = MAX_USER_SIZE - *offset;
	
	// Copy data from kernel space to user space
	if (copy_to_user(buffer, user_id + *offset, length)){
		// If copy fails return error
		return -EFAULT;
	}

	// Increment offset
	*offset += length;

	return length;
}


// Module init and exit calling
module_init(lab8_init_module);
module_exit(lab8_exit_module);
