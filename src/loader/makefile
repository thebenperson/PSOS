#PSOS Development Build
#https://github.com/TheBenPerson/PSOS/tree/dev

#Copyright (C) 2016 Ben Stockett <thebenstockett@gmail.com>

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

BIN := ../../bin
KERNEL_SIZE := $(shell du -B 512 --apparent-size $(BIN)/kernel.bin | awk '{print $$1;}')
CFLAGS := -I.. -I/usr/include -I/usr/include/unistring -DKERNEL_SIZE=$(KERNEL_SIZE) -ansi
LFLAGS := -c -Mc

all: loader.bin

loader.bin: loaderStub.o storage.o vga.o
	bcc $(CFLAGS) loader.c $(LFLAGS)
	ld86 -d $^ loader.o $(BIN)/math.o -o loader.bin
	objcopy -I binary loader.bin -Bi386 -O elf32-i386 loader.o
	ld -melf_i386 -Tloader.ld -o loader.bin
	rm *.o
	mv loader.bin $(BIN)/loader.bin

loaderStub.o:
	nasm loaderStub.asm -f as86

storage.o:
	bcc $(CFLAGS) drv/storage/storage.c $(LFLAGS) -o $@

vga.o:
	bcc $(CFLAGS) drv/vga/vga.c $(LFLAGS) -o $@
