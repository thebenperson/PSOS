/*

PSOS Development Build
https://github.com/TheBenPerson/PSOS/tree/dev

Copyright (C) 2016 Ben Stockett <thebenstockett@gmail.com>

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

#include "drv/keyboard/keyboard.h"
#include "drv/pit/pit.h"
#include "drv/storage/storage.h"
#include "drv/vga/vga.h"
#include "kernel.h"
#include "types.h"

char versionString[] = "PSOS (Pretty Simple/Stupid Operating System) Development Build\nBen Stockett - 2016\n";

extern void breakpointISR();
extern void sysCallISR();

bool exec(char* file);
void initKernel();
void installISR(word intNum, void* handler);
void errDivideByZero();

void main() {

	char file[512];
	bool result;

	initKernel();
	charAttr = FG_RED;
	printString("Loaded kernel.\n\n");
	printString(versionString);
	printString("\nReady.\n\n");

	charAttr = FG_GREEN;
	printString(">>test.txt:\n\n");

	result = loadSector(5, 1, KERNEL_SEGMENT, file);

	if (result)	printString(file);
	else printString("Error loading file.");

	#asm

		cli
		hlt

	#endasm

}

void breakpointHandler() {

	byte oldCharAttr = charAttr;

	charAttr = BG_WHITE | FG_YELLOW;
	clearText();

	printString("------------------------------------\n");
	printString("Breakpoint: press escape to continue\n");
	printString("------------------------------------\n\n");

	while (!keyState[VK_ESCAPE]) {

		#asm

			hlt

		#endasm

	}

	charAttr = oldCharAttr;
	clearText();

}

void errDivideByZero() {

	#asm

		cli

	#endasm

	charAttr = BG_RED;
	clearText();

	printString("---------------------------------------\n");
	printString("Fatal error: division by zero at 0x????\n");
	printString("---------------------------------------");

	#asm

		hlt

	#endasm

}

/*bool exec(char* file) {

	//readFile(file, );

	return true;

}*/

void initKernel() {

	installISR(0, errDivideByZero);
	installISR(3, breakpointISR);

	initKeyboard();
	initPIT();
	initVGA();

}

void installISR(word intNum, void* handler) {

	intNum *= 4;

	#asm

		xor ax, ax
		mov es, ax

		mov bx, [bp + 4]
		mov ax, [bp + 6]

		seg es
		mov [bx], ax

		seg es
		mov [bx + 2], #KERNEL_SEGMENT

	#endasm

}

void sysCallHandler(byte driver, byte function) {

	/*switch (driver) {

		case STORAGE:

			switch (function) {



			}

		break;

		case VGA:

			switch (function) {


			}

		break;

	}*/

}
