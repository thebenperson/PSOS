/*

PSOS Development Build
Copyright (C) 2016 Ben Stockett.

This file is part of PSOS (Pretty Simple/Stupid Operating System).

PSOS is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

PSOS is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with PSOS.  If not, see <http://www.gnu.org/licenses/>.

*/

#ifndef HG_Loader_VGA_H

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
#define FG_MAGENTA 0xA
#define FG_RED 0x4
#define FG_WHITE 0x7
#define FG_YELLOW 0x6

extern byte attribute;
extern word charX;
extern word charY;

extern void clearText();
extern void initVGA();
extern void printString(ptr string);
extern void setCursor(bool enabled);
extern void setVGAMode(byte mode);

#define HG_Loader_VGA_H
#endif
