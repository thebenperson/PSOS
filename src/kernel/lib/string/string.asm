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

StrCmp:

	push bp
	push si
	push di
	push cx

	mov bp, sp
	mov si, [bp + 12]

	push si
	call StrLen
	add sp, 0x2

	mov di, [bp + 10]
	mov bl, al

	push di
	call StrLen
	add sp, 0x2

	cmp bl, al
	je .Continue

		jmp .False

	.Continue:

	xor cl, cl

	.Loop:

		cmp cl, al
		je .True

		cmpsb
		jne .False

		inc cl

	jmp .Loop

	.True:

		mov al, 0x1
		jmp .Return

	.False:

		xor al, al

	.Return:

	pop cx
	pop di
	pop si
	pop bp

ret

StrLen:

	push bp
	push si
	push bx

	mov bp, sp
	mov si, [bp + 0x8]

	xor ah, ah

	.Loop:

		lodsb

		cmp al, 0x0
		je .Return

		inc ah

		jmp .Loop

	.Return:

	mov al, ah

	pop bx
	pop si
	pop bp

ret
