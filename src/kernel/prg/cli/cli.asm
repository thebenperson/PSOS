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

%include "/drv/keyboard/keyboard.inc"
%include "/lib/string/string.asm"

%include "/prg/circle/circle.asm"
%include "/prg/clear/clear.asm"
%include "/prg/fstest/fstest.asm"
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

	mov ax, [CharX]
	dec al

	push 0x0
	push ax
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

	push 1
	call SetCursor
	add sp, 2

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
	push CS_FSTEST
	call StrCmp
	add sp, 0x4

	cmp al, 0x1
	jne .Else5

		call FSTEST
		jmp .Return

	.Else5:

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
