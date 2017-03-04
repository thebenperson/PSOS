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

#include "kernel/drv/vga/vga.h"
#include "kernel/kernel.h"
#include "types.h"

void clearText() {

	asm("int 0x20" :: "a" (VGA | (CLEAR_TEXT << 8)));

}

void printString(ptr string) {

	asm("" :: "a" (VGA | (PRINT_STRING << 8)));
	asm("int 0x20" :: "S" (string));

}

void scroll() {

	asm("int 0x20" :: "a" (VGA | (SCROLL << 8)));

}

void setCharAttr(byte attr) {

	asm("" :: "a" (VGA | (SET_ATTR << 8)));
	asm("int 0x20" :: "b" (attr));

}

void setCharPos(byte X, byte Y) {

	asm("" :: "a" (VGA | (SET_POS << 8)));
	asm("int 0x20" :: "b" (X | (Y << 8)));

}

void setCursor(bool enabled) {

	asm("" :: "a" (VGA | (SET_CURSOR << 8)));
	asm("int 0x20" :: "b" (enabled));

}

void setVGAMode(byte mode) {

	asm("" :: "a" (VGA | (SET_MODE << 8)));
	asm("int 0x20" :: "b" (mode));

}
