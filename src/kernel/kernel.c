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

#include "drv/keyboard/keyboard.h"
#include "drv/pit/pit.h"
#include "drv/vga/vga.h"
#include "types.h"

char* versionString = "PSOS (Pretty Simple/Stupid Operating System) Development Build\nBen Stockett - 2016\n";

extern void breakpointISR();
void initKernel();
void installISR(word interrupt, ptr handler);
void commandLine(byte);

__attribute__((noreturn)) void kernelMain() {

	initKernel();

	attribute = FG_WHITE;
	printString("Loaded kernel.\n\n");
	printString(versionString);
	printString("\nReady.\n\n");

	attribute = FG_GREEN;
	printString(">>");
	keyCallback = &commandLine;

	for (;;) asm("hlt");

}

void commandLine(byte scanCode) {

	switch (scanCode) {

		case VK_LEFT:

			charX--;

		break;

		case VK_RIGHT:

			charX++;

		break;

		case VK_RETURN:

			charY++;
			charX = 0;

			printString(">>");
			charY /= 0;

		break;

		default:

			printChar(keyMap[scanCode]);

		break;

	}

}

void breakpointHandler() {

	byte oldAttribute = attribute;

	attribute = BG_WHITE | FG_YELLOW;
	clearText();

	printString("------------------------------------\n");
	printString("Breakpoint: press escape to continue\n");
	printString("------------------------------------\n\n");

	while (!keyState[VK_RETURN]) asm("hlt");

	attribute = oldAttribute;
	clearText();

}

__attribute__((noreturn)) void errDivideByZero() {

	asm("cli");

	attribute = BG_RED;
	clearText();

	printString("---------------------------------------\n");
	printString("Fatal error: division by zero at 0x????\n");
	printString("---------------------------------------");

	asm("hlt");

}

void initKernel() {

	installISR(0, (ptr) &errDivideByZero);
	installISR(3, (ptr) &breakpointISR);

	initPIT();
	initKeyboard();
	initVGA();

}

void installISR(word interrupt, ptr handler) {

	asm("xor ax, ax");
	asm("mov es, ax");
	asm("movw es:[%0], %1" :: "b" (interrupt * 4), "a" (handler));
	asm("movw es:[bx + 2], cs"); //install IRQ

}
