#include "../include/libc.h"
#include <stdlib.h>
#include <stdint.h>

#define STDIN 0
#define STDOUT 1
#define STDERR 2


extern uint64_t syscall(uint64_t rdi, ...);

void printRegisters(){
    syscall(1,STDOUT);
}

void print(char * word, int size){
    syscall(4,STDOUT,word,size);
}

void read(char * buffer,int size){
    syscall(3,STDOUT, buffer, size);
    return;
}

void printErr(char * word, int size){
    syscall(4,STDERR,word,size);
}

void nlPrint(){
    syscall(5,STDOUT);
}


void sleep(int ticks){
    syscall(8,ticks);
    return;
}

int strCompare(char *s1, char*s2){
    int flag = 1;
    if(strSize(s1) != strSize(s2)){
        flag = 0;
    }
    for(int i = 0; i< strSize(s1) && flag; i++){
            if(s1[i] != s2[i]){
                flag = 0;
            }
    }
    return flag;
}

int strSize(char * word){
    int toRet = 0;
    while(word[toRet] != 0){
        toRet++;
    }
    return toRet;
}