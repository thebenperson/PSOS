#!/bin/sh

qemu-system-x86_64 -fda bin/PSOS.img -enable-kvm -rtc base=localtime -soundhw pcspk $@ 2> /dev/null
