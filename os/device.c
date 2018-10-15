#include<linux/module.h>
#include<linux/init.h>
#include<linux/types.h>
#include<linux/platform_device.h>

#define DEVICE_NAME "AlEX"

static struct platform_device alex_pdev = {
	.name  = DEVICE_NAME,
	.id    = 1,

};


static int __init alex_dev_init() {
 	int  ret;
	printk("Hello world, this is Alex dev!\n");

	//add alex dev to system
	ret = platform_device_register(&alex_pdev);

	if(ret < 0) {
		printk("can't register alex_pdev!\n");
		return ret;
	}

	return 0;
}

static void __exit alex_dev_exit(void) {
	
	platform_device_unregister(&alex_pdev);

	printk("bye world, this is alex dev!\n");

	return;
}


module_init(alex_dev_init);
module_exit(alex_dev_exit);

MODULE_LISENCE("GPL");

   


