#ifndef PROCESS_H
#define PROCESS_H

#include<stdint.h>
#include <stdlib.h>
#include <pcb.h>


typedef struct {
    void* r15;
    void* r14;
    void* r13;
    void* r12;
    void* r11;
    void* r10;
    void* r9;
    void* r8;
    void* rsi;
    void* rdi;
    void* rbp;
    void* rdx;
    void* rcx;
    void* rbx;
    void* rax;
    void* rip;
    void* cs;
    void* rflags;
    void* rsp;
    void* ss;
} p_stack;

int32_t createProcess(void (*fn)(uint8_t, char **), uint8_t argc, char* argv[], char* name, int priority, int is_foreground);
void load_args(p_stack *new_stack, p_info * new_process, uint8_t argc, char* argv[]);
void entry_point_wrapper(void (*fn)(uint8_t, char**), uint8_t argc, char** argv) ;
void exit_process();  
int get_pid();
void copy_context(p_info* new_process, char *name, void * stack_base, void * stack_pointer,void * stack_top ,int priority, int is_foreground);
int wait_pid(int pid);
int wait();
void initialize_zero(uint16_t array[], int size);
void assignForeground(p_info* new_process, int is_foreground);
void assignPid(p_info* new_process);
void freePid(int pid);


#endif