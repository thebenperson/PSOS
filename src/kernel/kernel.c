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
#include "kbd.h"
#include "pit.h"
#include "storage.h"
#include "vga.h"
#include "kernel.h"
#include "math.h"
#include "types.h"

byte kernelSize;
bool syscalled = false;
word segment;
char versionString[] =
"PSOS (Pretty Simple/Stupid Operating System) Development Build\n"
"Copyright (C) 2016 - 2017 Ben Stockett <thebenstockett@gmail.com>\n";

bool kexec(mem16_t path);
void initKernel();
void installISR(word num, mem16_t handler);
void panic(mem16_t reason);

extern void syscallISR();

KENTRY void kmain() {

	initKernel();
	kputs("Loaded kernel.\n\n");
	kputs(versionString);
	kputs("\nReady...\n\n");

	if (!kexec("SH.BIN")) panic("Could not locate SH.BIN");

	kputs("System ready for shutdown\nPlease power off and remove boot medium");

	HANG();

}

void kbrkpt() {

	syscalled = false;

	byte old = charAttr;

	charAttr = BG_WHITE | FG_YELLOW;
	kclearText();

	kputs("Breakpoint: press return to continue\n");

	struct regs regs;
	getRegs(&regs);

	kputs("\nFree mem: ");
	kputn((regs.sp - (kernelSize * 512)) / 1024, false);
	kputs("KiB");

	while (!keyState[VK_RETURN])
		asm ("hlt");

	keyState[VK_RETURN] = false;

	charAttr = old;
	kclearText();

}

bool kexec(mem16_t path) {

	File file;

	bool result = openFile(path, &file);
	if (!result) return false;

	segment = KERNEL_SEGMENT + (((1 + kernelSize) * 512) / 16);
	result = loadFile(&file, segment, 0);

	if (!result) return;

	asm("xor eax, eax");
	asm("mov ds, %0" :: "a" (segment));
	asm("mov ss, ax");

	asm("push eax");
	asm("push 0");

	asm("retf");

}

void initKernel() {

	asm("mov es, %0" :: "r" (0x7C0));
	asm("mov %0, es:[0x1FD]" : "=r" (kernelSize)); //get kernel size

	installISR(0x20, syscallISR); //install syscall handler

	initKBD();
	initPIT();
	initStorage();
	initVGA();

}

void installISR(word num, mem16_t handler) {

	asm("mov es, %0" :: "r" (0));

	asm("mov es:[%0], %1" :: "b" (num * 4), "r" (handler));
	asm("movw es:[bx + 2], %0" :: "i" (KERNEL_SEGMENT));

}

__attribute__((noreturn)) void panic(mem16_t reason) {

	charAttr = BG_RED;
	kclearText();

	kputs("KERNEL PANIC: ");
	kputs(reason);

	HANG();

}
