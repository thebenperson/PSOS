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

#include "kernel.h"
#include "math.h"
#include "types.h"
#include "vga.h"

byte vAttr = BG_GREEN;
word vPort;
word vOffset = 0;

void scroll();

void initVGA() {

	ksetVGAMode(3);
	kclearText();
	ksetCursor(false);

	word tvPort;

	asm("mov es, %0" :: "a" (0));
	asm("mov %0, es:[0x463]" : "=g" (tvPort)); //address 0x0:0x463 in BDA is base VGA vPort
	vPort  = tvPort;

}

void kclearText() {

	asm("mov es, %0" :: "r" (0xB800));

	for (word i = 0; i < (80 * 25 * 2); i += 2) {

		asm("movw es:[%0], %1" :: "b" (i), "g" (vAttr << 8));

	}

	vOffset = 0;

}

void kputc(char c) {

	asm("mov es, %0" :: "a" (0xB800));

	if (vOffset >= (80 * 25 * 2)) {

		scroll();
		vOffset = (80 * 24 * 2);

	}

	if (c == '\n') {

		vOffset += 160 - (vOffset % 160);

	} else {

		asm("mov es:[%0], %1" :: "b" (vOffset), "r" (c));
		asm("mov es:[bx + 1], %0" :: "r" (vAttr));

		vOffset += 2;

	}

	ksetPosition(vOffset);

}

void kputn(word num, bool hex) {

	if (hex) kputs("0x");

	byte base = hex ? 16 : 10;
	byte nCarry = 0;
	byte place = 0;

	while (num) {

		bool carry;
		place = 0;
		int rem = num;

		while (rem >= base) {

			carry = !(rem % base);

			if (carry) nCarry++;
			else nCarry = 0;

			rem /= base;
			place++;

		}

		if ((hex) && (rem >= 10)) kputc(rem + 0x37);
		else kputc(rem + 0x30);

		if (carry) {

			for (byte i = 0; i < nCarry; i++)
				kputc('0');

			nCarry = 0;

		} else nCarry++;

		num -= (rem * pow(10, place));

	}

}

void kputs(mem16_t string) {

	word tSegment = syscalled ? uSegment : KERNEL_SEGMENT;

	asm("mov es, %0" :: "r" (0xB800));

	for (;;) {

		REMOTE();
		char c = *((char*) string);
		LOCAL();

		if (!c) {

			ksetPosition(vOffset);
			return;

		}

		if (vOffset >= (80 * 25 * 2)) {

			scroll();
			vOffset = (80 * 24 * 2);

		}

		if (c == '\n') {

			vOffset += 160 - (vOffset % 160);

		} else {

			asm("mov es:[%0], %1" :: "b" (vOffset), "r" (c));
			asm("mov es:[bx + 1], %0" :: "r" (vAttr));

			vOffset += 2;

		}

		string++;

	}

}

void ksetCursor(bool enabled) {

	asm("int 0x10" :: "a" (1 << 8), "c" ((word) (enabled ? 0x7 : 0x700)));

}

word ksetPosition(word position) {

	if (position == 0xFFFF) return vOffset;

	vOffset = position;
	position /= 2;

	asm("out %0, %1" :: "d" (vPort), "a" ((byte) 0xF));
	asm("out %0, %1" :: "d" ((word) (vPort + 1)), "a" ((byte) (position & 0xFF)));
	asm("out %0, %1" :: "d" (vPort), "a" ((byte) 0xE));
	asm("out %0, %1" :: "d" ((word) (vPort + 1)), "a" ((byte) (position >> 8)));

	return 0;

}

void ksetVGAMode(byte mode) {

	asm("int 0x10" :: "a" (mode));

}

void scroll() {

	asm("mov es, %0" :: "r" (0xB800));

	for (size_t i = 0; i < (80 * 24 * 2); i += 2) {

		asm("movw ax, es:[%0 + 160]" :: "b" (i));
		asm("movw es:[bx], ax");

	}

	for (size_t i = (80 * 24 * 2); i < (80 * 25 * 2); i += 2)
		asm("mov es:[%0], %1" :: "b" (i), "a" (vAttr << 8));

}

void setAttr(byte attr) {

	vAttr = attr;

}
