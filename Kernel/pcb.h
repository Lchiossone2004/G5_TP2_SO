#ifndef PCB_H
#define PCB_H

#include<stdint.h>

#define MAX_PROCESSES 10
typedef enum { READY, RUNNING, BLOCKED, TERMINATED } ProcessState;

typedef struct{
    void* stack_pointer;
    uint64_t program_counter;
    ProcessState state;
    int pid;
    int priority;
    char process_name[20];
    struct PCB* next;
}PCB;


typedef struct ReadyNode {
    PCB* process;
    struct ReadyNode* next;
} ReadyNode;

#endif