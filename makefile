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

#filesystem options
CLUSTERS := 4085
ENTRIES := 256
FAT_SIZE := $(shell let "FAT_SIZE = ($(CLUSTERS) + 256 - 1) / 256"; echo $$FAT_SIZE)

#kernel options
KERNEL_SEGMENT := 0x7E0
DRIVERS := kbd.o pit.o rtc.o spkr.o storage.o vga.o
LIBS := arch.o math.o string.o

#flags
export CPATH := $(shell find src -type d | tr '\n' ':')
DEF := -DKERNEL_SEGMENT=$(KERNEL_SEGMENT) -DCLUSTERS=$(CLUSTERS) -DENTRIES=$(ENTRIES) -DFAT_SIZE=$(FAT_SIZE)
CF := $(DEF) -masm=intel -Wno-int-conversion -Wno-int-to-pointer-cast -Wno-pointer-to-int-cast -march=i386 -m16
LF := -c -ffreestanding -Os -fno-pie -fno-pic -fno-stack-protector

#build directories
ROOT := bin
OBJ := $(ROOT)/obj
FS := $(ROOT)/fs
IMG := $(ROOT)/img
ISO := $(ROOT)/iso

#default targets
all: $(addprefix $(ROOT)/, PSOS.img PSOS.iso)

$(ROOT)/PSOS.img: $(addprefix $(IMG)/, loader.img kernel.img fs.img)
	cat $^ > $@
	sudo mount $@ mnt
	sudo cp $(FS)/* mnt/. > /dev/null
	sudo umount mnt
	cp $@ $(ISO)/.

$(ROOT)/PSOS.iso: $(ROOT)/PSOS.img
	truncate -s 2880K $(ISO)/PSOS.img
	mkisofs -o $@ -V PSOS-dev -b PSOS.img $(ISO)

include src/loader/makefile
include src/kernel/makefile
include src/fs/makefile
include src/lib/makefile

#aliases for explicit targets
%.o: $(OBJ)/%.o
	#made $@

%.bin: $(FS)/%.bin
	#made $@

PSOS.img: $(ROOT)/PSOS.img
	#made $@

%.img: $(IMG)/%.img
	#made $@

%.iso: $(ROOT)/%.iso
	#made $@

.PHONY: clean
clean:
	-rm $(OBJ)/* $(FS)/* $(IMG)/* $(ROOT)/* 2> /dev/null
