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
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/

#include "kbd.h"
#include "types.h"
#include "usr.h"

void keyHandler(byte scanCode);

byte i = 0;
char buffer[13];

void main() {

	setCursor(true);
	puts("sh - a simple shell\nPress escape to exit.\n\nsh>");
	setCallback(keyHandler);

	for (;;) {

		asm("hlt");

		if (getKey(VK_ESC)) break;
		if (getKey(VK_RETURN)) {

			putc('\n');

			buffer[i++] = '.';
			buffer[i++] = 'b';
			buffer[i++] = 'i';
			buffer[i++] = 'n';
			buffer[i++] = '\0';

			if (!exec(buffer)) {

				puts("Error: command does not exist\nEnter 'help' for help\n");

			}

			i = 0;

			puts("sh>");
			sleep(200); //enough time for driver to detect keyup

		}

	}

	setCursor(false);

}

void keyHandler(byte scanCode) {

	switch (scanCode) {

		case VK_LEFT:

			if (i) {

				i--;
				setPosition(getPosition() - 2);

			}

		break;

		case VK_RIGHT:

			if (i < 8) {

				i++;
				setPosition(getPosition() + 2);

			}

		break;

		case VK_BKSPACE:

			if (i) {

				buffer[i--] = '\0';
				word position = getPosition() - 2;
				setPosition(position);
				putc('\0');
				setPosition(position);

			}

		break;

		case VK_L:

			if (getKey(VK_LCTRL)) {

				clearText();
				puts("sh>");

				break;

			}

		default: {

			char c = toChar(scanCode);
			if ((c) && (i < 8)) {

				buffer[i++] = c;
				putc(c);

			}

		}

	}

}
