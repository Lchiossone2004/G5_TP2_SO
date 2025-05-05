#ifndef _BUDDY_SYSTEM_H
#define _BUDDY_SYSTEM_H

#include "memory_manager.h"

// Obtiene la instancia del administrador Buddy System
memory_manager_t* get_buddy_system_manager(void);

// Constantes para el Buddy System
#define MIN_BLOCK_SIZE 64  // Tamaño mínimo de bloque
#define MAX_ORDER 12       // Orden máximo (2^12 = 4096 bytes)

#endif // _BUDDY_SYSTEM_H 