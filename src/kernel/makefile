# PSOS Development Build
# https://github.com/TheBenPerson/PSOS/tree/dev
#
# Copyright (C) 2016 - 2017 Ben Stockett <thebenstockett@gmail.com>
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

$(IMG)/kernel.img: $(addprefix $(OBJ)/, kernel.o $(DRIVERS) $(LIBS))
	ld $^ -melf_i386 -r -o kernel.o
	ld -melf_i386 -Tsrc/kernel/kernel.ld -o $@
	rm *.o

$(OBJ)/kernel.o: src/kernel/kernel.c
	gcc $(CF) $^ $(LF) -o kernelc.o
	nasm -f elf src/kernel/stubs.asm -o stubs.o
	ld kernelc.o stubs.o -melf_i386 -r -o $@
	rm *.o

$(OBJ)/kbd.o: src/kernel/drv/kbd/kbd.c src/kernel/drv/kbd/kbdISR.asm
	gcc $(CF) src/kernel/drv/kbd/kbd.c $(LF) -o kbdc.o
	nasm -f elf src/kernel/drv/kbd/kbdISR.asm -o kbdISR.o
	ld -r kbdc.o kbdISR.o -melf_i386 -o $@
	rm *.o

$(OBJ)/pit.o: src/kernel/drv/pit/pit.c src/kernel/drv/pit/pitISR.asm
	gcc $(CF) src/kernel/drv/pit/pit.c $(LF) -o pitc.o
	nasm -f elf src/kernel/drv/pit/pitISR.asm -o pitISR.o
	ld -r pitc.o pitISR.o -melf_i386 -o $@
	rm *.o

$(OBJ)/%.o: src/kernel/drv/*/%.c
	gcc $(CF) $^ $(LF) -o $@
