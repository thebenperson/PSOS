#include "arch.h"
#include "usr.h"

void main() {

	struct regs regs;
	getRegs(&regs);

	word mem = 0xFFFF - regs.sp;
	bool kib = false;

	if (mem >= 1024) {

		mem /= 1024;
		kib = true;

	}

	puts("Used memory: ");
	putn(mem, false);
	if (kib) puts("KiB\n");
	else puts("B\n");

}
