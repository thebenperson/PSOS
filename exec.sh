#!/bin/sh

qemu-system-x86_64 -drive file="bin/PSOS.bin",format=raw -enable-kvm -vga qxl -m 1024 $@
