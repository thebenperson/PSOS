#!/bin/sh

qemu-system-x86_64 -fda bin/PSOS.img -enable-kvm -vga qxl -m 1024 $@ &> /dev/null
