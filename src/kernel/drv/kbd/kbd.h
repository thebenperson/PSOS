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

#ifndef HG_Kernel_Keyboard_H

#define VK_ESC 0x0
#define VK_1 0x1
#define VK_2 0x2
#define VK_3 0x3
#define VK_4 0x4
#define VK_5 0x5
#define VK_6 0x6
#define VK_7 0x7
#define VK_8 0x8
#define VK_9 0x9
#define VK_0 0xA
#define VK_MINUS 0xB
#define VK_PLUS 0xC
#define VK_BKSPACE 0xD
#define VK_TAB 0xE
#define VK_Q 0xF
#define VK_W 0x10
#define VK_E 0x11
#define VK_R 0x12
#define VK_T 0x13
#define VK_Y 0x14
#define VK_U 0x15
#define VK_I 0x16
#define VK_O 0x17
#define VK_P 0x18
#define VK_LBRACKET 0x19
#define VK_RBRACKET 0x1A
#define VK_RETURN 0x1B
#define VK_LCTRL 0x1C
#define VK_A 0x1D
#define VK_S 0x1E
#define VK_D 0x1F
#define VK_F 0x20
#define VK_G 0x21
#define VK_H 0x22
#define VK_J 0x23
#define VK_K 0x24
#define VK_L 0x25
#define VK_COLON 0x26
#define VK_QUOTE 0x27
#define VK_TILDE 0x28
#define VK_LSHIFT 0x29
#define VK_BKSLASH 0x2A
#define VK_Z 0x2B
#define VK_X 0x2C
#define VK_C 0x2D
#define VK_V 0x2E
#define VK_B 0x2F
#define VK_N 0x30
#define VK_M 0x31
#define VK_COMMA 0x32
#define VK_PERIOD 0x33
#define VK_UPSLASH 0x34
#define VK_RSHIFT 0x35
#define VK_POUND 0x36
#define VK_LALT 0x37
#define VK_SPACE 0x38
#define VK_CABSLOCK 0x39
#define VK_F1 0x3A
#define VK_F2 0x3B
#define VK_F3 0x3C
#define VK_F4 0x3D
#define VK_F5 0x3E
#define VK_F6 0x3F
#define VK_F7 0x40
#define VK_F8 0x41
#define VK_F9 0x42
#define VK_F10 0x43
#define VK_NMLOCK 0x44
#define VK_SCLOCK 0x45
#define VK_KP7 0x46
#define VK_KP8 0x47
#define VK_KP9 0x48
#define VK_KPMINUS 0x49
#define VK_KP4 0x4A
#define VK_KP5 0x4B
#define VK_KP6 0x4C
#define VK_KPPLUS 0x4D
#define VK_KP1 0x4E
#define VK_KP2 0x4F
#define VK_KP3 0x50
#define VK_KP0 0x51
#define VK_KPPERIOD 0x52
#define VK_F11 0x53
#define VK_F12 0x54

#define VK_LEFT VK_KP4
#define VK_RIGHT VK_KP6
#define VK_UP VK_KP8
#define VK_DOWN VK_KP2

#include "types.h"

extern volatile bool keyState[87];

extern void initKeyboard();

#define HG_Kernel_Keyboard_H
#endif
