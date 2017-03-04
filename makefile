#PSOS Version 1.0
#https://github.com/TheBenPerson/PSOS

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

FLAGS := -Isrc -f bin

all: PSOS.bin clean

PSOS.bin: loader.bin kernel.bin
	cat bin/loader.bin bin/kernel.bin > bin/$@

loader.bin: src/loader/loader.asm
	nasm $(FLAGS) -Isrc/loader $^ -o bin/$@

kernel.bin: src/kernel/kernel.asm
	nasm $(FLAGS) -Isrc/kernel $^ -o bin/$@

fs.bin: src/fs/fs.asm
	make -C src/fs/prg
	nasm $(FLAGS) -Isrc/fs $^ -o bin/$@

.PHONY: clean
clean:
	rm bin/loader.bin bin/kernel.bin
