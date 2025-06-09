#ifndef PCB_H
#define PCB_H
#define MAX_CHILDREN 9
#define MAX_PIPES 10 
#define STDIN 0
#define STDOUT 1
#include<stdint.h>
#include <stdlib.h>
#include "../include/pipe.h"
typedef enum STATUS {RUNNING,READY, BLOCKED, TERMINATED} STATUS;
#define MAX_BUFF 10

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
    int parent_pid; 
    int waiting_on_child;
    uint16_t children[MAX_CHILDREN];
    uint16_t children_length;
    int fd_table[MAX_BUFF * 2];
    int stdin;
    int stdout;
}p_info;

typedef struct ReadyNode {
    p_info* process_info;
    int counter;
    struct ReadyNode* next;
} ReadyNode;



#endif