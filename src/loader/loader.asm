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
