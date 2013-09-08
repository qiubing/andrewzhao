KERNELDIR := /lib/modules/$(shell uname -r)/build

PWD := $(shell pwd)

INSTALLDIR = /home/andrewzhao/kernel/build

obj-m := ch341.o

CC = gcc

default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

install:
	cp ch341.ko $(INSTALLDIR)

clean:
	rm -f modules.order Module.symvers *.ko *.mod.c *.mod.o *.o

.PHONY: modules install clean
