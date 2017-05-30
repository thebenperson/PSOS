/*

PSOS Development Build
https://github.com/TheBenPerson/PSOS/tree/dev

Copyright (C) 2016 - 2017 Ben Stockett <thebenstockett@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial vPortions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/

#include "types.h"
#include "usr.h"
#include "vga.h"

void main() {

	setVGAMode(640, 480, true);

	asm("mov es, %0" :: "a" (0xA000));

	dword val = 0;
	for (dword i = 0; i < 0xFFFFFFFF; i += 3) {

		asm("mov es:[%0], %1" :: "b" (i), "a" ((byte) 127));
		asm("mov es:[%0], %1" :: "b" (i + 1), "a" ((byte) 127));
		asm("mov es:[%0], %1" :: "b" (i + 2), "a" ((byte) 0x30));

		val++;

	}

}
