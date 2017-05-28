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
#include "rtc.h"
#include "storage.h"
#include "vga.h"
#include "kernel.h"
#include "math.h"
#include "types.h"

byte kernelSize;
bool syscalled = false;
word uSegment = KERNEL_SEGMENT;
char versionString[] =
"PSOS (Pretty Simple/Stupid Operating System) Development Build\n"
"Copyright (C) 2016 - 2017 Ben Stockett <thebenstockett@gmail.com>\n";

bool kexec(mem16_t path);
void initKernel();
void panic(mem16_t reason);

extern void syscallISR();

KENTRY void kmain() {

	initKernel();
	kputs("Loaded kernel.\n\n");
	kputs(versionString);
	kputs("\nReady...\n\n");

	if(!kexec("SH.BIN")) panic("Could not locate SH.BIN");

	kputs("\n\nSystem ready for shutdown\nPlease power off and remove boot medium");
	HANG();

}

bool kexec(mem16_t path) {

	File file;

	bool result = openFile(path, &file);
	if (!result) return false;

	word tSegment = uSegment;
	uSegment += (((1 + kernelSize) * 512) / 16);

	result = loadFile(&file, uSegment, 0);
	if (result) {

		word tTunnel = cTunnel;

		word tCallback = callback;
		callback = NULL;

		kinstallISR(0x21, uSegment, 0); //maybe there's a better way?

		asm("mov ds, %0" :: "r" (uSegment));
		asm("mov ss, %0" :: "r" (uSegment));

		asm("pushad");
		asm("int 0x21");
		asm("popad");

		asm("mov ds, %0" :: "r" (KERNEL_SEGMENT));
		asm("mov ss, %0" :: "r" (KERNEL_SEGMENT));

		callback = tCallback;
		cTunnel = tTunnel;

		kinstallISR(0x21, tSegment, cTunnel); //reinstate tunnel

	}

	uSegment = tSegment;
	return result;

}

void initKernel() {

	asm("mov es, %0" :: "r" (0x7C0));
	asm("mov %0, es:[0x1FD]" : "=r" (kernelSize)); //get kernel size

	kinstallISR(0x20, KERNEL_SEGMENT, syscallISR); //install syscall handler

	initKBD();
	initPIT();
	initRTC();
	initStorage();
	initVGA();

}

void kinstallISR(byte num, mem16_t segment, mem16_t offset) {

	asm("mov es, %0" :: "r" (0));

	asm("mov es:[%0], %1" :: "b" (num * 4), "r" (offset));
	asm("movw es:[bx + 2], %0" :: "r" (segment));

}

__attribute__((noreturn)) void panic(mem16_t reason) {

	vAttr = BG_RED;
	kclearText();

	kputs("KERNEL PANIC: ");
	kputs(reason);

	HANG();

}
