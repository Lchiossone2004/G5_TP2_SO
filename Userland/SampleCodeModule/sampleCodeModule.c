/* sampleCodeModule.c */
#include <stdint.h>
#include "./include/shell.h"
#include "./include/c-lib.h"
#define VERDE 0x00FF00

char * v = (char*)0xB8000 + 79 * 2;

extern uint64_t syscall(uint64_t rdi, ...);

void idle(){
	while(1){}
}
int main() {
	char *argv[] = {0};
	usr_create_process((void*)idle,0,argv, "idle", PRIORITY_LOW,1);
	usr_create_process((void*)shell,0,argv, "shell", PRIORITY_LOW,1);
	while (1) {}

	return 0xDEADBEEF;
}
