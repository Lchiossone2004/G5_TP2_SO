#ifndef PCB_H
#define PCB_H
#define MAX_CHILDREN 9
#include<stdint.h>
#include <stdlib.h>

typedef enum STATUS {RUNNING,READY, BLOCKED, TERMINATED} STATUS;

typedef struct{
    int pid;
    char* name;
    void* stack_top;
    void* stack_base;
    void* stack_pointer;
    char ** argv;
    int argc;
    int priority;
    char *priorityName;
    int is_foreground;
    STATUS state;
    uint16_t children[MAX_CHILDREN];
    uint16_t children_length;
}p_info;

typedef struct ReadyNode {
    p_info* process_info;
    int counter;
    struct ReadyNode* next;
} ReadyNode;

#endif