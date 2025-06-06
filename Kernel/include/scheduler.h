#ifndef SCHEDULER_H
#define SCHEDULER_H
#include<stdint.h>
#include <stdlib.h>
#include "./pcb.h"

void* scheduler(void* current_sp);
void add_to_ready_list(p_info* process);
void remove_from_ready_list(p_info* process);
int remove_from_processes_list(p_info* process);
int block_process(int pid);
int unblock_process(int pid);
void add_to_process_list(p_info* process);
p_info* find_process_by_stack(void* sp);
p_info* get_current_process();
int kill_process(int pid);
int modify_priority(int pid, int newPriority);
int foundprocess(int pid);
void get_processes();
void printFixed(const char* str);
uint16_t quitCPU();
p_info* get_foreground_process();

#endif