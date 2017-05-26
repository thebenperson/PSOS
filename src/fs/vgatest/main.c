#include "types.h"
#include "usr.h"

void main() {

	struct __attribute__((packed)) {

		char signature[4];
		word version;
		dword oemString;
		byte caps[4];
		dword modePTR;
		word mem;
		byte reserved[236];

	} VBEData;

	word result;

	asm("mov ax, ds");
	asm("mov es, ax");
	asm("int 0x10" : "=a" (result) : "a" (0x4F00), "D" (&VBEData));
	if (result != 0x4F) {

		puts("Error: VESA not supported\n");
		return;

	}

	word* modes = (word*) VBEData.modePTR;

	//for (byte i = 0; modes[i] != -1; i++) {

		struct __attribute__((packed)) {

			word attributes;
			byte winA,winB;
			word granularity;
			word winsize;
			word segmentA;
			word segmentB;
			word realFctPtr;
			word pitch;

			word Xres, Yres;
			byte Wchar, Ychar, planes, bpp, banks;
			byte memory_model, bank_size, image_pages;
			byte reserved0;

			byte red_mask, red_position;
			byte green_mask, green_position;
			byte blue_mask, blue_position;
			byte rsv_mask, rsv_position;
			byte directcolor_attributes;

			dword physbase;
			byte reserved1;
			byte reserved[216];

		} info;

		asm("int 0x10" :: "a" (0x4F01), "c" (modes[0]), "D" (&info));
		//if (info.attributes & 0xA) continue;
		puts("Width: ");
		putn(info.Wchar, false);
		puts(" Height: ");
		putn(info.Ychar, false);

//	}

	putc('\n');

}
