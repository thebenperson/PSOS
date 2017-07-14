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
#include "kbd.h"
#include "types.h"

/* ===== global variables ===== */

volatile uint16_t callback = NULL;
volatile uint16_t kTunnel = NULL;
bool keyState[87];

/*  ============================ static variables ============================ */

static char keyMap[] = {

	NULL,
	'1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', NULL,
	NULL, 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', NULL, NULL,
	'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', NULL, '\\',
	'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', NULL,

	'*', NULL, ' ', NULL,

	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,

	NULL, NULL,
	'7', '8', '9', '-',
	'4', '5', '6', '+',
	'1', '2', '3', '0', '.',

	NULL, NULL,

	NULL, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', NULL,
	NULL, 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', NULL, NULL,
	'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '\"', '~', NULL, '|',
	'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', NULL,

	NULL, NULL, NULL, NULL,

	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,

	NULL, NULL,
	NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL, NULL,

	NULL, NULL

};

/*
 *
 * lookup table for mapping scan codes to ASCII
 * NULL means that the scan code has no ASCII mapping
 *
 */

/* =========== global functions =========== */

extern void kbdISR(); // keyboard ISR handler

void initKBD() {

	kinstallISR(9, KERNEL_SEGMENT, kbdISR);

}

void kbdHandler() {

	uint8_t sc = inb(0x60); // PS/2 data port
	if (sc > 0xD8) return;

	/*
	 * return if multi-byte sequence
	 * refer to scan code set 1 for more info
	 *
	 */

	bool state = !(sc & 0x80); // bit 7 is set if keyup
	if (!state) sc ^= 0x80; // unset bit 7 if set

	sc--;

	/*
	 *
	 * the lowest scan code is 1
	 * arrays are zero based so
	 * everything needs to be lowered one
	 *
	 */

	keyState[sc] = state;

	if (!state) sc |= 0x80; // set bit 7 if keyup

	if (callback)
		asm("int 0x21" :: "b" ((uint32_t) callback), "a" ((uint32_t) sc));

	/*
	 *
	 * calling interrupt 0x21 with callback and val as parameters
	 * refer to kbd.h for more information
	 *
	 */

}

/* ============================== system calls ============================== */

bool kgetKey(uint8_t key) {

	return keyState[key];

}

void ksetCallback(uint16_t tunnel, uint16_t offset) {

	kTunnel = tunnel;
	callback = offset;

	kinstallISR(0x21, uSegment, kTunnel); //important: uSegment not this segment

	/*
	 *
	 * sets keyboard callback
	 * refer to kbd.h for more information
	 *
	 */

}

char ktoChar(uint8_t scanCode) {

	if (keyState[VK_LSHIFT] || keyState[VK_RSHIFT]) scanCode += 85;
	return keyMap[scanCode];

	/*
	 *
	 * converts scan code to ASCII
	 * capital characters start at offset 85 in lookup table
	 *
	 */

}
