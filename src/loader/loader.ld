/*

PSOS Development Build
Copyright (C) 2016 Ben Stockett.

This file is part of PSOS (Pretty Simple/Stupid Operating System).

PSOS is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

PSOS is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with PSOS.  If not, see <http://www.gnu.org/licenses/>.

*/

SEARCH_DIR(drv)
INPUT(loaderStub.o loader.o drv.o)
OUTPUT(../../bin/loader.bin)
OUTPUT_FORMAT(binary)

SECTIONS {

	.text : {

		loaderStub.o(.text)

		loader.o(.text)
		drv.o(.text)

	}

	.data : {

		loader.o(.data)

	}

	.sig : AT(0x1FE) {

		SHORT(0xAA55);

	}

	/DISCARD/ : {

		*(.eh_frame)
		*(.comment)

	}

}
