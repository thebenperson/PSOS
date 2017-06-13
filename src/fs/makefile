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

PROG := sh.bin help.bin beep.bin clear.bin date.bin free.bin lscpu.bin vgatest.bin

$(IMG)/fs.img: $(addprefix $(FS)/, $(PROG))
	nasm -DCLUSTERS=$(CLUSTERS) -DENTRIES=$(ENTRIES) -DFAT_SIZE=$(FAT_SIZE) src/fs/fat16.asm -f bin -o $@

$(FS)/free.bin: $(addprefix $(OBJ)/, free.o usr.o arch.o)
	ld -melf_i386 $^ -Tsrc/fs/bin.ld -o $@

$(FS)/lscpu.bin: $(addprefix $(OBJ)/, lscpu.o usr.o arch.o)
	ld -melf_i386 $^ -Tsrc/fs/bin.ld -o $@

$(FS)/%.bin: $(addprefix $(OBJ)/, %.o usr.o)
	ld -melf_i386 $^ -Tsrc/fs/bin.ld -o $@

$(OBJ)/%.o: src/fs/%/main.c
	gcc $(CF) $^ $(LF) -o $@
