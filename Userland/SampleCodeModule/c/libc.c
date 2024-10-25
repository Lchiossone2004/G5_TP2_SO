#include "../include/libc.h"
#include <stdlib.h>
#include <stdint.h>

#define STDOUT 1
#define STDIN 0
#define STDERR 2

extern uint64_t syscall(uint64_t rdi, ...);

void getChar(char * buffer){
    syscall(2,STDOUT,buffer,1);
}

void print(char * word, int size){
    syscall(4,STDOUT,word,size);
}

void read(char * buffer){
    syscall(3,STDOUT, buffer);
    return;
}
void printRegisters(){
    syscall(1,STDOUT);
}
// char stringcmp(const char* str1, const char* str2){      Y esto?
//     syscall(5, str1, str2);
// }

void nlPrint(){
    syscall(5,STDOUT);
}