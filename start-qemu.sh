#!/bin/bash

# Determine name of kernel image to boot
KERNELPATH="linux-5.10.252/arch/x86_64/boot/bzImage"
if [[ ! -f $KERNELPATH ]]; then
	echo "no bzImage found. Please compile first"
	exit 1
fi

# the number after the -m flag is the amount of memory you are going to give to the machine.
# you can give more if you want. Please give at least 256M to ensure the assignment works properly.
qemu-system-x86_64 -m 256M -nographic -kernel $KERNELPATH -hda ./openwrt-21.02.7-x86-64-generic-ext4-combined.img -append "root=/dev/sda2 rootfstype=ext4 rootwait console=tty0 console=ttyS0,38400n8 edd=off noinitrd" -hdb fat:rw:`pwd`/shared
