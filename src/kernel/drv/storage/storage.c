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

#include "kernel.h"
#include "storage.h"
#include "string.h"
#include "types.h"

#define ROOT_SIZE (ENTRIES * 32) / 512

/* static variables */

static uint8_t drive;

static uint8_t heads;
static uint8_t sectors;

/* ================================= global functions ================================= */

void initStorage() {

	asm("mov es, %0" :: "r" (0x7C0));
	asm("mov %0, es:[0x1FE]" : "=r" (drive)); //get drive number from loader

	asm("mov ah, 0x8");
	asm("int 0x13"
		:
		: "d" (drive)
		: "cc", "ah", "cx", "bl", "di"); //get emulated drive geometry

	/*
	 *
	 * int 0x13; ah = 0x8: read drive parameters
	 *
	 * arguments:	dl - drive
	 *
	 * returns:		cf - set on error
	 *				ah - return code
	 *				dl - number of drives
	 *				dh - number of heads - 1
	 *				cx - sectors per track - 1 and number of tracks
	 *				bl - drive type
	 *				es:di - parameter table
	 *
	 */

	uint8_t cl;
	asm("mov %0, cl" : "=g" (cl));
	asm("mov %0, dh" : "=g" (heads));

	sectors = cl & 0x3F; // first 6 bits of cx are sectors per track
	heads++;

}

static bool loadSector(uint8_t start, uint8_t length, uint16_t segment, uint16_t offset);

bool open(uint16_t path, uint16_t file) {

	uint8_t root[ROOT_SIZE * 512]; // load root dir

	bool result = loadSector(1 + kernelSize + FAT_SIZE, ROOT_SIZE, KERNEL_SEGMENT, &root);
	if (!result) return false;

	INIT_REMOTE();

	char name[11]; // path format is 8.3

	uint8_t d = 0;
	uint8_t s = 0;
	bool ext = false;

	/*
	 *
	 * internally, FAT16 filenames are 11 characters long
	 * these filenames are capitalized
	 * the first 8 are the name
	 * the last 3 are the extension
	 * if the name is less than 8, the remainder is padded with spaces
	 * the following converts a "normal" filename into a FAT16 one
	 * ex: "sh.bin" -> "SH      BIN"
	 *
	 */

	for (;;) {

		REMOTE();
		char c = ((char*) path)[d++];
		LOCAL();

		if (!c) break;

		if (c == '.') {

			ext = true;
			continue;

		}

		if ((c >= 'a') && (c <= 'z')) c -= 32; // to uppercase

		if (ext) {

			for (; s < 8; s++)
				name[s] = ' ';

		}

		name[s++] = c;

	}

	// structure of a FAT16 file entry
	struct __attribute__((packed)) {

		char name[11];
		uint8_t attribute;
		uint8_t reserved1;
		uint8_t creationSec;
		uint16_t creationTime;
		uint16_t creationDate;
		uint16_t lastAccess;
		uint16_t reserved2;
		uint16_t modTime;
		uint16_t modDate;
		uint16_t cluster;
		uint32_t size;

	} *entry = (uint16_t) root;

	for (uint16_t i = 0; i < ENTRIES; i++) {

		if (strncmp(entry->name, name, 11)) {

			((File*) file)->attribute = entry->attribute;
			((File*) file)->cluster = entry->cluster - 2; // cluster numbers start at 2

			((File*) file)->size = entry->size;

			return true;

		}

		entry = (((uint16_t) entry) + 32); // entries are 32 bytes

	}

	return false;

}

bool loadFile(uint16_t file, uint16_t segment, uint16_t offset) {

	// load FAT into memory
	uint16_t fat[(FAT_SIZE * 512) / 2];
	bool result = loadSector(1 + kernelSize, FAT_SIZE, KERNEL_SEGMENT, &fat);
	if (!result) return false;

	uint16_t base = 1 + kernelSize + FAT_SIZE + ROOT_SIZE; //offset to data area
	uint16_t cluster = ((File*) file)->cluster;

	do {

		result = loadSector(base + cluster, 1, segment, offset);
		if (!result) return false;

		cluster = fat[cluster + 2] - 2; // cluster numbers start at 2
		offset += 512;

	} while (cluster != 0xFFFD); //0xFFFF - 2 = 0xFFFD

	return true;

}

/* ========================================= static functions ========================================= */

static bool loadSector(uint8_t start, uint8_t length, uint16_t segment, uint16_t offset) {

	// convert LBA to CHS
	uint8_t track = start / (heads * sectors);
	uint8_t head = (start / sectors) % heads;
	uint8_t sector = (start % sectors) + 1;

	bool result;

	asm("mov es, %0" :: "r" (segment));
	asm("int 0x13"
		:
		: "c" ((track << 6) | sector), "a" ((0x2 << 8) | length), "b" (offset), "d" ((head << 8) | drive)
		: "cc");

	asm("setc %0" : "=g" (result));

	/*
	 *
	 * int 0x13; ah = 0x2: read sectors
	 *
	 * arguments: 	al - number of sectors
	 *				ch - cylinder
	 *				cl - sector
	 *				dh - head
	 *				dl - drive
	 *				es - segment
	 *				bx - offset
	 *
	 * returns: 	cf - set on error
	 *				ah - return code
	 *				al - sectors read
	 *
	 */


	return !result;

}
