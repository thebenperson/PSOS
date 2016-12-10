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

bool loadSector(byte start, byte length, word segment, word offset) {

	bool result;

	asm("mov ah, 2");
	asm("mov al, %0" :: "b" (length));
	asm("mov cl, %0" :: "b" (start));
	asm("xor ch, ch");
	asm("xor dh, dh");
	asm("mov dl, %0" :: "b" (drive));
	asm("mov es, %0" :: "b" (segment));
	asm("mov bx, %0" :: "b" (offset));
	asm("int 0x13"); //load sector

	asm("setc [%0]" :: "r" (&result)); //get result

	return result ? false : true;

}
