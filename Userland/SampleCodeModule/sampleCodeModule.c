/* sampleCodeModule.c */
#include "./include/shell.h"
#include <stdint.h>
#include "./include/c-lib.h"
#define VERDE 0x00FF00

char * v = (char*)0xB8000 + 79 * 2;

extern uint64_t syscall(uint64_t rdi, ...);



void sayA(uint8_t argc, char** argv);
void sayB(uint8_t argc, char** argv);

void inactive(){}

int main() {
	//void* functionPointer2 = (void*)inactive;
	//startShell();
	char* args[] = {"hola"};
	uint64_t pidA = syscall(22, (void*)sayA, 1, args, "A", 5, 1);
    uint64_t pidB = syscall(22, (void*)sayB, 1, args, "B", 2, 0);
    


char bufA[10], bufB[10];
uintToBase(pidA, bufA, 10);
uintToBase(pidB, bufB, 10);

syscall(4, STDOUT, "PID A: ", 7);
syscall(4, STDOUT, bufA, strSize(bufA));
syscall(4, STDOUT, "\n", 1);

syscall(4, STDOUT, "PID B: ", 7);
syscall(4, STDOUT, bufB, strSize(bufB));
syscall(4, STDOUT, "\n", 1);


syscall(8, 30);  // dejar que impriman
//syscall(23, pidA, 0, 0, 0, 0);  // matar a A
//syscall(29);
	while (1) {
    syscall(8, 5);  // el main no interfiere, solo espera
}

	return 0xDEADBEEF;
}

void sayA(uint8_t argc, char** argv) {
    char letra = 'A';
   uint16_t childpid = syscall(30);
    //syscall(29);
    while (1) {
        print(argv[0]);
        syscall(8, 1);
    }
}

void sayB(uint8_t argc, char** argv) {
    char letra = 'B';
    uint16_t childpid = syscall(30);
    syscall(29);
    while (1) {
        for (int i = 0; i < 3; i++) {
            syscall(4, STDOUT, &letra, 1);
        }
        syscall(8, 1);
    }
}

void uintToBase(uint64_t value, char *buffer, uint32_t base) {
    char *p = buffer;
    char *p1, *p2;
    uint64_t temp_value;

    // Mínima base es 2, máxima es 16
    if (base < 2 || base > 16) {
        *buffer = 0;
        return;
    }

    do {
        temp_value = value;
        value /= base;
        *p++ = "0123456789ABCDEF"[temp_value - (value * base)];
    } while (value);

    // Null-terminate
    *p = '\0';

    // Reverse
    p1 = buffer;
    p2 = p - 1;
    while (p1 < p2) {
        char tmp = *p1;
        *p1 = *p2;
        *p2 = tmp;
        p1++;
        p2--;
    }
}
