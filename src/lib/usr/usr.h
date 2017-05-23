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

extern void tunnel();

//core syscalls
#define brkpt() syscall(0, 0, 0, 0)
#define exec(w) syscall (1, w, 0, 0)
#define installISR(b, w1, w2) syscall(2, b, w1, w2)

//keyboard syscalls
#define getKey(b) syscall(3, b, 0, 0)
#define setCallback(w) syscall(4, tunnel, w, 0)
#define toChar(b) syscall(5, b, 0, 0)

//pit syscalls
#define sleep(w) syscall(6, w, 0, 0)

//vga syscalls
#define clearText() syscall(7, 0, 0, 0)
#define putc(b) syscall(8, b, 0, 0)
#define putn(w, b) syscall(9, w, b, 0)
#define puts(w) syscall(10, w, 0, 0)
#define setCursor(b) syscall(11, b, 0, 0)
#define setPosition(w) syscall(12, w, 0, 0)
#define setAttr(b) syscall(13, b, 0, 0)

#define getPosition() setPosition(0xFFFF)

extern word syscall(byte call, word arg1, word arg2, word arg3);

#define HG_LIB_SYSCALL_H
#endif
