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
