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

#include "kernel.h"
#include "storage.h"
#include "string.h"
#include "types.h"
#include "vga.h"

#define ENTRIES 256
#define FAT_SIZE 3
#define ROOT_SIZE (ENTRIES * 32) / 512

volatile byte drive;

volatile byte tracks;
volatile byte sectors;
volatile byte heads;

void initStorage() {

	asm("mov es, %0" :: "a" (0x7C0));
	asm("mov %0, es:[0x1FE]" : "=r" (drive)); //get drive number from loader

	asm("mov ah, 0x8");
	asm("int 0x13" :: "d" (drive)); //get emulated drive geometry

	volatile word cx;
	asm("mov %0, cx" : "=r" (cx));
	asm("mov %0, dh" : "=r" (heads));

	tracks = (cx >> 6) + 1;

	sectors = (cx << 10) >> 10;
	heads++;

}

bool loadSector(byte start, byte length, word segment, word offset) {

	byte track = start / (heads * sectors);
	byte head = (start / sectors) % heads;
	byte sector = (start % sectors) + 1; //LBA to CHS translation

	bool result;

	asm("mov es, %0" :: "a" (segment));

	asm("int 0x13" :: "c" ((track << 6) | sector), "a" ((0x2 << 8) | length), "b" (offset), "d" ((head << 8) | drive));
	asm("setc %0" : "=r" (result));

	return !result;

}

bool loadFile(mem16_t file, word segment, word offset) {

	return loadSector(1 + kernelSize + FAT_SIZE + ROOT_SIZE + ((File*) file)->cluster, 1, segment, offset);

}

bool openFile(mem16_t path, mem16_t file) {

	for (byte i = 0; ((char*) path)[i]; i++) {

		if (((char*) path)[i] == '.') {

			((char*) path)[i] = ((char*) path)[i + 1];
			((char*) path)[i + 1] = ((char*) path)[i + 2];
			((char*) path)[i + 2] = ((char*) path)[i + 3];
			((char*) path)[i + 3] = '\0';

			break;

		}

	}

	byte root[ROOT_SIZE * 512];

	bool result = loadSector(1 + kernelSize + FAT_SIZE, ROOT_SIZE, KERNEL_SEGMENT, &root);
	if (!result) return false;

	FATEntry *entry = root;

	while (entry->name[0]) {

		for (byte i = 0; i < 8; i++) {

			if (entry->name[i] == ' ') {

				entry->name[i++] = entry->name[8];
				entry->name[i++] = entry->name[9];
				entry->name[i++] = entry->name[10];
				entry->name[i] = '\0';

				break;

			}

		}

		if (strcmp(entry->name, path)) {

			((File*) file)->attribute = entry->attribute;
			((File*) file)->cluster = entry->cluster - 2;
			((File*) file)->size = entry->size;

			return true;

		}

		entry = (FATEntry*) (((mem16_t) entry) + 32);

	}

	return false;

}
