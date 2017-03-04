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

void initVGA() {

	#asm

		mov al, #0x3
		int 0x10

		mov ah, #0x1
		mov cx, #0x700
		int 0x10

	#endasm

}

void printString(char* string) {

	#asm

		mov ax, #0xB800
		mov es, ax

	#endasm

	word offset = 0;

	while (*string != '\0') {

		if (*string == '\n') {

			offset += 160 - (offset % 160);

		} else {

			#asm

				mov bx, [bp + 4]
				mov al, [bx] //get next character

				mov bx, [bp - 2] //get offset

				seg es //ackk! yuck! as86 is messed up (equates to [es:bx])
				mov [bx], al

				mov al, #0x4
				seg es
				mov [bx + 1], al

			#endasm

			offset += 2;

		}

		string++;

	}

}
