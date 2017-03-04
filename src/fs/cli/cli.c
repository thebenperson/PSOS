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

#include "keyboard/keyboard.h"
#include "vga/vga.h"

void main() {

	keyCallback = &keyHandler;
	while (!(keyState[VK_LALT] && keyState[VK_C])) asm("hlt");

}

void keyHandler(byte scanCode) {

	switch (scanCode) {

		case VK_BACKSPACE:

			charX -= 2;
			printChar(" ");

		break;

		case VK_LEFT:

			if (charX - 2) charX--;

		break;

		case VK_RIGHT:

			charX++;

		break;

		case VK_RETURN:

			if (charY + 1 == 25) scroll();
			else charY++;

			charX = 0;

			printString(">>");

		break;

		default: {

			char c = scToChar(scanCode);
			if (c) printChar(c);

		} break;

	}

}
