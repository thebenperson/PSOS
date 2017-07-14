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
#include "types.h"

/* == global variables == */

volatile uint16_t time = 0;

/* ============================================ global functions ============================================ */

extern void pitISR();

void initPIT() {

	CLI();

	outb(0x43, 0x34);
	outb(0x40, 0xA9);
	outb(0x40, 0x4);

	/*
	 *
	 * initializes PIT to send an ISR every ms
	 * port 0x43 is PIT mode/command register
	 * port 0x40 is PIT channel 0 data port
	 *
	 * outb(0x43, 0x34); selects channel 0, sets access mode to low-hi, selects mode 2, and selects binary mode
	 * channel 0 is connected to IRQ0, and in mode 2, generates an ISR at a frequency of 1.193182 MHz / X MHz
	 * X is a word value set by two consecutive writes to the data port
	 * this sets X to 0x4A9 = 1193
	 * 1.193182 MHz / 1193 MHz = 0.001 MHz, or 1 ms
	 * the PIT will now send an ISR every ms
	 *
	 */

	kinstallISR(8, KERNEL_SEGMENT, &pitISR); // IRQ0 is mapped to interrupt 8

	STI();

}

/* ========== syscalls ========== */

void ksleep(uint16_t mili) {

	uint16_t lastTime = time;

	while ((time - lastTime) < mili)
		HLT();

}
