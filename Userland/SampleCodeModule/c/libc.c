#include "../include/libc.h"
#include <stdlib.h>
#include <stdint.h>
static char* savedLetter;
extern uint64_t syscall(uint64_t rdi, ...);

void print(char * word){
    int i = 0;
    while(word[i] != 0){
        i++;
    }
    syscall(4,1,word,i);
}

char* read(char letter){
    syscall(3,1,letter,1);
    return savedLetter;
}