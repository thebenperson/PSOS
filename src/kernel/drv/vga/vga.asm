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

%include "/drv/vga/vga.inc"

ClearScreen:

	push es
	push ax
	push bx

	mov ax, 0xB800
	mov es, ax

	xor bx, bx
	mov al, [Color]

	.Loop:

		cmp bx, 0x1040
		je .Return

		mov byte [es:bx], 0x0
		mov [es:bx + 1], al

		add bx, 0x2

	jmp .Loop

	.Return:

	pop bx
	pop ax
	pop es

ret

MoveCursor:

	push bp
	push ax
	push bx

	mov ah, 0x2
	xor bh, bh

	mov bp, sp
	mov dx, [bp + 0x8]

	int 10h

	pop bx
	pop ax
	pop bp

ret

PrintChar:

	push bp
	push ax
	push bx

	mov bp, sp
	mov al, [bp + 8]

	mov bx, [CharX]

	cmp al, 0xA
	jne .Continue

		inc bh
		xor bl, bl

		jmp .Continue3

	.Continue:

	push ax
	push bx
	call PutChar
	add sp, 4

	inc bl

	cmp bl, 80
	jne .Continue3

		inc bh
		xor bl, bl

	.Continue3:

	cmp bh, 25
	jne .Return

		call ScrollConsole

		dec bh
		xor bl, bl

	.Return:

	push bx
	call MoveCursor
	add sp, 2

	mov [CharX], bx

	pop bx
	pop ax
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

PutChar:

	push ax
	push es
	push bx
	push bp

	mov ax, 0xB800
	mov es, ax

	mov bp, sp
	mov bx, [bp + 10]

	mov al, 80
	mul bh

	xor bh, bh
	add bx, ax

	shl bx, 1

	mov bp, sp
	mov al, [bp + 12]

	mov ah, [Color]

	mov [es:bx], al
	mov [es:bx + 1], ah

	pop bp
	pop bx
	pop es
	pop ax

ret

ScrollConsole:

	push ax
	push es
	push ds
	push si
	push di

	mov ax, 0xB800
	mov es, ax
	mov ds, ax

	mov si, 160
	xor di, di

	.Loop:

		movsd

	cmp di, 0xFA0
	jne .Loop

	pop di
	pop si
	pop ds
	pop es
	pop ax

ret

SetColor:

	push bp
	push ax

	mov bp, sp
	mov al, [bp + 6]

	mov [Color], al

	pop ax
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

SetPixel:

	push ax
	push es
	push bp
	push bx

	mov ax, 0xA000
	mov es, ax

	mov bp, sp
	mov ax, [bp + 12]

	mov bx, 320
	mul bx

	add ax, [bp + 14]
	mov bx, ax

	mov al, [bp + 10]

	mov [es:bx], al

	pop bx
	pop bp
	pop es
	pop ax

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

CharX: db 0
CharY: db 0
Color: db FG_GREEN
