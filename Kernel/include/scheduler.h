#ifndef SCHEDULER_H
#define SCHEDULER_H
#include<stdint.h>
#include <stdlib.h>
#include "./pcb.h"

void* scheduler(void* current_sp);
void add_to_ready_list(p_info* process);
void remove_from_ready_list(p_info* process);
int remove_from_processes_list(p_info* process);
int block_process(uint16_t pid);
int unblock_process(uint16_t pid);
void add_to_process_list(p_info* process);
p_info* find_process_by_stack(void* sp);
p_info* get_current_process();
int kill_process(uint64_t pid);
int modify_priority(uint16_t pid, int newPriority);
int foundprocess(uint16_t pid);
void get_processes();
void printFixed(const char* str);
uint16_t quitCPU();

#endif