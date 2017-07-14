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

#ifndef HG_LIB_SYSCALL_H

#include "types.h"

extern uint16_t syscall(uint8_t call, uint16_t arg1, uint16_t arg2, uint16_t arg3);
extern void tunnel();

//core syscalls
static inline bool exec(uint16_t path) { return syscall(0, path, 0, 0); }
static inline void installISR(uint8_t num, uint16_t segment, uint16_t offset) { syscall(1, num, segment, offset); }

//keyboard syscalls
static inline bool getKey(uint8_t key) { return syscall(2, key, 0, 0); }
static inline void setCallback(uint16_t callback) { syscall(3, tunnel, callback, 0); }
static inline char toChar(uint8_t key) { return syscall(4, key, 0, 0); }

//pit syscalls
static inline void sleep(uint16_t milli) { syscall(5, milli, 0, 0); }

//rtc syscalls
static inline void getDate(uint16_t date, uint8_t mask) { syscall(6, date, mask, 0); }

//spkr syscalls
static inline void beep(uint16_t freq, uint16_t dur) { syscall(7, freq, dur, 0); }

static inline void setSound(uint16_t freq) { syscall(7, freq, 0xFFFF, 0); } //same syscall as beep
static inline void soundOff() { syscall(7, 0xFFFF, 0, 0); } //same syscall as beep

//vga syscalls
static inline void clearText() { syscall(8, 0, 0, 0); }
static inline void draw(uint16_t shape) { syscall(9, shape, 0, 0); }
static inline void putc(char c) { syscall(10, c, 0, 0); }
static inline void putn(uint16_t num, bool hex) { syscall(11, num, hex, 0); }
static inline void puts(uint16_t string) { syscall(12, string, 0, 0); }
static inline void setCursor(bool enabled) { syscall(13, enabled, 0, 0); }
static inline void setPosition(uint16_t pos) { syscall(14, pos, 0, 0); }
static inline bool setVGAMode(uint16_t width, uint16_t height, bool graphical) { return syscall(15, width, height, graphical); }
static inline void setAttr(uint8_t attr) { syscall(16, attr, 0, 0); }

static inline uint16_t getPosition() { return syscall(14, 0xFFFF, 0, 0); } //same syscall as setPosition

#define HG_LIB_SYSCALL_H
#endif
