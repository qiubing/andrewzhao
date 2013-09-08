KERNELDIR := /lib/modules/$(shell uname -r)/build

PWD := $(shell pwd)

INSTALLDIR = /home/andrewzhao/kernel/build

CC = gcc

default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

install:
	cp rfkill.ko $(INSTALLDIR)

.PHONY: modules install clean
