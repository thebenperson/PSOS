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

#ifndef HG_Kernel_Storage_H

#include "types.h"

extern void initStorage();
extern bool loadFile(uint16_t file, uint16_t segment, uint16_t offset);
extern bool loadSector(uint8_t start, uint8_t length, uint16_t segment, uint16_t offset);
extern bool openFile(uint16_t path, uint16_t file);

typedef struct __attribute__((packed)) {

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

} FATEntry;

typedef struct {

	uint8_t attribute;
	uint16_t cluster;
	uint32_t size;

} File;

#define HG_Kernel_Storage_H
#endif
