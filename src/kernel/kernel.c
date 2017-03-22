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

#include "arch.h"
#include "keyboard.h"
#include "pit.h"
#include "storage.h"
#include "vga.h"
#include "kernel.h"
#include "math.h"
#include "types.h"

byte kernelSize;
byte sysret[14];
char versionString[] =
"PSOS (Pretty Simple/Stupid Operating System) Development Build\n"
"Copyright (C) 2016 - 2017 Ben Stockett <thebenstockett@gmail.com>\n";

bool exec(mem16_t path);
void getRegs(mem16_t regs);
void initKernel();
void installISR(word num, mem16_t handler);
void panic(mem16_t reason);
void syscall(byte call, ...);

extern void brkptISR();
extern void syscallISR();

__attribute__((noreturn, section(".kmain"))) void kmain() {

	initKernel();
	puts("Loaded kernel.\n\n");
	puts(versionString);
	puts("\nReady...\n\n");

	if (!exec("SH.BIN")) panic("Could not locate SH.BIN");

	puts("System ready for shutdown\nPlease power off and remove boot medium");

	HANG();

}

void brkpt() {

	byte old = charAttr;

	charAttr = BG_WHITE | FG_YELLOW;
	clearText();

	puts("Breakpoint: press return to continue\n");

	struct regs regs;
	getRegs(&regs);

	puts("\nFree mem: ");
	putn((regs.sp - (kernelSize * 512)) / 1024, false);
	puts("KiB");

	while (!keyState[VK_RETURN])
		asm ("hlt");

	keyState[VK_RETURN] = false;

	charAttr = old;
	clearText();

}

bool exec(mem16_t path) {

	File file;

	bool result = openFile(path, &file);
	if (!result) return false;

	word segment = KERNEL_SEGMENT + (((1 + kernelSize) * 512) / 16);
	result = loadFile(&file, segment, 0);

	asm("mov ds, %0" :: "a" (segment));
	asm("mov ss, ax");

	asm("call 0x8E0:0");

}

void initKernel() {

	asm("mov es, %0" :: "a" (0x7C0));
	asm("mov %0, es:[0x1FD]" : "=r" (kernelSize)); //get kernel size

	installISR(3, brkptISR); //install breakpoint handler
	installISR(0x20, syscallISR); //install syscall handler

	initKeyboard();
	initPIT();
	initStorage();
	initVGA();

}

void installISR(word num, mem16_t handler) {

	num *= 4;

	asm("xor ax, ax");
	asm("mov es, ax");

	asm("mov es:[%0], %1" :: "b" (num), "a" (handler));
	asm("movw es:[bx + 2], %0" :: "i" (KERNEL_SEGMENT));

}

__attribute__((noreturn)) void panic(mem16_t reason) {

	charAttr = BG_RED;
	clearText();

	puts("KERNEL PANIC: ");
	puts(reason);

	HANG();

}

void syscall(byte call, ...) {

	word segment;
	asm("mov %0, ds" : "=r" (segment));

	asm("mov ds, %0" :: "a" (KERNEL_SEGMENT));

	byte subcall = call & 0xF;

	switch (call >> 4) {

		case 0:

			switch (subcall) {

				case 0:


				break;

			}

		break;

		case 1:

			switch (subcall) {

				case 0:

					asm("call clearText");

				break;

				case 1:

					asm("call puts");

				break;

			}

		break;

	}

	asm("mov ds, %0" :: "a" (segment));

}
