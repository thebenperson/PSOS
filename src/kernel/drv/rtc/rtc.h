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

#ifndef PSOS_RTC

#include "types.h"

/* === macros === */

#define T_YEAR 0x1
#define T_MONTH 0x2
#define T_DAY 0x4
#define T_HOUR 0x8
#define T_MIN 0x10
#define T_SEC 0x20
#define T_ALL 0x3F

/* === types === */

typedef struct {

	uint8_t year;
	uint8_t month;
	uint8_t day;
	uint8_t hour;
	uint8_t min;
	uint8_t sec;

} Date;

/* global functions */

extern void initRTC();

/* =============== system calls =============== */

extern void kgetDate(uint16_t date, uint8_t mask);

#define PSOS_RTC
#endif
