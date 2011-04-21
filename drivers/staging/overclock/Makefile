KERNEL_BUILD := ~/chameleon
KERNEL_CROSS_COMPILE := arm-eabi-
FILENAME := overclock

obj-m += $(FILENAME).o

all:
	ARCH=arm MACH=plat-omap make -C $(KERNEL_BUILD) CROSS_COMPILE=$(KERNEL_CROSS_COMPILE) M=$(PWD) modules
	$(KERNEL_CROSS_COMPILE)strip --strip-debug $(FILENAME).ko
	mv $(FILENAME).ko $(FILENAME)_droid_22.ko

clean:
	make -C $(KERNEL_BUILD) M=$(PWD) clean 2> /dev/null
	rm -f modules.order *~
