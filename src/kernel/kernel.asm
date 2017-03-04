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

[BITS 16]
[ORG 0x500]

call ClearScreen

push LoadedKernel
call PrintString
add sp, 2

push VersionString
call PrintString
add sp, 2

push Ready
call PrintString
add sp, 2

%include "/drv/keyboard/install.asm"
%include "/drv/pit/install.asm"

fninit

call CLIMain

call ClearScreen
mov word [CharX], 0

push 0
call SetCursor
add sp, 2

push ShutdownMessage
call PrintString
add sp, 2

cli
hlt

%include "/prg/cli/cli.asm"
%include "/drv/keyboard/keyboard.asm"
%include "/drv/pit/pit.asm"
%include "/drv/storage/storage.asm"
%include "/drv/vga/vga.asm"

LoadedKernel: db "Loaded kernel.", 0xA, 0xA, 0x0
Ready: db 0xA, "Ready.", 0xA, 0xA, 0x0
ShutdownMessage: db "System ready for shutdown.", 0xA, "Please power off and remove boot medium.", 0x0
VersionString: db "PSOS (Pretty Simple/Stupid Operating System) Version 1.0", 0xA, "Ben Stockett - 2016", 0x0

align 512
FS_Start:
