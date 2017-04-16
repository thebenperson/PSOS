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

#ifndef HG_Kernel_VGA_H

#include "types.h"

#define ATTR_BLINK 0x80
#define ATTR_BOLD 0x80

#define BG_BLUE 0x10
#define BG_CYAN 0x30
#define BG_GREEN 0x20
#define BG_MAGENTA 0x50
#define BG_RED 0x40
#define BG_WHITE 0x70
#define BG_YELLOW 0x60

#define FG_BLUE 0x1
#define FG_CYAN 0x3
#define FG_GREEN 0x2
#define FG_MAGENTA 0x5
#define FG_RED 0x4
#define FG_WHITE 0x7
#define FG_YELLOW 0x6

#define CLEAR_TEXT 0x0
#define PRINT_STRING 0x1
#define SCROLL 0x2
#define SET_ATTR 0x3
#define SET_POS 0x4
#define SET_CURSOR 0x5
#define SET_MODE 0x6

extern byte charAttr;
extern byte charX;
extern byte charY;

extern void kclearText();
extern void initVGA();
extern void kputn(int num, bool hex);
extern void kputs(mem16_t string);
extern void ksetCursor(bool enabled);
extern void ksetPosition(word position);
extern void ksetVGAMode(byte mode);

#define HG_Kernel_VGA_H
#endif
