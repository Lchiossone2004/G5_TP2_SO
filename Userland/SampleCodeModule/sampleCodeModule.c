/* sampleCodeModule.c */
#include <stdint.h>
#include "./include/shell.h"
#include "./include/c-lib.h"
#define VERDE 0x00FF00

char * v = (char*)0xB8000 + 79 * 2;

extern uint64_t syscall(uint64_t rdi, ...);

void inactive(){}

int main() {
	usr_create_process((void*)shell,NULL,NULL, "shell", 5,1);
	while (1) {
    syscall(8, 5);  // el main no interfiere, solo espera
}

	return 0xDEADBEEF;
}