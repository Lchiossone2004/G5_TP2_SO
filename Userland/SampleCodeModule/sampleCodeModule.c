/* sampleCodeModule.c */
#include "./include/shell.h"
#include <stdint.h>

char * v = (char*)0xB8000 + 79 * 2;

extern uint64_t syscall(uint64_t rdi, ...);

int main() {
	shell();
	return 0xDEADBEEF;
}
