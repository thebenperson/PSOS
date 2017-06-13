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

#ifndef PSOS_KERNEL_H

#include "types.h"

#define KENTRY __attribute__((noreturn, section(".kmain")))

#define INIT_REMOTE() uint16_t tSegment = syscalled ? uSegment : KERNEL_SEGMENT;
#define REMOTE() asm("mov ds, %0" :: "a" (tSegment));
#define LOCAL() asm("mov ds, %0" :: "a" (KERNEL_SEGMENT));

extern uint8_t kernelSize;
extern bool syscalled;
extern uint16_t uSegment;

extern void kinstallISR(uint8_t num, uint16_t segment, uint16_t offset);

static inline uint8_t inb(uint16_t port) {

	uint8_t val;
	asm("in %0, %1"
		: "=a" (val)
		: "d" (port));

	return val;

}

static inline void outb(uint16_t port, uint8_t val) {

	asm("out %0, %1" :: "Nd" (port), "a" (val));

}

#define PSOS_KERNEL_H
#endif
