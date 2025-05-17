#ifndef SCHEDULER_H
#define SCHEDULER_H
#include<stdint.h>
#include <stdlib.h>
#include "./pcb.h"

uint64_t scheduler(uint64_t current_sp);
void add_to_ready_list(p_info* process);
void remove_from_ready_list(p_info* process);
void block_process(p_info* process);
void unblock_process(p_info* process);
void add_to_process_list(p_info* process);
p_info* find_process_by_stack(void* sp);
p_info* get_current_process();

#endif