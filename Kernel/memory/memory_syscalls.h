#ifndef _MEMORY_SYSCALLS_H
#define _MEMORY_SYSCALLS_H

#include <stdint.h>
#include <stddef.h>

// Estructura para información de memoria
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

// Syscalls de memoria
void* sys_malloc(size_t size);
void sys_free(void* ptr);
void sys_get_memory_info(memory_info_t* info);

#endif // _MEMORY_SYSCALLS_H 