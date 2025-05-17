/* sampleCodeModule.c */
#include "./include/shell.h"
#include <stdint.h>

char * v = (char*)0xB8000 + 79 * 2;

extern uint64_t syscall(uint64_t rdi, ...);

void startShell(){
	char* args[] = {};
	syscall(22,(void*)shell,0,args);
}
int main() {
	startShell();
	while(1);
	return 0xDEADBEEF;
}
