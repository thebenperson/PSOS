;PSOS Version 1.0
;https://github.com/TheBenPerson/PSOS

;Copyright (C) 2016 Ben Stockett <thebenstockett@gmail.com>

;Permission is hereby granted, free of charge, to any person obtaining a copy
;of this software and associated documentation files (the "Software"), to deal
;in the Software without restriction, including without limitation the rights
;to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
;copies of the Software, and to permit persons to whom the Software is
;furnished to do so, subject to the following conditions:

;The above copyright notice and this permission notice shall be included in all
;copies or substantial portions of the Software.

;THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
;IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
;FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
;AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
;LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
;OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
;SOFTWARE.

%define ATTR_BLINK 10000000b
%define ATTR_BOLD 10000000b

%define BG_BLUE 00010000b
%define BG_CYAN 00110000b
%define BG_GREEN 00100000b
%define BG_MAGENTA 01010000b
%define BG_RED 01000000b
%define BG_WHITE 01110000b
%define BG_YELLOW 01100000b

%define FG_BLUE 00000001b
%define FG_CYAN 00000011b
%define FG_GREEN 00000010b
%define FG_MAGENTA 00001010b
%define FG_RED 00000100b
%define FG_WHITE 00000111b
%define FG_YELLOW 00000110b
