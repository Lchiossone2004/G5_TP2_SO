#ifndef MEMORY_LIB
#define MEMORY_LIB 

#include <stdint.h>
#include <stdlib.h>

typedef struct {
    size_t total_memory;
    size_t used_memory;
    size_t free_memory;
    size_t block_count;
    size_t free_block_count;
    size_t total_allocated;
    size_t total_freed;
    size_t current_blocks;
    int memory_leak;
} memory_info_t;

void *usr_malloc(int size);
void usr_free(void* ptr);
void print_usr_mem_info();

#endif