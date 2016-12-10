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

%include "/drv/keyboard/keyboard.inc"
%include "/lib/string/string.asm"

%include "/prg/circle/circle.asm"
%include "/prg/clear/clear.asm"
%include "/prg/help/help.asm"
%include "/prg/vgatest/vgatest.asm"

AddChar:

	push bp
	push ax
	push bx

	mov bp, sp
	mov al, [bp + 0x8]

	mov bl, [BufferIndex]
	mov ah, bl
	xor bh, bh

	add bx, CommandBuffer

	mov [bx], al

	inc ah
	mov [BufferIndex], ah

	push ax
	call PrintChar
	add sp, 0x2

	pop bx
	pop ax
	pop bp

ret

Backspace:

	push ax
	push bx

	mov bl, [BufferIndex]
	cmp bl, 0x0
	je .Return

	dec bl
	xor bh, bh
	add bx, CommandBuffer
	mov byte [bx], 0x0

	mov al, [CharX]
	dec al
	mov ah, [CharY]
	push ax
	push 0x0
	call PutChar
	add sp, 0x4

	push -0x1
	call MoveIndex
	add sp, 0x2

	.Return:

	pop bx
	pop ax

ret

CLIKeyCallback:

	push bp
	push ax
	push bx

	mov bp, sp
	mov al, [bp + 0x8]

	cmp al, SC_BKSPACE
	jne .Else

		call Backspace
		jmp .Return

	.Else: cmp al, SC_LEFT
	jne .Else2

		push -0x1
		call MoveIndex
		add sp, 0x2

		jmp .Return

	.Else2: cmp al, SC_RIGHT
	jne .Continue

		push 0x1
		call MoveIndex
		add sp, 0x2

		jmp .Return

	.Continue:

	push ax
	call ScanCodeToAscii
	add sp, 0x2

	cmp al, 0xFF
	je .Return

	push ax
	call AddChar
	add sp, 0x2

	.Return:

	pop bx
	pop ax
	pop bp

ret

CLIMain:

	push Prompt
	call PrintString
	add sp, 0x2

	mov word [KeyCallback], CLIKeyCallback

	.Loop:

		hlt

		mov bx, KeyState
		add bx, SC_RETURN

		mov al, [bx]

		cmp al, 0x1
		jne .Else

			mov byte [bx], 0x0

			call Return

			jmp .Loop

		.Else:

		mov bx, KeyState
		add bx, SC_ESCAPE

		mov al, [bx]

		cmp al, 0x1

	jne .Loop

ret

MoveIndex:

	push bp
	push ax
	push bx

	mov bp, sp
	mov al, [bp + 0x8]
	mov ah, [BufferIndex]

	cmp al, 0x0
	jg .Greater

		cmp ah, 0x0
		je .Return

		jmp .Continue

	.Greater:

		mov bx, BufferIndex
		mov bl, [BufferIndex]
		xor bh, bh
		add bx, CommandBuffer

		mov bl, [bx]

		cmp bl, 0x0
		je .Return

		cmp ah, 0xFF
		je .Return

	.Continue:

	add ah, al

	mov [BufferIndex], ah

	mov ah, [CharX]
	add ah, al
	mov [CharX], ah

	mov al, ah
	mov ah, [CharY]

	push ax
	call MoveCursor
	add sp, 0x2

	.Return:

	pop bx
	pop ax
	pop bp

ret

ResetBuffer:

	push si

	mov si, CommandBuffer

	.Loop:

		cmp byte [si], 0x0

		je .Continue

		mov byte [si], 0x0
		inc si

	jmp .Loop

	.Continue:

	mov byte [BufferIndex], 0x0

	pop si

ret


Return:

	push ax

	push 0xA
	call PrintChar
	add sp, 0x2

	push CommandBuffer
	push CS_CLEAR
	call StrCmp
	add sp, 0x4

	cmp al, 0x1
	jne .Else

		call CLEAR
		jmp .Return

	.Else:

	push CommandBuffer
	push CS_VGATEST
	call StrCmp
	add sp, 0x4

	cmp al, 0x1
	jne .Else2

		call VGATEST
		jmp .Return

	.Else2:

	push CommandBuffer
	push CS_HELP
	call StrCmp
	add sp, 0x4

	cmp al, 0x1
	jne .Else3

		call HELP
		jmp .Return

	.Else3:

	push CommandBuffer
	push CS_CIRCLE
	call StrCmp
	add sp, 0x4

	cmp al, 0x1
	jne .Else4

		call CIRCLE
		jmp .Return

	.Else4:

		push CommandBuffer
		call PrintString
		add sp, 0x2

		push CLIError
		call PrintString
		add sp, 0x2

	.Return:

	call ResetBuffer

	push Prompt
	call PrintString
	add sp, 0x2

	pop ax

ret

BufferIndex: db 0x0
CommandBuffer: times 255 db 0x0
CLIError: db ": unrecognized command.", 0xA, "Please try again.", 0xA, 0xA, 0x0
Prompt: db "[stupid@PSOS ~]# ", 0x0
