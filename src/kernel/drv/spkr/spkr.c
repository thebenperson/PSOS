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
#include "pit.h"
#include "types.h"

/* ============ system calls ============ */

void kbeep(uint16_t freq, uint16_t dur) {

	uint8_t val = inb(0x61);
	val |= 0x3;

	/*
	 *
	 * read value from port 0x61 of PS/2 controller
	 * and set bits 0 and 1
	 *
	 */

	if (freq != 0xFFFF) {

		// if freq = 0xFFFF disable speaker

		freq = 1193182 / freq;

		CLI();

		outb(0x43, 0xB6);
		outb(0x42, freq & 0xFF);
		outb(0x42, freq >> 8);

		/*
		 *
		 * port 0x43 is PIT mode/command register
		 * port 0x42 is PIT channel 2 data port
		 *
		 * outb(0x43, 0xB6); selects channel 2, sets access mode to low-hi, selects mode 3, and selects binary mode
		 * channel 2 is connected to PC speaker
		 * in mode 3, it outputs a square wave at a frequency of 1.193182 MHz / X MHz
		 * X is a word value set by two consecutive writes to the data port
		 *
		 */

		STI();

		outb(0x61, val);

		/*
		 *
		 * bits 0 and 1 are now set on port 0x61 of PS/2 controller
		 * PIT channel 2 is now connected to PC speaker
		 *
		 */

		if (dur == 0xFFFF) return; // don't disable speaker

		ksleep(dur);

	}

	outb(0x61, val ^ 0x3); // disable speaker

}
