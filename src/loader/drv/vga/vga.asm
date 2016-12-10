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
