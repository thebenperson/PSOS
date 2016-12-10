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

KeyboardHandler:

	push ax
	push bx

		xor ah, ah
		in al, 0x60

		cmp al, 0xD8
		ja .Return

			test al, 0x80
			jnz .SetReleased

				push ax

					add ax, KeyState
					mov bx, ax
					mov byte [bx], 0x1

				pop ax

				mov bx, [KeyCallback]
				cmp bx, 0x0
				jz .Return

				push ax
				call bx
				add sp, 0x2

			jmp .Return

			.SetReleased:

				sub ax, 0x80
				add ax, KeyState
				mov bx, ax
				mov byte [bx], 0x0

		.Return:

		mov al, 0x20
		out 0x20, al

	pop bx
	pop ax

iret

ScanCodeToAscii:

	push bp
	push bx

		mov bp, sp
		mov al, [bp + 0x6]

		mov bl, al
		sub bl, 0x2

		cmp bl, 0x38
		jbe .Continue

			mov al, 0xFF
			jmp .Return

		.Continue:

		xor bh, bh

		add bx, ScanCodes

		mov al, [bx]

		.Return:

	pop bx
	pop bp

ret

KeyCallback: dw 0x0
KeyState: times 0x58 db 0x0
ScanCodes: db "1234567890-=", 0xFF, 0xFF, "QWERTYUIOP[]", 0xFF, 0xFF, "ASDFGHJKL;'`", 0xFF, "\ZXCVBNM,./", 0xFF, '*', 0xFF, ' '
