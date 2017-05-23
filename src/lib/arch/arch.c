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
#include "types.h"

void cpuid(mem16_t string) {

	asm("cpuid" :
		"=b" (((dword*) string)[0]),
		"=d" (((dword*) string)[1]),
		"=c" (((dword*) string)[2])
		: "a" ((dword) 0));

}

void getRegs(mem16_t regs) {

	asm("mov %0, ax" : "=r" (((struct regs*) regs)->ax));
	asm("mov %0, bx" : "=r" (((struct regs*) regs)->bx));
	asm("mov %0, cx" : "=r" (((struct regs*) regs)->cx));
	asm("mov %0, dx" : "=r" (((struct regs*) regs)->dx));

	asm("mov %0, bp" : "=r" (((struct regs*) regs)->bp));
	asm("mov %0, sp" : "=r" (((struct regs*) regs)->sp));

	asm("mov %0, es" : "=r" (((struct regs*) regs)->es));
	asm("mov %0, cs" : "=r" (((struct regs*) regs)->cs));
	asm("mov %0, ss" : "=r" (((struct regs*) regs)->ss));
	asm("mov %0, ds" : "=r" (((struct regs*) regs)->ds));
	asm("mov %0, fs" : "=r" (((struct regs*) regs)->fs));
	asm("mov %0, gs" : "=r" (((struct regs*) regs)->gs));

}
