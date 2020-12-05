#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/string.h>
#include <linux/init.h>

struct proc_dir_entry *dir_entry;
#define PROCFS_MAX_SIZE PAGE_SIZE
char proc_data[PROCFS_MAX_SIZE]; // variable to store the string
int totalsize=0;


ssize_t write(struct file *filep, const char __user *buff, size_t len,loff_t *offset) {
	copy_from_user(proc_data,buff,len); 
	printk(KERN_INFO "shetty_strlen module recieved the string");
	totalsize =len;	
	proc_data[len] = '\0';
	return len; 

}

ssize_t read(struct file *filep, char __user *buff, size_t len,loff_t *offset ) 
{	
	ssize_t byteread;
	int result = strlen(proc_data);
	char arr[100];
	sprintf(arr, "%d\n", result);
	byteread = totalsize;
	if(copy_to_user(buff,arr,byteread))
	{
		return -EFAULT;
	}
	totalsize = totalsize - byteread;
	printk(KERN_INFO "shetty_strlen module providing string length\n");	
	return byteread;
}

static struct file_operations shetty_strlen_fops = {.read=read, .write=write, };

static int shetty_strlen_init(void) // initializes and creates the module using proc_create. 
{
	printk(KERN_INFO "shetty_strlen module loaded.\n");
  	dir_entry = proc_create( "shetty_strlen", 0666, NULL, &shetty_strlen_fops);
  
return 0;
}

static void shetty_strlen_exit(void) // It has the entries for module unloading.
{  
	remove_proc_entry("shetty_strlen", NULL);
	printk(KERN_INFO "shetty_strlen module unloaded.\n");
}

module_init(shetty_strlen_init); // module defined for loading is specified.
module_exit(shetty_strlen_exit); // module defined for unloading is specified.
