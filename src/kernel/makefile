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

export CPATH := $(shell find ../ -type d | tr '\n' ':')

CF += -DKERNEL_SEGMENT=$(KERNEL_SEGMENT) -DCLUSTERS=$(CLUSTERS) -DENTRIES=$(ENTRIES) -DFAT_SIZE=$(FAT_SIZE)
BIN := ../../bin

all: kernel.bin

kernel.bin: kbd.o pit.o rtc.o spkr.o storage.o vga.o
	gcc $(CF) kernel.c $(LF) -o kernelc.o
	nasm -f elf stubs.asm
	ld kernelc.o stubs.o $(BIN)/arch.o $(BIN)/math.o $(BIN)/string.o $^ -melf_i386 -r -o kernel.o
	ld -melf_i386 -Tkernel.ld -o kernel.bin
	rm *.o
	mv kernel.bin $(BIN)/.

kbd.o:
	gcc $(CF) drv/kbd/kbd.c $(LF) -o kbdc.o
	nasm -f elf drv/kbd/kbdISR.asm -o kbdISR.o
	ld -r kbdc.o kbdISR.o -melf_i386 -o $@

pit.o:
	gcc $(CF) drv/pit/pit.c $(LF) -o pitc.o
	nasm -f elf drv/pit/pitISR.asm -o pitISR.o
	ld -r pitc.o pitISR.o -melf_i386 -o $@

%.o: drv/*/%.c
	gcc $(CF) $^ $(LF) -o $@
