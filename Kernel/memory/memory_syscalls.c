#include "memory_syscalls.h"
#include "memory_manager.h"
#include <stddef.h>

// Variables globales para tracking de memoria
static size_t total_allocated = 0;
static size_t total_freed = 0;
static size_t current_blocks = 0;

void* sys_malloc(size_t size) {
    if (size == 0) {
        return NULL;
    }
    void* ptr = mm_malloc(size);
    if (ptr != NULL) {
        total_allocated += size;
        current_blocks++;
    }
    return ptr;
}

void sys_free(void* ptr) {
    if (ptr == NULL) {
        return;
    }
    mm_free(ptr);
    current_blocks--;
}

void sys_get_memory_info(memory_info_t* info) {
    if (info == NULL) {
        return;
    }
    mm_get_info(info);
    info->total_allocated = total_allocated;
    info->total_freed = total_freed;
    info->current_blocks = current_blocks;
    info->memory_leak = (total_allocated > total_freed);
} 