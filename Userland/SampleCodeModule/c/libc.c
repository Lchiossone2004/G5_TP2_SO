#include "../include/libc.h"
#include <stdlib.h>
#include <stdint.h>

extern uint64_t syscall(uint64_t rdi, ...);
static char buffer[10];

void print(char * word){
    int i = 0;
    while(word[i] != 0){
        i++;
    }
    syscall(4,1,word,i);
}

char* read(char letter){
    buffer[0] = syscall(3,1,letter,1);
    buffer[1] = 0;
    return buffer;
}