/* sampleCodeModule.c */
#include "./include/shell.h"
#include <stdint.h>

char * v = (char*)0xB8000 + 79 * 2;

extern uint64_t syscall(uint64_t rdi, ...);

void startShell(){
	void* functionPointer = shell;
	char* args[] = {"hola", "bunas"};
	syscall(22,functionPointer,2,args, "shell");
}
int main() {
	startShell();
	while(1);
	return 0xDEADBEEF;
}
