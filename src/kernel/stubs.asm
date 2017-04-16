;PSOS Development Build
;https://github.com/TheBenPerson/PSOS/tree/dev

;Copyright (C) 2016 - 2017 Ben Stockett <thebenstockett@gmail.com>

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

[BITS 16]

SECTION .text

extern segment
extern syscalled
global syscallISR

syscallISR: ;syscall handler

	sti ;enable interrupts
	pushad ;save registers

	mov ax, cs
	mov ds, ax ;ds = KERNEL_SEGMENT

	pop dword [regs]
	pop dword [regs + 4]
	pop dword [regs + 8]
	pop dword [regs + 12]
	pop dword [regs + 16]
	pop dword [regs + 20]
	pop dword [regs + 24]
	pop dword [regs + 28]

	mov byte [syscalled], 1 ;syscalled = true

	pop dword [retinfo]
	pop word [retinfo + 4] ;now arguments are at the top of the stack

	pop ebx
	shl ebx, 2 ;call *= 4

	call dword [callTable + ebx]
	mov fs, ax

	push ebx

	push word [retinfo + 4]
	push dword [retinfo]

	mov byte [syscalled], 0 ;syscalled = false

	push dword [regs + 28] ;restore registers
	push dword [regs + 24]
	push dword [regs + 20]
	push dword [regs + 16]
	push dword [regs + 12]
	push dword [regs + 8]
	push dword [regs + 4]
	push dword [regs]

	mov ax, [segment]
	mov ds, ax ;restore segment

	popad

iret

SECTION .data

callTable:

	;core syscalls

	extern kbrkpt
	dd kbrkpt

	;keyboard syscalls

	extern kgetKey
	dd kgetKey

	extern ksetCallback
	dd ksetCallback

	extern ktoChar
	dd ktoChar

	;pit syscalls

	extern ksleep
	dd ksleep

	;vga syscalls

	extern kclearText
	dd kclearText

	extern kputc
	dd kputc

	extern kputn
	dd kputn

	extern kputs
	dd kputs

	extern ksetCursor
	dd ksetCursor

regs: times 32 db 0
retinfo: times 6 db 0
