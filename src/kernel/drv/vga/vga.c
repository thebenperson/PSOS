/*
 *
 * PSOS Development Build
 * https://github.com/TheBenPerson/PSOS/tree/dev
 *
 * Copyright (C) 2016 - 2017 Ben Stockett <thebenstockett@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */

#include "kernel.h"
#include "math.h"
#include "types.h"
#include "vga.h"

/* ======== static variables ======== */

static uint16_t port; // vga base port
static uint16_t *modes = NULL; // fixme?
static uint8_t attr = BG_GREEN;
static uint16_t vOffset = 0;

static struct __attribute__((packed)) {

	char signature[4];
	uint16_t version;
	uint32_t oemString; //far pointer
	uint8_t caps[4];
	uint32_t modes; //far pointer
	uint16_t mem;
	uint8_t reserved[236];

} vbeInfo;

/* ================ global functions ================ */

void initVGA() {

	// get base VGA port from BDA
	asm("mov es, %0" :: "a" (0));
	asm("mov %0, es:[0x463]" : "=r" (port));

	uint16_t result;

	asm("mov ax, ds");
	asm("mov es, ax");
	asm("int 0x10"
		: "=a" (result)
		: "a" (0x4F00), "D" (&vbeInfo));

	/*
	 *
	 * int 0x10; ax = 0x4F00: get VBE info
	 *
	 * arguments:	es:di - pointer to VBEInfo
	 *
	 * returns:		ax - return value (0x4F on success)
	 *
	 */

	if (result == 0x4F) modes = vbeInfo.modes & 0xFFFF;

	kclearText();
	ksetCursor(false);

}

/* ================================== static functions ================================== */

static void scroll() {

	asm("mov es, %0" :: "r" (0xB800));

	for (uint16_t i = 0; i < (80 * 24 * 2); i += 2) {

		/*
		 *
		 * for every word of video memory, copy the one from the next row down into this one
		 * I use words instead of bytes because it is more efficient
		 * one word is a character and its attribute, so it makes more sense too
		 *
		 */

		asm("movw ax, es:[%0 + 160]" :: "b" (i));
		asm("movw es:[bx], ax");

	}

	/*
	 *
	 * the loop above skips the last row
	 * who knows what is outside of video memory so we shouldn't copy it
	 * instead we should initialize it manually
	 *
	 */

	for (uint16_t i = (80 * 24 * 2); i < (80 * 25 * 2); i += 2)
		asm("mov es:[%0], %1" :: "b" (i), "a" (attr << 8));

}

static void setBank(uint16_t bank) {

	asm("int 0x10" :: "a" (0x4F05), "b" ((uint16_t) 0), "d" (bank));

	/*
	 *
	 * int 0x10; ax = 0x4F05: get/set VGA bank
	 *
	 * arguments:	bh - set/get bank
	 * 				bl - window number
	 * 				dx - bank number
	 *
	 * returns: 	ax - return value (0x4F on success)
	 * 				dx - bank number (for get only)
	 *
	 */

}

/* ============================= system calls ============================= */

