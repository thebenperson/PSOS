#include "arch.h"
#include "usr.h"

void main() {

	struct regs regs;
	getRegs(&regs);

	puts("Used memory: ");
	putn((0xFFFF - regs.sp) / 1024, false);
	puts(" KiB\n");

}
