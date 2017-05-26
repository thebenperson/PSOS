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

BIN := ../../bin

all: fs.img sh.bin help.bin clear.bin date.bin free.bin lscpu.bin vgatest.bin

fs.img: fat16.asm
	nasm -DCLUSTERS=$(CLUSTERS) -DENTRIES=$(ENTRIES) -DFAT_SIZE=$(FAT_SIZE) $^ -f bin -o $@
	mv $@ $(BIN)/$@

sh.bin: sh/main.c
	gcc $(CF) $^ $(LF) -o sh.o
	ld -melf_i386 sh.o $(BIN)/usr.o -Tbin.ld -o $(BIN)/$@
	rm *.o

help.bin: help/main.c
	gcc $(CF) $^ $(LF) -o help.o
	ld -melf_i386 help.o $(BIN)/usr.o -Tbin.ld -o $(BIN)/$@
	rm *.o

clear.bin: clear/main.c
	gcc $(CF) $^ $(LF) -o help.o
	ld -melf_i386 help.o $(BIN)/usr.o -Tbin.ld -o $(BIN)/$@
	rm *.o

date.bin: date/main.c
	gcc $(CF) $^ $(LF) -o date.o
	ld -melf_i386 date.o $(BIN)/usr.o -Tbin.ld -o $(BIN)/$@
	rm *.o

free.bin: free/main.c
	gcc $(CF) $^ $(LF) -o free.o
	ld -melf_i386 free.o $(BIN)/usr.o $(BIN)/arch.o -Tbin.ld -o $(BIN)/$@
	rm *.o

lscpu.bin: lscpu/main.c
	gcc $(CF) $^ $(LF) -o lscpu.o
	ld -melf_i386 lscpu.o $(BIN)/usr.o $(BIN)/arch.o -Tbin.ld -o $(BIN)/$@
	rm *.o

vgatest.bin: vgatest/main.c
	gcc $(CF) $^ $(LF) -o vgatest.o
	ld -melf_i386 vgatest.o $(BIN)/usr.o $(BIN)/arch.o -Tbin.ld -o $(BIN)/$@
	rm *.o
