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

#ifndef HG_LIB_SYSCALL_H

#include "types.h"

extern word syscall(byte call, word arg1, word arg2, word arg3);
extern void tunnel();

//core syscalls
static inline bool exec(mem16_t path) { return syscall(0, path, 0, 0); }
static inline void installISR(byte num, word segment, word offset) { syscall(1, num, segment, offset); }

//keyboard syscalls
static inline bool getKey(byte key) { return syscall(2, key, 0, 0); }
static inline void setCallback(mem16_t callback) { syscall(3, tunnel, callback, 0); }
static inline char toChar(byte key) { return syscall(4, key, 0, 0); }

//pit syscalls
static inline void sleep(word milli) { syscall(5, milli, 0, 0); }

//rtc syscalls
static inline void getDate(mem16_t date, byte mask) { syscall(6, date, mask, 0); }

//spkr syscalls
static inline void beep(size_t freq, size_t dur) { syscall(7, freq, dur, 0); }

static inline void setSound(size_t freq) { syscall(7, freq, 0xFFFF, 0); } //same syscall as beep
static inline void soundOff() { syscall(7, 0xFFFF, 0, 0); } //same syscall as beep

//vga syscalls
static inline void clearText() { syscall(8, 0, 0, 0); }
static inline void putc(char c) { syscall(9, c, 0, 0); }
static inline void putn(size_t num, bool hex) { syscall(10, num, hex, 0); }
static inline void puts(mem16_t string) { syscall(11, string, 0, 0); }
static inline void setCursor(bool enabled) { syscall(12, enabled, 0, 0); }
static inline void setPosition(word pos) { syscall(13, pos, 0, 0); }
static inline bool setVGAMode(word width, word height, bool graphical) { syscall(14, width, height, graphical); }
static inline void setAttr(byte attr) { syscall(15, attr, 0, 0); }

static inline word getPosition() { return syscall(13, 0xFFFF, 0, 0); } //same syscall as setPosition

#define HG_LIB_SYSCALL_H
#endif
