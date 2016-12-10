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
