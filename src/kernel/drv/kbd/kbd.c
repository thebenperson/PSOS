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

#include "types.h"
#include "kernel.h"
#include "kbd.h"

volatile mem16_t callback = NULL; //avoiding optimization in kexec
volatile mem16_t kTunnel = NULL; //avoiding optimization in kexec
bool keyState[87];
char keyMap[] = {

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

extern void kbdISR();

void initKBD() {

	kinstallISR(9, KERNEL_SEGMENT, kbdISR);

}

void kbdHandler() {

	byte scanCode = inb(0x60);
	if (scanCode > 0xD8) return;

	bool state = !(scanCode & 0x80);

	byte val = scanCode & 0x7F;
	val--;

	keyState[val] = state;

	if (!state) val |= 0x80;

	if (callback)
		asm("int 0x21" :: "b" ((dword) callback), "a" ((dword) val));

}

bool kgetKey(byte key) {

	return keyState[key];

}

void ksetCallback(mem16_t tunnel, mem16_t offset) {

	kTunnel = tunnel;
	callback = offset;

	kinstallISR(0x21, uSegment, kTunnel); //important: uSegment not this segment

}

char ktoChar(byte scanCode) {

	if (keyState[VK_LSHIFT] || keyState[VK_RSHIFT]) scanCode += 85;
	return keyMap[scanCode];

}
