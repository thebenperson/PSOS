/*

PSOS Development Build
Copyright (C) 2016 Ben Stockett.

This file is part of PSOS (Pretty Simple/Stupid Operating System).

PSOS is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

PSOS is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with PSOS.  If not, see <http://www.gnu.org/licenses/>.

*/

#include "vga/vga.h"
#include "kernel.h"
#include "types.h"

bool timer = false;

void pitISR();

void initPIT() {

	asm("cli");

	asm("out 0x43, %0" :: "a" (0x34));
	asm("out 0x40, %0" :: "a" (0xA9));
	asm("out 0x40, %0" :: "a" (0x4)); //initialize PIT

	installISR(8, (ptr) &pitISR); //install irq handler

	asm("sti");

}

void sleep(word mili) {

	word count = 0;
	bool lastTimer = timer;

	while (count != mili) {

		asm("hlt");

		if (timer != lastTimer) {

			lastTimer = timer;
			count++;

		}

	}

}
