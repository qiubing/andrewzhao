KERNELDIR := /lib/modules/$(shell uname -r)/build

PWD := $(shell pwd)

INSTALLDIR = /home/andrewzhao/kernel/build

obj-m := wkq.o

CC = gcc

default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

install:
	cp wkq.ko $(INSTALLDIR)

.PHONY: modules install clean
