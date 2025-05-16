#ifndef PROCESS_H
#define PROCESS_H

#include<stdint.h>
#include <stdlib.h>


typedef struct {
    void *r15;
    void *r14;
    void *r13;
    void *r12;
    void *r11;
    void *r10;
    void *r9;
    void *r8;
    void *rsi;
    void *rdi;
    void *rbp;
    void *rdx;
    void *rcx;
    void *rbx;
    void *rax;
    void *rip;
    void *cs;
    void *rflags;
    void *rsp;
    void *ss;
} p_stack;

uint64_t createProcess(void (*fn)(uint8_t, char **), uint8_t argc, char* argv[]);

#endif