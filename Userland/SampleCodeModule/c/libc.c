#include "../include/libc.h"
#include <stdlib.h>
#include <stdint.h>

#define STDOUT 0
#define STDIN 1
#define STDERR 2

extern uint64_t syscall(uint64_t rdi, ...);
void printRegisters(){
    syscall(1,STDOUT);
}

void getChar(char * buffer, int index){
    syscall(2,STDOUT,buffer,index);
}

void print(char * word, int size){
    syscall(4,STDIN,word,size);
}

void read(char * buffer){
    syscall(3,STDOUT, buffer);
    return;
}

void printErr(char * word, int size){
    syscall(4,STDERR,word,size);
}


void nlPrint(){
    syscall(5,STDOUT);
}

void zoomIn(){
    syscall(6,STDOUT);
}
void zoomOut(){
    syscall(7,STDOUT);
}

