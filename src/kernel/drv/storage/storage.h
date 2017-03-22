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
extern bool loadFile(mem16_t file, word segment, word offset);
extern bool loadSector(byte start, byte length, word segment, word offset);
extern bool openFile(mem16_t path, mem16_t file);

typedef struct __attribute__((packed)) {

	char name[11];
	byte attribute;
	byte reserved1;
	byte creationSec;
	word creationTime;
	word creationDate;
	word lastAccess;
	word reserved2;
	word modTime;
	word modDate;
	word cluster;
	dword size;

} FATEntry;

typedef struct {

	byte attribute;
	word cluster;
	dword size;

} File;

#define HG_Kernel_Storage_H
#endif
