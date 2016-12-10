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
