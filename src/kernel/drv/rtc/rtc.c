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

#include "kernel.h"
#include "rtc.h"
#include "types.h"

bool bcd;
bool military;

void initRTC() {

	uint8_t val;

	outb(0x70, (0x80 | 0xB));
	val = inb(0x71);

	bcd = !(val & 0x4);
	military = (val & 0x2);

}

void kgetDate(uint16_t date, uint8_t mask) {

	uint16_t tSegment = syscalled ? uSegment : KERNEL_SEGMENT;

	uint8_t val;

	if (mask & T_YEAR) {

		outb(0x70, (0x80 | 0x9));
		val = inb(0x71);

		if (bcd) val = ((val / 16) * 10) + (val & 0xF);

		REMOTE();
		((Date*) date)->year = val;
		LOCAL();

	}

	if (mask & T_MONTH) {

		outb(0x70, (0x80 | 0x8));
		val = inb(0x71);

		if (bcd) val = ((val / 16) * 10) + (val & 0xF);

		REMOTE();
		((Date*) date)->month = val;
		LOCAL();

	}

	if (mask & T_DAY) {

		outb(0x70, (0x80 | 0x7));
		val = inb(0x71);

		if (bcd) val = ((val / 16) * 10) + (val & 0xF);

		REMOTE();
		((Date*) date)->day = val;
		LOCAL();

	}

	if (mask & T_HOUR) {

		outb(0x70, (0x80 | 0x4));
		val = inb(0x71);

		bool pm = false;
		if (!military && (val & 0x80)) {

			pm = true;
			val ^= 0x80;

		}

		if (bcd) val = ((val / 16) * 10) + (val & 0xF);

		if (!military && pm) {

			if (val == 12) val = 0;
			else val += 12;

		}

		REMOTE();
		((Date*) date)->hour = val;
		LOCAL();

	}

	if (mask & T_MIN) {

		outb(0x70, (0x80 | 0x2));
		val = inb(0x71);

		if (bcd) val = ((val / 16) * 10) + (val & 0xF);

		REMOTE();
		((Date*) date)->min = val;
		LOCAL();

	}

	if (mask & T_SEC) {

		outb(0x70, (0x80 | 0x0));
		val = inb(0x71);

		if (bcd) val = ((val / 16) * 10) + (val & 0xF);

		REMOTE();
		((Date*) date)->sec = val;
		LOCAL();

	}

}
