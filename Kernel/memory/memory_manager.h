#ifndef _MEMORY_MANAGER_H
#define _MEMORY_MANAGER_H

#include <stdint.h>
#include <stddef.h>
#include "memory_syscalls.h"

typedef struct {
    void* (*init)(void* start, size_t size);
    void* (*malloc)(size_t size);
    void (*free)(void* ptr);
    void (*dump)(void);
    void (*get_info)(memory_info_t* info);
} memory_manager_t;

void init_memory_manager(void* start, size_t size);
void* mm_malloc(size_t size);
void mm_free(void* ptr);
void mm_dump(void);
void mm_get_info(memory_info_t* info);

#ifdef BUDDY_SYSTEM
    #define MEMORY_MANAGER_TYPE "Buddy System"
#else
    #define MEMORY_MANAGER_TYPE "Group Memory Manager"
#endif

#endif // _MEMORY_MANAGER_H 