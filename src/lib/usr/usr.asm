;PSOS Development Build
;https://github.com/TheBenPerson/PSOS/tree/dev

;Copyright (C) 2016 - 2017 Ben Stockett <thebenstockett@gmail.com>

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

SECTION .start
extern main

	mov bx, 1 ;syscall 1 (installISR)
	mov ecx, 0x21 ;interrupt 0x21
	mov esi, cs ;this segment
	mov edi, callback
	int 0x20 ;call installISR
	;user mode interrupt is now installed
	;allows kernel to call user mode functions

	call dword main ;start the program

	cli ;hang
	hlt

;ret

SECTION .text

global callback
callback:

	pushad

	mov cx, cs
	mov ds, cx
	mov ss, cx

	push eax
	call dword ebx
	add sp, 4

	mov ax, 0x7E0 ;change me plz...
	mov ds, ax
	mov ss, ax

	popad

iret
