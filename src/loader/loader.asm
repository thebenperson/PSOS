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

jmp boot
nop

db "PSOS-dev" ;OEM identifier
dw 512 ;bytes per sector
db 1 ;sectors per cluster
dw 1 + KERNEL_SIZE ;reserved sectors
db 1 ;FATs
dw 256 ;directory entries
dw 1 + KERNEL_SIZE + 3 + ((256 * 32) / 512) + ((128 * 1024) / 512) ;sectors
db 0xF0 ;media descriptor byte
dw 3 ;sectors per FAT
dw 16 ;sectors per track
dw 2 ;heads
dd 0 ;hidden sectors
dd 0 ;sectors (large)
db 0 ;drive number
db 0 ;reserved
db 0x29 ;signature
dd 127 ;volume ID
db "PSOS-dev   " ;volume label
db "FAT16   " ;filesystem

boot:

jmp 0x7C0:start ;set code segment to 0x7C0

start:

mov ax, cs
mov ds, ax ;set up data segment

mov ax, 0x50
mov ss, ax
mov sp, 0xFFFF ;set up initial stack space

mov [drive], dl ;save drive number

mov ax, KERNEL_SEGMENT
mov es, ax

mov cl, 2 ;sector offset of 2
mov al, KERNEL_SIZE ;size in sectors of kernel
xor bx, bx ;offset of zero
mov ah, 0x2
xor ch, ch
xor dh, dh
mov dl, [drive]

int 0x13 ;load sectors

jnc .longJump

	;oh-no! Something went wrong

	xor ah, ah
	mov al, 0x3
	int 0x10 ;set video mode 3

	mov ah, 0x1
	mov cx, 0x700
	int 0x10 ;disable cursor blink

	mov si, errorMsg

	.loop:

		mov al, [si]
		cmp al, 0

		je .hang

		cmp al, 0xA ;check for newline
		jne .continue

			push ax

			mov cx, 160

			xor dx, dx
			mov ax, bx
			div cx

			mov cx, 160
			sub cx, dx
			add bx, cx ;bx += 160 - (bx % 160)

			pop ax

			inc si
			jmp .loop

		.continue:

		mov [es:bx], al
		mov byte [es:bx + 1], 0x4 ;red text

		inc si
		add bx, 2

	jmp .loop

	.hang:

		cli
		hlt ;hang forever

.longJump:

mov ax, KERNEL_SEGMENT
mov ds, ax ;set up data segment

mov ss, ax
mov sp, 0xFFFF ;set up kernel stack space

jmp KERNEL_SEGMENT:0

errorMsg:
db "Error loading kernel.", 0xA
db "Please power off and remove boot medium.", 0xA, 0xA
db "   |\      _,,,--,,_", 0xA
db "  /,`.-'`'   ._  \-;;,_", 0xA
db " |,4-  ) )_   .;.(  `'-'", 0xA
db "'---''(_/._)-'(_\_)", 0xA
db "2 tired 2 werk. come bak latr.", 0xA, 0x0

times 509 - ($ - $$) db 0
db KERNEL_SIZE
drive: dw 0xAA55
