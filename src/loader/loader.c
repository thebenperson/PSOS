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

#include "drv/storage/storage.h"
#include "drv/vga/vga.h"
#include "types.h"
#include "kernel/kernel.h"

char bootError[] = "Error loading kernel.\n"
"Please power off and remove boot medium.\n\n"
"   |\\      _,,,--,,_\n"
"  /,`.-'`'   ._  \\-;;,_\n"
" |,4-  ) )_   .;.(  `'-'\n"
"'---''(_/._)-'(_\\_)\n"
"2 tired 2 werk. come bak latr.";

void loaderMain() {

	bool result = loadSector(2, KERNEL_SIZE, KERNEL_SEGMENT, 0);

	if (result) {

		#asm

			mov ax, #KERNEL_SEGMENT
			mov ds, ax

			mov sp, #0xFFFF

			jmp KERNEL_SEGMENT:0

		#endasm

	}

	initVGA();
	printString(bootError);

	#asm

		cli
		hlt

	#endasm

}
