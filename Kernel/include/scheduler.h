#ifndef SCHEDULER_H
#define SCHEDULER_H
#include<stdint.h>
#include <stdlib.h>
#include "../pcb.h"

uint64_t scheduler(uint64_t current_sp);
void add_to_ready_list(PCB* process);
void remove_from_ready_list(PCB* process);
void block_process(PCB* process);
void unblock_process(PCB* process);
PCB* find_process_by_stack(void* sp);

#endif