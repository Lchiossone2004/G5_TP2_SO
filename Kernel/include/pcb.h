#ifndef PCB_H
#define PCB_H

#include<stdint.h>
#include <stdlib.h>

typedef enum STATUS {RUNNING,READY, BLOCKED, TERMINATED} STATUS;

typedef struct{
    uint16_t pid;
    char* name;
    void* stack_base;
    void* stack_pointer;
    char ** argv;
    int priority;
    STATUS state;
}p_info;

typedef struct ReadyNode {
    p_info* process_info;
    struct ReadyNode* next;
} ReadyNode;

#endif