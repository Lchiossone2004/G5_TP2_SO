#ifndef SCHEDULER_H
#define SCHEDULER_H
#include<stdint.h>

#define RUNNING 1
#define READY 2
#define BLOCKED 3

typedef struct{
    int state;

}PCB;
#endif