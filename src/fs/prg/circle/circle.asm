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

%include "/loader/loader.inc"
%include "/kernel/kernel.inc"

CIRCLE:

	mov bp, sp
	sub sp, 8

	fld dword [Radius]
	fld dword [Step]
	fldz

	push 0x13
	call SetVideoMode
	add sp, 0x2

	xor cx, cx

	.Loop:

		fld st0
		fcos
		fmul st3
		fistp dword [bp - 0x8]

		fld st0
		fsin
		fmul st3
		fistp dword [bp - 0x4]

		fadd st1

		add dword [bp - 0x8], 160
		add dword [bp - 0x4], 100

		push word [bp - 0x8]
		push word [bp - 0x4]
		push 127
		call SetPixel
		add sp, 0x6

		cmp cx, 0xFFFF
		je .Return

		inc cx

	jmp .Loop

	.Return:

	push 5000
	call Sleep
	add sp, 0x2

	push 0x3
	call SetVideoMode
	add sp, 0x2

	call CLEAR

	add sp, 0x8

ret

CS_CIRCLE: db "CIRCLE", 0x0
Radius: dd 50.0
Step: dd 1.0
