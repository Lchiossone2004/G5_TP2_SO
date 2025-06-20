#ifndef _MEMORY_MANAGER_H
#define _MEMORY_MANAGER_H

#include <stdint.h>
#include <stddef.h>

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

typedef struct {
    void* (*init)(void* start, size_t size);
    void* (*malloc)(size_t size);
    size_t (*free)(void* ptr);
    void (*get_info)(memory_info_t* info);
} memory_manager_t;

void init_memory_manager(void* start, size_t size);
void* mm_malloc(size_t size);
size_t mm_free(void* ptr);
void mm_get_info(memory_info_t* info);

#ifdef BUDDY_SYSTEM
    #define MEMORY_MANAGER_TYPE "Buddy System"
#else
    #define MEMORY_MANAGER_TYPE "Group Memory Manager"
#endif

#endif