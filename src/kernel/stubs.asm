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

extern brkpt
global brkptISR

brkptISR: ;int3 handler

	sti
	pushad

	mov ax, cs
	mov ds, ax

	call dword brkpt

	mov ax, ss
	mov ds, ax

	popad

iret

extern segment
extern syscalled
global syscallISR

syscallISR: ;syscall handler

	sti ;enable interrupts

	pushad ;save registers
	pop dword [regs]
	pop dword [regs + 4]
	pop dword [regs + 8]
	pop dword [regs + 12]
	pop dword [regs + 16]
	pop dword [regs + 20]
	pop dword [regs + 24]
	pop dword [regs + 28]

	mov ax, cs
	mov ds, ax ;ds = KERNEL_SEGMENT

	mov byte [syscalled], 1 ;syscalled = 1

	pop dword [retinfo]
	pop dword [retinfo + 4]
	pop dword [retinfo + 8]
	pop word [retinfo + 12] ;now arguments are at the top of the stack

	pop ebx
	shl ebx, 2 ;call *= 4

	call dword [callTable + ebx]
	mov [retval], ax

	push ebx

	push word [retinfo + 12]
	push dword [retinfo + 8]
	push dword [retinfo + 4]
	push dword [retinfo]

	mov ax, [segment]
	mov ds, ax ;restore segment

	push dword [regs + 28] ;restore registers
	push dword [regs + 24]
	push dword [regs + 20]
	push dword [regs + 16]
	push dword [regs + 12]
	push dword [regs + 8]
	push dword [regs + 4]
	push dword [regs]
	popad

	mov ax, [retval]

iret

SECTION .data

callTable:

	;keyboard syscalls

	extern getKey

	dd getKey

	;pit syscalls

	extern sleep

	dd sleep

	;vga syscalls

	extern clearText
	extern putc
	extern putn
	extern puts

	dd clearText
	dd putc
	dd putn
	dd puts

regs: times 32 db 0
retinfo: times 14 db 0
retval: dw 0
