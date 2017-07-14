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
#include "rtc.h"
#include "types.h"

/* static variables */

static bool bcd;
static bool military;

/* ===================== global functions ===================== */

void initRTC() {

	/*
	 *
	 * writing to port 0x70 tells the RTC what register we want
	 * reading from port 0x71 gives us the value in that register
	 *
	 */

	outb(0x70, 0xB); // select status register
	uint8_t val = inb(0x71);

	bcd = !(val & 0x4); // if BCD bit 2 is unset
	military = (val & 0x2); // if military bit 1 is set

}

/* ====================== static functions ====================== */

static uint8_t toBin(uint8_t val) {

	// converts BCD (binary coded decimals) to normal binary values
	return ((val / 16) * 10) + (val & 0xF);

}

/* ========================= system calls ========================= */

void kgetDate(uint16_t date, uint8_t mask) {

	INIT_REMOTE();

	uint8_t val;

	if (mask & T_YEAR) {

		outb(0x70, 0x9); // select year register
		val = inb(0x71);

		if (bcd) val = toBin(val);

		REMOTE();
		((Date*) date)->year = val;
		LOCAL();

	}

	if (mask & T_MONTH) {

		outb(0x70, 0x8); // select month register
		val = inb(0x71);

		if (bcd) val = toBin(val);

		REMOTE();
		((Date*) date)->month = val;
		LOCAL();

	}

	if (mask & T_DAY) {

		outb(0x70, 0x7); // select day register
		val = inb(0x71);

		if (bcd) val = toBin(val);

		REMOTE();
		((Date*) date)->day = val;
		LOCAL();

	}

	if (mask & T_HOUR) {

		outb(0x70, 0x4); // select hour register
		val = inb(0x71);

		bool pm = false;
		if (!military && (val & 0x80)) {

			// if not military and PM, bit 7 is set

			pm = true;
			val ^= 0x80; // unset bit 7

		}

		if (bcd) val = toBin(val);

		if (!military && pm) {

			// midnight is supposed to be 0 but in RTC format it is 12
			if (val == 12) val = 0;
			else val += 12;

		}

		REMOTE();
		((Date*) date)->hour = val;
		LOCAL();

	}

	if (mask & T_MIN) {

		outb(0x70, 0x2); // select minute register
		val = inb(0x71);

		if (bcd) val = toBin(val);

		REMOTE();
		((Date*) date)->min = val;
		LOCAL();

	}

	if (mask & T_SEC) {

		outb(0x70, 0x0); // select second register
		val = inb(0x71);

		if (bcd) val = toBin(val);

		REMOTE();
		((Date*) date)->sec = val;
		LOCAL();

	}

}
