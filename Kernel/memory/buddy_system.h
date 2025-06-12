#ifndef _BUDDY_SYSTEM_H
#define _BUDDY_SYSTEM_H

#include "memory_manager.h"

memory_manager_t* get_buddy_system_manager(void);

#define MIN_BLOCK_SIZE 64
#define MAX_ORDER 12       // Orden máximo (2^12 = 4096 bytes)

#endif
