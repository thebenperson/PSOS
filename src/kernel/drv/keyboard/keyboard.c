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

#include "types.h"
#include "kernel.h"

void (*keyCallback)(byte) = 0;
byte keyState[87];
byte keyMap[] = {0x0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '+', 0x0, 0x0, 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', 0x0, 0x0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0x0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0x0, '*', 0x0, ' ', 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, '7', '8', '9', '-', '4', '5', '6', '+', '1', '2', '3', '0', '.'};

extern void keyboardISR();

void initKeyboard() {

	installISR(9, (ptr) &keyboardISR);

}

void keyboardHandler() {

	byte scanCode;
	asm("in %0, 0x60" : "=a" (scanCode)); //get scan code

	if (scanCode > 0xD8) return;

	bool value = !(scanCode & 0x80);

	scanCode <<= 1;
	scanCode >>= 1;

	keyState[--scanCode] = value;

	if (value && keyCallback) keyCallback(scanCode);

}
