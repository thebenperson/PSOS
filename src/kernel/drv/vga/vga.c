/*

PSOS Development Build
https://github.com/TheBenPerson/PSOS/tree/dev

Copyright (C) 2016 Ben Stockett <thebenstockett@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/

#include "types.h"
#include "vga.h"

byte charAttr = FG_WHITE;
word vidOffset = 0;

void clearText() {

	word i; //stupid K&R syntax

	#asm

		mov ax, #0xB800
		mov es, ax

	#endasm

	for (i = 0; i < (80 * 25 * 2); i += 2) {

		#asm

			mov bx, [bp - 2]

			seg es
			mov [bx], #0

			mov al, [_charAttr]

			seg es
			mov [bx + 1], al

		#endasm

	}

	vidOffset = 0;

}

void initVGA() {

	setVGAMode(3);
	setCursor(false);

}

void moveCursor(byte row, byte column) {

	//asm("xor bh, bh");
	//asm("int 0x10" :: "a" (0x2 << 8), "d" ((column << 8) | row));

}

void printChar(char character) {

	//asm("mov es, %0" :: "a" (0xB800));
	//asm("movb es:[%0], %1" :: "b" ((charY * 160) + (charX * 2)), "a" (character));
	//asm("movb es:[bx + 1], %0" :: "a" (charAttr));

	/*if (++charX == 80) {

		charY++;
		charX = 0;

	}

	moveCursor(charX, charY);*/

}

void printString(char* string) {

	#asm

		mov ax, #0xB800
		mov es, ax

	#endasm

	while (*string != '\0') {

		if (*string == '\n') {

			vidOffset += 160 - (vidOffset % 160);

		} else {

			#asm

				mov bx, [bp + 4]
				mov al, [bx] //get next character

				mov bx, [_vidOffset]

				seg es
				mov [bx], al

				mov al, [_charAttr]

				seg es
				mov [bx + 1], al

			#endasm

			vidOffset += 2;

		}

		string++;

	}

}

/*void scroll() {

	char far* vram = (char far*) 0xB8000000;

	for (word i = 0; i < (80 * 25 * 2); i += 2) {

		//asm("movw ax, es:[%0 + 160]" :: "b" (i));
		//asm("movw es:[bx], ax");

	}

}

void setCharAttr(byte attr) {

	charAttr = attr;

}

void setCharPos(byte x, byte y) {

	charX = x;
	charY = y;

}*/

void setCursor(bool enabled) {

	if (enabled) {

		#asm

			mov cx, #0x7

		#endasm

	} else {

		#asm

			mov cx, #0x700

		#endasm

	}

	#asm

		mov ah, #0x1
		int 0x10

	#endasm

}

void setVGAMode(byte mode) {

	#asm

		push bp
		mov bp, sp

		xor ah, ah
		mov al, [bp + 4]
		int 0x10

		pop bp

	#endasm

}
