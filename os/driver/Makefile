KDIR := /lib/modules/$(shell uname -r)/build
MOD_NAME1 = alex_drv
MOD_NAME2 = alex_dev

$(MOD_NAME1)-objs := driver.o  tc_test.o
$(MOD_NAME2)-objs := device.o 

obj-m := $(MOD_NAME1).o  $(MOD_NAME2).o

PWD=$(shell pwd)

all:
	make $(ACROSS_ARCH) -C $(KDIR) M=$(PWD) modules

clean:
	 make -C $(KDIR) M=$(PWD) clean
