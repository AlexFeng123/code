#include<linux/module.h>
#include<linux/types.h>
#include<linux/platform_device.h>
#include<linux/init.h>
#include<linux/fs.h>
#include<linux/cdev.h>

#include<asm/uaccess.h>

#define  DEVICE_NAME "ALEX"

#define ALEX_DEFAULT_MAJOR (111)
#define ALEX_NUMS          (2)


/* the alex char device and it's major*/

static struct cdev   alex_dev;

static unsigned char alex_major = ALEX_DEFAULT_MAJOR;


#define BUF_LEN   (32)

static char alex_mem[BUF_LEN];

extern void ts_test_stop(void);
extern void ts_test_start(void);

static int alex_open(struct inode *inode, struct file *filp) {

	unsigned int major, minor;

	major = imajor(inode);
	minor = iminor(inode);

	printk("%s , major = %d , minor = %d !\n", __func__,major, minor);

	return 0;
}

static ssize_t alex_read(struct file *filp, char *buf, size_t  len, loff_t *offset){

	printk("%s, len = %d\n", __func__, len);

	if(filp = NULL || buf == NULL || len == 0 ) {

		printk("params error!\n");

		return -1;
	}


	if(len > BUF_LEN)
	   len = BUF_LEN;

	copy_to_user(buf,alex_mem,len);

	return;
}



static ssize_t alex_write(struct file *filp, const char *buf, size_t len, loff_t *offset) {

	printk("%s, len = %d\n", __func__, len);


	if(filp = NULL || buf == NULL || len == 0 ) {

		printk("params error!\n");

		return -1;
	}


	if(len > BUF_LEN)
	   len = BUF_LEN;

        copy_from_user(alex_mem, buf, len);

	if(strncmp(buf,"start\n", sizeof(buf)) == 0){
                ts_test_start();
	}

	if(strncmp(buf, "stop\n", sizeof(buf)) == 0) {
		ts_test_stop();
	}


	return len;
}


static int alex_close(struct inode *inode, struct file *filp) {

	printk("%s\n", __func__);

	return 0;
}

static  struct file_operations alex_fops = {
	.open = alex_open,
	.read = alex_read,
	.write  = alex_write,
	.release = alex_close,
};



static int alex_probe(struct platform_device *pdev) {


	int ret;
	dev_t devno;
	printk("%s!\n",__func__);

	if(alex_major > 0 ){

		devno =MKDEV(alex_major,0);
		ret = register_chrdev_region(devno,ALEX_NUMS, DEVICE_NAME);
		if(ret < 0) {
			printk("can't static register chrdev region!\n");
			return ret;
		}

	}
	else {
        	ret = alloc_chrdev_region(&devno,0,ALEX_NUMS, DEVICE_NAME);
		if(ret < 0){
			printk("can't alloc chrdev region!\n");
			return ret;
		}
		alex_major = MAJOR(devno);
	}
	
	cdev_init(&alex_cdev,&alex_fops);
	
	alex_cdev.owner = THIS_MODULE;
	alex_cdev.ops = &alex_fops;

	ret = cdev_add(&alex_cdev, devno, ALEX_NUMS);
	if(ret < 0 ) {
		printk("can't add char device!\n");
		unregister_chrdev_region(devno,ALEX+_NUMS);
		return ret;
	}

        return 0;
}


static int alex_remove (struct platform_device *pdev){
	printk("%s \n",__func__);
        

	cdev_del(&alex_dev);

	unregister_chrdev_region(MKDEV(alex_major,0),ALEX_NUMS):

	ts_test_stop();

	return 0;

}


static struct platform_driver alex_drv = {
	.driver = {
	     .name = DEVICE_NAME,
	},
	.probe = alex_probe;
	.remove = alex_remove;
};


static int __init alex_drv_init(void){

	int ret;

	printk("Hello world, this is alex drv!\n");

	ret = platform_driver_register(&alex_drv);

	if(ret < 0) {
		printk("can't register alex drv!\n");
		return ret;
	}

	return 0;
}


static void __exit alex_drv_exit(void){
	
	platform_driver_unregister(&alex_drv);

	printk("bye world , this is alex drv\n");

	return;
}

module_init(alex_drv_init);
module_exit(alex_drv_exit);


MODULE_LISENSE("GPL");















