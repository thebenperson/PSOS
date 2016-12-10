;PSOS Development Build
;Copyright (C) 2016 Ben Stockett.

;This file is part of PSOS (Pretty Simple/Stupid Operating System).

;PSOS is free software: you can redistribute it and/or modify
;it under the terms of the GNU General Public License as published by
;the Free Software Foundation, either version 3 of the License, or
;(at your option) any later version.

;PSOS is distributed in the hope that it will be useful,
;but WITHOUT ANY WARRANTY; without even the implied warranty of
;MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;GNU General Public License for more details.

;You should have received a copy of the GNU General Public License
;along with PSOS.  If not, see <http://www.gnu.org/licenses/>.

[BITS 16]

jmp 0x7C0:start ;set code segment to 0x7C0

start:

mov ax, cs
mov ds, ax ;set up data segment

mov ax, 0x50
mov ss, ax
mov sp, 0xFFFF ;set up initial stack space

%include "drv/storage/init.asm" ;save drive number
