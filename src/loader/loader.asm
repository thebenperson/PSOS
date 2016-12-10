;PSOS Version 1.0
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
[ORG 0x7C00]

%include "/kernel/kernel.inc"

xor ax, ax
mov ds, ax

mov ax, 0x7000
mov ss, ax

mov sp, 0xFFFF

%include "/drv/storage/init.asm"

push 0x3
call SetVideoMode
add sp, 2

push 0
call SetCursor
add sp, 2

push 2
push KernelLength
push 0x0
push KernelMain
call ReadSector
add sp, 8

cmp ax, 1
je Continue

	push BootError
	call PrintString
	add sp, 2

	cli
	hlt

Continue:

%include "/drv/storage/install.asm"

jmp KernelMain

%include "/drv/storage/storage.asm"
%include "/drv/vga/vga.asm"

BootError: db "Error loading kernel.", 0xA, "Please power off and remove boot medium.", 0xA, 0xA
db "   |\      _,,,--,,_", 0xA
db "  /,`.-'`'   ._  \-;;,_", 0xA
db " |,4-  ) )_   .;.(  `'-'", 0xA
db "'---''(_/._)-'(_\_)", 0xA
db "2 tired 2 werk. come bak latr.", 0xA, 0x0

times 510 - ($ - $$) db 0

dw 0xAA55
