#ifndef _GROUP_MEMORY_MANAGER_H
#define _GROUP_MEMORY_MANAGER_H

#include "memory_manager.h"

memory_manager_t* get_group_memory_manager(void);

#define MIN_BLOCK_SIZE 64
#define MAX_BLOCKS 1024

#endif