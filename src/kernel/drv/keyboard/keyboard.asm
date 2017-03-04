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
