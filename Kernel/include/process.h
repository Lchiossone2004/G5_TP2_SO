#ifndef PROCESS_H
#define PROCESS_H
#include<stdint.h>
#include <stdlib.h>
#include "../pcb.h"
#include "../memory/memory_manager.h"
#include "../include/scheduler.h"

#define STACK_SIZE 4096

PCB* create_process(void (*entry_point)(void)) ;

#endif