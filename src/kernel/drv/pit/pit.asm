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

PITHandler:

	push ax

	mov al, [Timer]
	xor al, 0xFF

	mov byte [Timer], al

	mov al, 0x20
	out 0x20, al

	pop ax

iret

Sleep:

	push bp
	push ax
	push bx
	push cx
	push dx

	mov bh, [Timer]

	mov bp, sp
	mov ax, [bp + 0xC]

	xor dx, dx

	.Loop:

		hlt

		mov bl, bh
		mov bh, [Timer]

		mov cx, bx
		xor ch, cl
		cmp ch, 0xFF
		jne .Loop

		inc dx

		cmp dx, ax

	jne .Loop

	pop dx
	pop cx
	pop bx
	pop ax
	pop bp

ret

Timer: db 0x0
