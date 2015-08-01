# Makefile for Xillybus drivers
#
# Will compile and install for other kernel than the currently running,
# given the TARGET parameter (should be the name of a directory in
# /lib/modules) e.g.
# make TARGET=3.10.0-CUSTOM
#
# This makefile compiles the drivers for Xillybus using PCIe. For Device Tree
# (Open Firmware) support, uncomment where xillybus_of is mentioned below
# (and possibly comment out occurrences of xillybus_pcie).

ifneq ($(KERNELRELEASE),)
obj-m	:= xillybus_core.o
obj-m	+= xillybus_pcie.o
# obj-m	+= xillybus_of.o

else
ifeq ($(TARGET),)
TARGET := $(shell uname -r)
endif
PWD := $(shell pwd)
KDIR := /lib/modules/$(TARGET)/build

default:
	@echo $(TARGET) > module.target
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules

clean:
	@rm -f *.ko *.o modules.order Module.symvers *.mod.? .xillybus_*.* *~
	@rm -rf .tmp_versions module.target

install: xillybus_core.ko xillybus_pcie.ko # xillybus_of.ko
	install --mode 0644 xillybus_core.ko /lib/modules/$(shell cat module.target)/kernel/drivers/char/
	install --mode 0644 xillybus_pcie.ko /lib/modules/$(shell cat module.target)/kernel/drivers/char/
#	install --mode 0644 xillybus_of.ko /lib/modules/$(shell cat module.target)/kernel/drivers/char/
	/sbin/depmod -a $(shell cat module.target)
xillybus.ko:
	$(MAKE)
endif
