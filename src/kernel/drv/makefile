#PSOS Development Build
#Copyright (C) 2016 Ben Stockett.

#This file is part of PSOS (Pretty Simple/Stupid Operating System).

#PSOS is free software: you can redistribute it and/or modify
#it under the terms of the GNU General Public License as published by
#the Free Software Foundation, either version 3 of the License, or
#(at your option) any later version.

#PSOS is distributed in the hope that it will be useful,
#but WITHOUT ANY WARRANTY; without even the implied warranty of
#MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#GNU General Public License for more details.

#You should have received a copy of the GNU General Public License
#along with PSOS.  If not, see <http://www.gnu.org/licenses/>.

all: drv.o

drv.o: keyboard.o pit.o storage.o vga.o
	ld -melf_i386 -r $^ -o $@
	rm keyboard.o pit.o storage.o vga.o

keyboard.o: keyboard/keyboard.c
	gcc -I.. -I. -I../.. -c -masm=intel $^ -m16 -nostdlib -o keyboardc.o
	nasm -f elf keyboard/keyboardISR.asm -o keyboardISR.o
	ld -melf_i386 -r keyboardc.o keyboardISR.o -o keyboard.o
	rm keyboardc.o keyboardISR.o

pit.o: pit/pit.c
	gcc -I. -I.. -I../.. -c -masm=intel $^ -m16 -nostdlib -o pitc.o
	nasm -f elf pit/pitISR.asm -o pitISR.o
	ld -melf_i386 -r pitc.o pitISR.o -o pit.o
	rm pitc.o pitISR.o

storage.o: storage/storage.c
	gcc -I../.. -c -masm=intel $^ -m16 -nostdlib

vga.o: vga/vga.c
	gcc -I../.. -c -masm=intel $^ -m16 -nostdlib
