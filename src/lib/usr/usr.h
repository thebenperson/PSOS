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

#define brkpt() syscall(0, 0, 0, 0)
#define installISR(b, w1, w2) syscall(1, b, w1, w2)

#define getKey(b) syscall(2, b, 0, 0)
#define setCallback(w) syscall(3, w, 0, 0)
#define toChar(b) syscall(4, b, 0, 0)

#define sleep(w) syscall(5, w, 0, 0)

#define clearText() syscall(6, 0, 0, 0)
#define putc(b) syscall(7, b, 0, 0)
//putn is mising
#define puts(w) syscall(9, w, 0, 0)
#define setCursor(b) syscall(10, b, 0, 0)

extern word syscall(byte call, word arg, word arg2, word arg3);

#define HG_LIB_SYSCALL_H
#endif
