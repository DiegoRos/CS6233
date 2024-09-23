# Makefile for simple.c diver

# if KERNELRELEASE is defined, we've invoked from the 
# kernel build system and it can use its language.
ifneq ($(KERNELRELEASE),)
	obj-m := simple.o

# Otherwise we were called direcly from the command line
# invoke kernel build system.
else
	KERNELDIR ?= /lib/modules/$(shell uname -r)/build
	PWD := $(shell pwd)

default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

endif
