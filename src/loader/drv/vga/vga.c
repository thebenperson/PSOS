/*

PSOS Development Build
Copyright (C) 2016 Ben Stockett.

This file is part of PSOS (Pretty Simple/Stupid Operating System).

PSOS is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

PSOS is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with PSOS.  If not, see <http://www.gnu.org/licenses/>.

*/

#include "types.h"

void initVGA() {

	asm volatile("xor ah, ah");
	asm volatile("mov al, 3");
	asm volatile("int 0x10"); //set VGA mode 3

	asm volatile("mov ah, 1");
	asm volatile("mov cx, 0x700");
	asm volatile("int 0x10"); //disable cursor

}

void printString(ptr string) {

	asm volatile("mov ax, 0xB800");
	asm volatile("mov es, ax");
	asm volatile("xor bx, bx");
	asm volatile("movb es:[bx], 'A'");

	word X = 0;
	word Y = 0;

	while (*((char*) string) != '\0') {

		if (*((char*) string) == '\n') {

			Y++;
			X = 0;

		} else {

			asm volatile("movb es:[%0], %1" :: "a" ((Y * 160) + (X * 2)), "b" (*((char*) string)));

			X++;

		}

		string++;

	}

}
