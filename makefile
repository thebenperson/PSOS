#PSOS Development Build
#https://github.com/TheBenPerson/PSOS/tree/dev

#Copyright (C) 2016 - 2017 Ben Stockett <thebenstockett@gmail.com>

#Permission is hereby granted, free of charge, to any person obtaining a copy
#of this software and associated documentation files (the "Software"), to deal
#in the Software without restriction, including without limitation the rights
#to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
#copies of the Software, and to permit persons to whom the Software is
#furnished to do so, subject to the following conditions:

#The above copyright notice and this permission notice shall be included in all
#copies or substantial portions of the Software.

#THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
#IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
#FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
#AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
#LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
#OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
#SOFTWARE.

#FAT16 options
export CLUSTERS := 4085
export ENTRIES := 256
export FAT_SIZE := $(shell let "FAT_SIZE = ($(CLUSTERS) + 256 - 1) / 256"; echo $$FAT_SIZE)
#Kernel options
export KERNEL_SEGMENT := 0x7E0
#Compile flags
export CF := -masm=intel -Wno-int-conversion -Wno-int-to-pointer-cast -Wno-pointer-to-int-cast -m16
export LF := -c -ffreestanding -Os

all: PSOS

lib:
	make -C src/lib

PSOS: loader kernel fs
	cat bin/loader.bin bin/kernel.bin bin/fs.img > bin/$@.img
	rm bin/*.o bin/loader.bin bin/kernel.bin bin/fs.img
	sudo mount bin/PSOS.img mnt
	sudo mv bin/*.bin mnt/.
	sudo umount mnt
	#mkisofs -o bin/PSOS.iso -V PSOS-dev -hard-disk-boot -b PSOS.img bin

loader: kernel
	make -C src/loader

kernel: lib
	make -C src/kernel

fs: lib
	make -C src/fs
