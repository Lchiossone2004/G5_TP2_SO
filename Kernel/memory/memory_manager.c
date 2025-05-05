#include "memory_manager.h"
#include "buddy_system.h"
#include "group_memory_manager.h"

static memory_manager_t* current_manager = NULL;

void init_memory_manager(void* start, size_t size) {
    if (start == NULL || size == 0) {
        return;
    }

    #ifdef BUDDY_SYSTEM
        current_manager = get_buddy_system_manager();
    #else
        current_manager = get_group_memory_manager();
    #endif
    
    if (current_manager && current_manager->init) {
        current_manager->init(start, size);
    }
}

void* mm_malloc(size_t size) {
    if (!current_manager || !current_manager->malloc || size == 0) {
        return NULL;
    }
    return current_manager->malloc(size);
}

void mm_free(void* ptr) {
    if (!current_manager || !current_manager->free || ptr == NULL) {
        return;
    }
    current_manager->free(ptr);
}

void mm_dump(void) {
    if (!current_manager || !current_manager->dump) {
        return;
    }
    current_manager->dump();
}

void mm_get_info(memory_info_t* info) {
    if (!current_manager || !current_manager->get_info || !info) {
        return;
    }
    current_manager->get_info(info);
} 