bool ksetVGAMode(uint16_t width, uint16_t height, bool graphical) {

	// return if VBE unsupported
	if (!modes) return false;

	uint16_t mode;
	uint8_t i = 0;

	asm("mov ax, ds");
	asm("mov es, ax");

	for (;;) {

		mode = modes[i++];

		// return if no more modes
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

		/*
		 *
		 * int 0x10; ax = 0x4F01: get mode info
		 *
		 * arguments:	cx - mode number
		 * 				es:di - pointer to ModeInfo
		 *
		 * returns:		ax - return value (0x4F on success)
		 *
		 */

		if (result != 0x4F) continue;

		// next mode if mode doesn't meet requirements
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

	/*
	 *
	 * int 0x10; ax = 0x4F02: set VBE mode
	 *
	 * arguments:	bx - mode number
	 * 				es:di - pointer to CRTCInfo (used if bit 11 of bx is set)
	 *
	 * returns:		ax: return value (0x4F on success)
	 *
	 */

	return (result == 0x4F);

}

void ksetAttr(uint8_t new) {

	attr = new;

}

void kclearText() {

	asm("mov es, %0" :: "r" (0xB800));

	for (uint16_t i = 0; i < (80 * 25 * 2); i += 2) {

		// initialize every word of video memory
		asm("movw es:[%0], %1" :: "b" (i), "g" (attr << 8));

	}

	vOffset = 0;

}

void ksetCursor(bool enabled) {

	outb(port, 0x9); // select max scan line
	uint8_t max = inb(port + 1) & 0x1F; // only first 5 bits

	outb(port, 0xA); // select cursor start
	outb(port + 1, !enabled * max);
	outb(port, 0xB); // select cursor end
	outb(port + 1, enabled * max);

	// if start > end cursor is disabled

}

uint16_t ksetPosition(uint16_t position) {

	// this system call can get and set the cursor position
	if (position == 0xFFFF) return vOffset / 2;

	vOffset = position * 2;

	outb(port, 0xF); // select cursor low
	outb(port + 1, position & 0xFF);
	outb(port, 0xE); // select cursor hi
	outb(port + 1, position >> 8);

	return NULL;

}

void kputs(uint16_t string) {

	INIT_REMOTE();

	asm("mov es, %0" :: "r" (0xB800));

	for (;;) {

		REMOTE();
		char c = *((char*) string);
		LOCAL();

		if (!c) {

			ksetPosition(vOffset / 2);
			return;

		}

		if (vOffset >= (80 * 25 * 2)) {

			scroll();
			vOffset = (80 * 24 * 2);

		}

		if (c == '\n') {

			// next row down first column
			vOffset += 160 - (vOffset % 160);

		} else {

			asm("mov es:[%0], %1" :: "b" (vOffset), "r" (c));
			asm("mov es:[bx + 1], %0" :: "r" (attr));

			vOffset += 2;

		}

		string++;

	}

}

void kputc(char c) {

	asm("mov es, %0" :: "a" (0xB800));

	if (vOffset >= (80 * 25 * 2)) {

		scroll();
		vOffset = (80 * 24 * 2);

	}

	if (c == '\n') {

		// next row down first column
		vOffset += 160 - (vOffset % 160);

	} else {

		asm("mov es:[%0], %1" :: "b" (vOffset), "r" (c));
		asm("mov es:[bx + 1], %0" :: "r" (attr));

		vOffset += 2;

	}

	ksetPosition(vOffset / 2);

}

void kputn(uint16_t num, bool hex) {

	uint8_t base = hex ? 16 : 10;
	uint16_t tNum = num;

	uint8_t digits = 1;
	while (tNum >= base) {

		tNum /= base;
		digits++;

		// find out how many digits

	}

	char string[digits + 1];

	for (uint8_t i = 0; i < digits; i++) {

		char value = (num % base) + 0x30; // to ASCII
		if (value > 0x39) value += 0x7; // to hex if digit > 9

		string[digits - 1 - i] = value;
		num /= base; // get next digit

	}

	string[digits] = '\0';

	// tell kputs the data is in KERNEL_SEGMENT
	bool tSyscalled = syscalled;
	syscalled = false;

	if (hex) kputs("0x");
	kputs(string);

	syscalled = tSyscalled;

}

// still in progress (FIXME)
void kdraw(uint16_t shape) {

	INIT_REMOTE();

	REMOTE();
	volatile uint16_t x1 = ((Line*) shape)->x1;
	volatile uint16_t y1 = 480 - ((Line*) shape)->y1;

	volatile uint16_t x2 = ((Line*) shape)->x2;
	volatile uint16_t y2 = 480 - ((Line*) shape)->y2;
	LOCAL();

	// left and down are zero

	asm("fninit"); // might not be needed

	float dX = x2 - x1;
	float dY = y2 - y1;

	float len = sqrt(pow(dX, 2) + pow(dY, 2)); // A^2 + B^2 = C^2
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

		asm("mov es:[%0], %1" :: "b" (offset), "a" ((uint8_t) 0xFF));
		asm("mov es:[%0], %1" :: "b" (offset + 1), "a" ((uint8_t) 0xFF));
		asm("mov es:[%0], %1" :: "b" (offset + 2), "a" ((uint8_t) 0x0));

	}

}
