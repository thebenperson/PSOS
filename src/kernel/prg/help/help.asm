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

HELP:

	push VersionString
	call PrintString
	add sp, 0x2

	push HelpMsg
	call PrintString
	add sp, 0x2

ret

CS_HELP: db "HELP", 0x0
HelpMsg: db 0xA, 0xA, "Enter a command or press escape to prepare for shutdown.", 0xA, 0xA, "Commands: ", 0xA, "CIRCLE: A circle drawing example.", 0xA, "CLEAR: Clear the console.", 0xA, "HELP: Display this help message.", 0xA, "VGATEST: Test VGA graphics mode.", 0xA, 0x0
