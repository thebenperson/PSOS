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

#ifndef PSOS_VGA

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

/* == types == */

typedef struct {

	uint16_t x1;
	uint16_t y1;
	uint16_t x2;
	uint16_t y2;

} Line;

/* global functions */

extern void initVGA();

/* =========================== system calls =========================== */

extern bool ksetVGAMode(uint16_t width, uint16_t height, bool graphical);
extern void ksetAttr(uint8_t attr);
extern void kclearText();
extern void ksetCursor(bool enabled);
extern uint16_t ksetPosition(uint16_t position);
extern void kputs(uint16_t string);
extern void kputn(uint16_t num, bool hex);

#define PSOS_VGA
#endif
