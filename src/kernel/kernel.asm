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
