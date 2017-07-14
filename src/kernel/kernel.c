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

#include "arch.h"
#include "kbd.h"
#include "pit.h"
#include "rtc.h"
#include "spkr.h"
#include "storage.h"
#include "vga.h"
#include "kernel.h"
#include "math.h"
#include "types.h"

/* ====== global variables ====== */

uint8_t kernelSize;
bool syscalled = false;
uint16_t uSegment = KERNEL_SEGMENT;

/* ======================= static variables ======================= */

static char versionString[] =
"PSOS (Pretty Simple/Stupid Operating System) Development Build\n"
"Copyright (C) 2016 - 2017 Ben Stockett <thebenstockett@gmail.com>\n";

/* ============================== global functions ============================== */

static void initKernel();
bool kexec(uint16_t path);
static void panic(uint16_t reason);

KENTRY void kmain() { // kernel entry point

	initKernel();
	kputs("Loaded kernel.\n\n");
	kputs(versionString);
	kputs("\nReady...\n\n");

	if(!kexec("SH.BIN")) panic("Could not locate SH.BIN");

	kputs("\n\nSystem ready for shutdown\nPlease power off and remove boot medium");
	HANG();

}

/* ============================= static functions ============================= */

static NORET void errOpcode() {

	asm("mov ax, cs");
	asm("mov ds, ax");
	asm("mov ss, ax");

	panic("Invalid Opcode");

}

extern void syscallISR(); // syscall handler

static void initKernel() {

	asm("mov es, %0" :: "r" (0x7C0));
	asm("mov %0, es:[0x1FD]" : "=r" (kernelSize)); //get kernel size

	kinstallISR(0x6, KERNEL_SEGMENT, errOpcode); //install invalid opcode handler
	kinstallISR(0x20, KERNEL_SEGMENT, syscallISR); //install syscall handler

	initKBD();
	initPIT();
	initRTC();
	initStorage();
	initVGA();

}

static NORET void panic(uint16_t reason) {

	ksetAttr(BG_RED);
	kclearText();

	syscalled = false;
	kputs("KERNEL PANIC: ");
	kputs(reason);

	ksetCursor(false);

	HANG();

}

/* ========================= system calls ========================= */

void kinstallISR(uint8_t num, uint16_t segment, uint16_t offset) {

	CLI();
	/*
	 *
	 * we do NOT want an interrupt halfway
	 * through installing a handler
	 *
	 */

	asm("mov es, %0" :: "r" (0));

	asm("mov es:[%0], %1" :: "b" (num * 4), "r" (offset));
	asm("movw es:[bx + 2], %0" :: "r" (segment));

	STI();

}

bool kexec(uint16_t path) {

	File file;

	bool result = open(path, &file);
	if (!result) return false;

	uint16_t tSegment = uSegment;
	uSegment += (((1 + kernelSize) * 512) / 16);

	result = loadFile(&file, uSegment, 0);
	if (result) {

		uint16_t tTunnel = kTunnel;

		uint16_t tCallback = callback;
		callback = NULL;

		kinstallISR(0x21, uSegment, 0); //maybe there's a better way?

		asm("mov ds, %0" :: "r" (uSegment));
		asm("mov ss, %0" :: "r" (uSegment));

		asm("pushad");
		asm("int 0x21");
		asm("popad");

		asm("mov ds, %0" :: "r" (KERNEL_SEGMENT));
		asm("mov ss, %0" :: "r" (KERNEL_SEGMENT));

		kTunnel = tTunnel;
		kinstallISR(0x21, tSegment, kTunnel); //reinstate kTunnel
		callback = tCallback;

	}

	uSegment = tSegment;
	return result;

}
