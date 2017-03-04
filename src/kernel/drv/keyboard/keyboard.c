/*

PSOS Development Build
https://github.com/TheBenPerson/PSOS/tree/dev

Copyright (C) 2016 Ben Stockett <thebenstockett@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/

#include "types.h"
#include "kernel.h"
#include "keyboard.h"

void (*keyCallback)(byte) = 0;
bool keyState[87];
char keyMap[] = {0x0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', 0x0, 0x0, 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', 0x0, 0x0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0x0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0x0, '*', 0x0, ' ', 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, '7', '8', '9', '-', '4', '5', '6', '+', '1', '2', '3', '0', '.', 0x0, 0x0, 0x0, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', 0x0, 0x0, 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', 0x0, 0x0, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '\"', '~', 0x0, '|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,};

extern void keyboardISR();

void initKeyboard() {

	installISR(9, keyboardISR);

}

void keyboardHandler() {

	byte scanCode;
	bool value; //why K&R? WHY?!?!

	#asm

		in al, #0x60
		mov [bp - 1], al

	#endasm

	if (scanCode > 0xD8) return;

	value = !(scanCode & 0x80);

	scanCode <<= 1;
	scanCode >>= 1;

	keyState[--scanCode] = value;

	if (value && keyCallback) keyCallback(scanCode);

}

char scToChar(byte scanCode) {

	if (keyState[VK_LSHIFT] || keyState[VK_RSHIFT]) scanCode += 85;

	return keyMap[scanCode];

}
