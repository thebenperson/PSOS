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

word modes[2]; //far pointer
byte vAttr = BG_GREEN;
word vPort;
word vOffset = 0;

void scroll();

void initVGA() {

	word tvPort;

	asm("mov es, %0" :: "a" (0));
	asm("mov %0, es:[0x463]" : "=g" (tvPort)); //address 0x0:0x463 in BDA is base VGA vPort
	vPort  = tvPort;

	struct __attribute__((packed)) {

		char signature[4];
		word version;
		word oemString[2]; //far pointer
		byte caps[4];
		word modes[2]; //far pointer
		word mem;
		byte reserved[236];

	} vbeInfo;

	word result;

	asm("mov ax, ds");
	asm("mov es, ax");
	asm("int 0x10"
		: "=a" (result)
		: "a" (0x4F00), "D" (&vbeInfo));

	if (result == 0x4F) {

		modes[0] = vbeInfo.modes[0];
		modes[1] = vbeInfo.modes[1];

	} else modes[0] = 0xFFFF;

	ksetVGAMode(0xFFFF, 0xFFFF, false);
	kclearText();
	ksetCursor(false);

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

	byte base = hex ? 16 : 10;
	word tNum = num;

	byte place = 1;
	while (tNum >= base) {

		tNum /= base;
		place++;

	}

	char string[place + 1];
	string[place] = '\0';

	for (byte i = 0; i < place; i++) {

		byte value = (num % base) + 0x30;
		if (value > 0x39) value += 0x7;

		string[place - i - 1] = value;
		num /= base;

	}

	bool tSyscalled = syscalled;
	syscalled = false;
	if (hex) kputs("0x");
	kputs(string);
	syscalled = tSyscalled;

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

	outb(vPort, 0xF);
	outb(vPort + 1, position & 0xFF);
	outb(vPort, 0xE);
	outb(vPort + 1, position >> 8);

	return 0;

}

bool ksetVGAMode(word width, word height, bool graphical) {

	if (modes[0] == 0xFFFF) return false;

	word bestWidth = 0;
	word bestHeight = 0;

	word bestCharWidth = 0;
	word bestCharHeight = 0;

	word bestWidthMode = NULL;
	word bestHeightMode = NULL;

	word bestCharWidthMode = NULL;
	word bestCharHeightMode = NULL;
	word mode;

	for (;;) {

		asm("mov es, %0" :: "a" (modes[1]));
		asm("mov %0, es:[%1]"
			: "=g" (mode)
			: "b" (modes[0]++));

		if (mode == 0xFFFF) return false;

		struct __attribute__((packed)) {

			word attributes;
			byte winA;
			byte winB;
			word granularity;
			word winsize;
			word segmentA;
			word segmentB;
			word realFctPtr[2]; //far pointer
			word pitch;

			word width;
			word height;
			byte charWidth;
			byte charHeight;
			byte planes;
			byte bpp;
			byte banks;
			byte memory_model;
			byte bank_size;
			byte image_pages;
			byte reserved0;

			byte red_mask;
			byte red_position;
			byte green_mask;
			byte green_position;
			byte blue_mask;
			byte blue_position;
			byte rsv_mask;
			byte rsv_position;
			byte directcolor_attributes;

			dword physbase;
			byte reserved[217];

		} modeInfo;

		asm("mov ax, ds");
		asm("mov es, ax");

		word result;

		asm("int 0x10"
			: "=a" (result)
			: "a" (0x4F01), "c" (mode), "D" (&modeInfo));

		if (result != 0x4F) continue;

		if ((modeInfo.attributes & 0x10) != graphical) continue;

		if ((width != 0xFFFF) && (modeInfo.width != width)) continue;
		else {

			if (modeInfo.width > bestWidth) {

				bestWidth = modeInfo.width;
				bestWidthMode = mode;

			} else continue;

		}

		if ((height != 0xFFFF) && (modeInfo.height != height)) continue;
		else {

			if (modeInfo.height > bestHeight) {

				bestHeight = modeInfo.height;
				bestHeightMode = mode;

			} else continue;

		}

		if (!graphical) {

			if (modeInfo.charWidth < bestCharWidth) {

					bestCharWidth = modeInfo.charWidth;
					bestCharWidthMode = mode;

			} else continue;

			if (modeInfo.charHeight < bestCharHeight) {

					bestCharHeight = modeInfo.charHeight;
					bestCharHeightMode = mode;

			} else continue;

		}

		break;

	}

	if (width == 0xFFFF) mode = bestWidthMode;
	else if (height == 0xFFFF) mode = bestHeightMode;

	asm("int 0x10" :: "a" (0x4F02), "b" (mode));
	return true;

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
