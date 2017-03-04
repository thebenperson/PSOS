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

PrintChar:

	push bp
	push dx
	push ax
	push cx
	push bx

	mov bp, sp
	mov dl, [bp + 12]

	mov ax, 0xB800
	mov es, ax

	mov cx, [CharX]

	cmp dl, 0xA
	jne .Continue

		inc ch
		xor cl, cl

		jmp .Return

	.Continue:

	cmp cl, 80
	jne .Continue2

		inc ch
		xor cl, cl

	.Continue2:

	mov bx, cx
	mov al, 80
	mul bh
	xor bh, bh
	add bx, ax
	shl bx, 1

	mov byte [es:bx], dl

	inc cl

	.Return:

	mov [CharX], cx

	pop bx
	pop cx
	pop ax
	pop dx
	pop bp

ret

PrintString:

	push bp
	push si
	push ax

	mov bp, sp
	mov si, [bp + 8]

	.Loop:

		lodsb

		cmp al, 0x0
		jz .Return

		push ax
		call PrintChar
		add sp, 2

	jmp .Loop

	.Return:

	pop ax
	pop si
	pop bp

ret

SetCursor:

	push bp
	push cx
	push ax

	mov bp, sp
	cmp byte [bp + 8], 1
	jne .Disable

		mov cx, 0x0007
		jmp .Continue

	.Disable:

		mov cx, 0x0700

	.Continue:

	mov ah, 1
	int 0x10

	pop ax
	pop cx
	pop bp

ret

SetVideoMode:

	push bp
	push ax

	xor ah, ah

	mov bp, sp
	mov al, [bp + 6]

	int 0x10

	pop ax
	pop bp

ret

CharX: db 0x0
CharY: db 0x0
