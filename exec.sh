#!/bin/sh

qemu-system-x86_64 -cdrom bin/PSOS.iso -enable-kvm -vga qxl -m 1024 $@ &> /dev/null
