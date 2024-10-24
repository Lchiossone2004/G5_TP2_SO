#include "../include/libc.h"
#include <stdlib.h>
#include <stdint.h>

#define STDOUT 1
#define STDIN 0
#define STDERR 2

extern uint64_t syscall(uint64_t rdi, ...);
static char buffer[100];

void print(char * word){
    int i = 0;
    while(word[i] != 0){
        i++;
    }
    syscall(4,STDOUT,word,i);
}

void read(char * buffer){
    syscall(3,STDOUT, buffer);
    return;
}
void printRegisters(){
    syscall(1);
}