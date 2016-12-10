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

#include "drv/storage/storage.h"
#include "drv/vga/vga.h"
#include "types.h"
#include "kernel/kernel.h"

char* bootError = "Error loading kernel.\n"
"Please power off and remove boot medium.\n\n"
"   |\\      _,,,--,,_\n"
"  /,`.-'`'   ._  \\-;;,_\n"
" |,4-  ) )_   .;.(  `'-'\n"
"'---''(_/._)-'(_\\_)\n"
"2 tired 2 werk. come bak latr.\n";

__attribute__((noreturn)) void loaderMain() {

	bool result = loadSector(2, KERNEL_LENGTH, kernelMain, 0);

	if (result) {

		asm volatile("mov ds, %0" :: "a" (kernelMain));
		asm volatile("mov sp, 0xFFFF");

		asm volatile("jmp %0:0" :: "i" (kernelMain));

	} else {

		initVGA();
		printString(bootError);

		asm volatile("cli");
		asm volatile("hlt");

	}

}
