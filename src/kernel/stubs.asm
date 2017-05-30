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

extern syscalled
extern uSegment
global syscallISR

syscallISR: ;syscall handler

	sti ;enable interrupts
	pushad ;save registers

	mov ax, cs
	mov ds, ax ;ds = KERNEL_SEGMENT
	mov ss, ax

	mov byte [syscalled], 1 ;syscalled = true

	shl bx, 2 ;call *= 4

	push edi
	push esi
	push ecx
	call dword [callTable + bx]
	add sp, 12

	mov es, ax

	mov byte [syscalled], 0 ;syscalled = false

	mov ax, [uSegment]
	mov ds, ax ;restore segment
	mov ss, ax

	popad

	mov ax, es

iret

SECTION .data

callTable:

	;core syscalls

	extern kexec
	dd kexec

	extern kinstallISR
	dd kinstallISR

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

	;rtc syscalls

	extern kgetDate
	dd kgetDate

	;spkr syscalls

	extern kbeep
	dd kbeep

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

	extern ksetPosition
	dd ksetPosition

	extern ksetVGAMode
	dd ksetVGAMode

	extern setAttr
	dd setAttr
