/* sampleCodeModule.c */
#include "./include/shell.h"
#include <stdint.h>

char * v = (char*)0xB8000 + 79 * 2;

static int var1 = 0;
static int var2 = 0;

extern uint64_t syscall(uint64_t rax, ...);

int main() {
	shell();
	return 0xDEADBEEF;
}
