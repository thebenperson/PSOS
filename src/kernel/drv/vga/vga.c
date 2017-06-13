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

uint16_t *modes = NULL;
uint8_t vAttr = BG_GREEN;
uint16_t vPort;
uint16_t vOffset = 0;

struct __attribute__((packed)) {

	char signature[4];
	uint16_t version;
	uint32_t oemString; //far pointer
	uint8_t caps[4];
	uint32_t modes; //far pointer
	uint16_t mem;
	uint8_t reserved[236];

} vbeInfo;

void scroll();
void setBank(uint16_t bank);

void initVGA() {

	uint16_t tvPort;

	asm("mov es, %0" :: "a" (0));
	asm("mov %0, es:[0x463]" : "=g" (tvPort)); //address 0x0:0x463 in BDA is base VGA vPort
	vPort  = tvPort;

	uint16_t result;

	asm("mov ax, ds");
	asm("mov es, ax");
	asm("int 0x10"
		: "=a" (result)
		: "a" (0x4F00), "D" (&vbeInfo));

	if (result == 0x4F) modes = vbeInfo.modes & 0xFFFF;

	kclearText();
	ksetCursor(false);

}

void kclearText() {

	asm("mov es, %0" :: "r" (0xB800));

	for (uint16_t i = 0; i < (80 * 25 * 2); i += 2) {

		asm("movw es:[%0], %1" :: "b" (i), "g" (vAttr << 8));

	}

	vOffset = 0;

}

void kdraw(uint16_t shape) {

	INIT_REMOTE();

	REMOTE();
	volatile uint16_t x1 = ((Line*) shape)->x1;
	volatile uint16_t y1 = 480 - ((Line*) shape)->y1;

	volatile uint16_t x2 = ((Line*) shape)->x2;
	volatile uint16_t y2 = 480 - ((Line*) shape)->y2;
	LOCAL();

	asm("fninit");

	float dX = x2 - x1;
	float dY = y2 - y1;

	float len = sqrt(pow(dX, 2) + pow(dY, 2));
	float cX = dX / len;
	float cY = dY / len;

	asm("mov es, %0" :: "a" (0xA000));

	uint8_t lastBank = 0;
	for (uint16_t i = 0; i < len; i++) {

		uint16_t x = (i * cX) + x1;
		uint16_t y = (i * cY) + y1;

		int offset = ((y * 640) + x) * 3;

		uint8_t bank = offset / (1024 * 64);
		offset %= (1024 * 64);

		if (bank != lastBank) {

			setBank(bank);
			lastBank = bank;

		}

		asm("mov es:[%0], %1" :: "b" (offset), "a" ((uint8_t) 0x55));
		asm("mov es:[%0], %1" :: "b" (offset + 1), "a" (i));

	}

	setBank(0);

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

void kputn(uint16_t num, bool hex) {

	uint8_t base = hex ? 16 : 10;
	uint16_t tNum = num;

	uint8_t place = 1;
	while (tNum >= base) {

		tNum /= base;
		place++;

	}

	char string[place + 1];
	string[place] = '\0';

	for (uint8_t i = 0; i < place; i++) {

		uint8_t value = (num % base) + 0x30;
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

void kputs(uint16_t string) {

	INIT_REMOTE();

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

	asm("int 0x10" :: "a" (1 << 8), "c" ((uint16_t) (enabled ? 0x7 : 0x700)));

}

uint16_t ksetPosition(uint16_t position) {

	if (position == 0xFFFF) return vOffset;

	vOffset = position;
	position /= 2;

	outb(vPort, 0xF);
	outb(vPort + 1, position & 0xFF);
	outb(vPort, 0xE);
	outb(vPort + 1, position >> 8);

	return 0;

}

bool ksetVGAMode(uint16_t width, uint16_t height, bool graphical) {

	if (!modes) return false;

	uint16_t mode;
	uint8_t i = 0;

	asm("mov ax, ds");
	asm("mov es, ax");

	for (;;) {

		mode = modes[i++];
		if (mode == 0xFFFF) return false;

		struct __attribute__((packed)) {

			uint16_t attributes;
			uint8_t winA;
			uint8_t winB;
			uint16_t granularity;
			uint16_t winsize;
			uint16_t segmentA;
			uint16_t segmentB;
			uint32_t realFctPtr; //far pointer
			uint16_t pitch;

			uint16_t width;
			uint16_t height;
			uint8_t charWidth;
			uint8_t charHeight;
			uint8_t planes;
			uint8_t bpp;
			uint8_t banks;
			uint8_t memory_model;
			uint8_t bank_size;
			uint8_t image_pages;
			uint8_t reserved0;

			uint8_t red_mask;
			uint8_t red_position;
			uint8_t green_mask;
			uint8_t green_position;
			uint8_t blue_mask;
			uint8_t blue_position;
			uint8_t rsv_mask;
			uint8_t rsv_position;
			uint8_t directcolor_attributes;

			uint32_t physbase;
			uint8_t reserved[212];

		} modeInfo;

		uint16_t result;

		asm("int 0x10"
			: "=a" (result)
			: "a" (0x4F01), "c" (mode), "D" (&modeInfo));

		if (result != 0x4F) continue;

		//if (graphical && (modeInfo.attributes & 0x80)) continue; //no linear frame buffer
		if (!(modeInfo.attributes & 0x10) == graphical) continue;
		if (modeInfo.bpp != 24) continue;

		if (modeInfo.width != width) continue;
		if (modeInfo.height != height) continue;

		break;

	}

	uint16_t result;

	asm("int 0x10"
		: "=a" (result)
		: "a" (0x4F02), "b" (mode));

	return (result == 0x4F);

}

void scroll() {

	asm("mov es, %0" :: "r" (0xB800));

	for (uint16_t i = 0; i < (80 * 24 * 2); i += 2) {

		asm("movw ax, es:[%0 + 160]" :: "b" (i));
		asm("movw es:[bx], ax");

	}

	for (uint16_t i = (80 * 24 * 2); i < (80 * 25 * 2); i += 2)
		asm("mov es:[%0], %1" :: "b" (i), "a" (vAttr << 8));

}

void setAttr(uint8_t attr) {

	vAttr = attr;

}

void setBank(uint16_t bank) {

	asm("int 0x10" :: "a" (0x4F05), "b" ((uint16_t) 0), "d" (bank));

}
