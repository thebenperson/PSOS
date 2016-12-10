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
#include "vga.h"

byte attribute = FG_GREEN;

word charX = 0;
word charY = 0;

void clearText() {

	charX = 0;
	charY = 0;

	word length = 80 * 25 * 2;

	asm("mov ax, 0xB800");
	asm("mov es, ax");

	for (word i = 0; i < length; i += 2) {

		asm("movb es:[%0], 0" :: "b" (i));
		asm("movb es:[bx + 1], %0" :: "a" (attribute));

	}

}

void initVGA() {

	setVGAMode(3);
	setCursor(false);

}

void printChar(char character) {

	asm("mov ax, 0xB800");
	asm("mov es, ax");
	asm("movb es:[%0], %1" :: "b" ((charY * 160) + (charX * 2)), "a" (character));
	asm("movb es:[bx + 1], %0" :: "a" (attribute));

	charX++;

}

void printString(ptr string) {

	asm("mov ax, 0xB800");
	asm("mov es, ax");

	while (*((char*) string) != '\0') {

		if (*((char*) string) == '\n') {

			charY++;
			charX = 0;

		} else {

			asm("movb es:[%0], %1" :: "b" ((charY * 160) + (charX * 2)), "a" (*((char*) string)));
			asm("movb es:[bx + 1], %0" :: "a" (attribute));

			charX++;

		}

		string++;

	}

}

void setCursor(bool enabled) {

	if (enabled) asm("mov cx, 0x7");
	else asm("mov cx, 0x700");

	asm("mov ah, 1");
	asm("int 0x10"); //set cursor

}

void setVGAMode(byte mode) {

	asm("xor ah, ah");
	asm("mov al, %0" :: "r" (mode));
	asm("int 0x10"); //set VGA mode

}
