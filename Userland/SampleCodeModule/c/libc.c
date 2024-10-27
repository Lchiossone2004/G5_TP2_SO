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
void zoomIn(){  //estas tal vez las moveria a un archivo que sea zoom, tipo la lib.c es para que la use el usuario
    syscall(6,STDOUT);
}
void zoomOut(){
    syscall(7,STDOUT);
}
void printTime(){
    syscall(13,STDOUT);
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
void history(){
    syscall(8,STDOUT);
}
