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

byte drive;

dword cylinders;
dword heads;
double sectors;
dword sectorsPerTrack;

void initStorage() {

	byte result[30];

	asm volatile("mov ah, 0x48");
	asm volatile("mov dl, %0" :: "r" (drive));
	asm volatile("mov si, %0" :: "r" ((word) &result));
	asm volatile("int 0x13"); //get disk geometry

	cylinders = result[4];
	heads = result[8];
	sectors = result[16];
	sectorsPerTrack = result[12];

}

bool loadSector(byte start, byte length, word dest, word offset) {

	asm volatile("mov ah, 2");
	asm volatile("mov al, %0" :: "b" (length));
	asm volatile("mov cl, %0" :: "b" (start));
	asm volatile("xor ch, ch");
	asm volatile("xor dh, dh");
	asm volatile("mov dl, %0" :: "b" (drive));
	asm volatile("mov bx, cs");
	asm volatile("mov es, bx");
	asm volatile("mov bx, %0" :: "b" (dest));
	asm volatile("int 0x13"); //load sector

	bool result;
	asm volatile("setc [%0]" :: "r" (&result)); //get result

	return result ? false : true;

}
