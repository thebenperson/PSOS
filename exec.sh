#!/bin/sh

qemu-system-x86_64 -fda bin/PSOS.img -enable-kvm -m 1024 $@ 2> /dev/null
