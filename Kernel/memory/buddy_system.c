#include "buddy_system.h"

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

typedef struct block_header {
    size_t size;             // Tamaño total del bloque (payload, sin header)
    size_t user_size;        // Tamaño solicitado por el usuario
    bool is_free;
    struct block_header* next;
} block_header_t;

static block_header_t* free_blocks[MAX_ORDER + 1];
static void* memory_start;
static size_t total_size;
static size_t current_blocks = 0;
static size_t total_allocated = 0;
static size_t total_freed = 0;

static size_t get_order(size_t size) {
    size_t order = 0;
    size_t block_size = MIN_BLOCK_SIZE;
    while (block_size < size && order < MAX_ORDER) {
        block_size <<= 1;
        order++;
    }
    return order;
}

static void* buddy_init(void* start, size_t size) {
    memory_start = start;
    total_size = size;

    for (int i = 0; i <= MAX_ORDER; i++) {
        free_blocks[i] = NULL;
    }
    block_header_t* initial = (block_header_t*)start;
    initial->size = size - sizeof(block_header_t);
    initial->user_size = 0;
    initial->is_free = true;
    initial->next = NULL;

    size_t order = get_order(initial->size + sizeof(block_header_t));
    free_blocks[order] = initial;
    return start;
}

static void* buddy_malloc(size_t size) {
    if (size == 0) return NULL;

    size_t requested = size;
    size_t req_with_hdr = size + sizeof(block_header_t);

    // Encontrar la orden mínima que pueda alojar el tamaño solicitado + header.
    size_t order = get_order(req_with_hdr);

    // Buscar el bloque disponible de orden >= order.
    size_t cur_order = order;
    while (cur_order <= MAX_ORDER && free_blocks[cur_order] == NULL) {
        cur_order++;
    }
    if (cur_order > MAX_ORDER) return NULL;

    // Tomar el bloque más grande disponible.
    block_header_t* block = free_blocks[cur_order];
    free_blocks[cur_order] = block->next;

    // Dividir bloques hasta alcanzar el orden deseado.
    while (cur_order > order) {
        cur_order--;

        size_t split_block_size = (1u << cur_order) * MIN_BLOCK_SIZE;
        block_header_t* buddy = (block_header_t*)((char*)block + split_block_size);

        // Configurar el buddy.
        buddy->size = split_block_size - sizeof(block_header_t);
        buddy->user_size = 0;
        buddy->is_free = true;
        buddy->next = free_blocks[cur_order];
        free_blocks[cur_order] = buddy;

        // Ajustar el bloque actual.
        block->size = split_block_size - sizeof(block_header_t);
    }

    // Asignar el bloque al usuario.
    block->is_free = false;
    block->user_size = requested;
    block->next = NULL;

    current_blocks++;
    total_allocated += requested;

    return (void*)((char*)block + sizeof(block_header_t));
}


static size_t buddy_free(void* ptr) {
    if (ptr == NULL) return 0;
    block_header_t* block = (block_header_t*)((char*)ptr - sizeof(block_header_t));
    size_t freed_size = block->user_size;

    block->is_free = true;
    block->user_size = 0;
    if (total_allocated >= freed_size) {
        total_allocated -= freed_size;
    }
    total_freed += freed_size;
    current_blocks--;

    size_t order = get_order(block->size + sizeof(block_header_t));

    while (order < MAX_ORDER) {
        size_t merge_block_bytes = (1u << order) * MIN_BLOCK_SIZE;
        uintptr_t block_addr = (uintptr_t)block;
        uintptr_t buddy_addr = block_addr ^ merge_block_bytes;

        if (buddy_addr < (uintptr_t)memory_start ||
            buddy_addr >= (uintptr_t)memory_start + total_size) {
            break;
        }

        block_header_t* buddy = (block_header_t*)buddy_addr;

        if (!(buddy->is_free) || (buddy->size != block->size)) {
            break;
        }

        block_header_t** prev = &free_blocks[order];
        block_header_t* found = NULL;
        while (*prev) {
            if (*prev == buddy) {
                found = buddy;
                break;
            }
            prev = &((*prev)->next);
        }
        if (!found) {
            break;
        }
        *prev = buddy->next;

        if (buddy_addr < block_addr) {
            block = buddy;
        }

        block->size = 2 * (block->size + sizeof(block_header_t)) - sizeof(block_header_t);

        order++;
    }

    block->next = free_blocks[order];
    free_blocks[order] = block;

    return freed_size;
}


static void buddy_get_info(memory_info_t* info) {
    if (!info) return;
    info->total_memory= total_size;
    info->current_blocks= current_blocks;
    info->free_block_count= 0;
    info->free_memory= 0;
    info->total_allocated= total_allocated;
    info->total_freed= total_freed;
    info->memory_leak= (total_allocated != total_freed);
    for (int i = 0; i <= MAX_ORDER; i++) {
        block_header_t* cur = free_blocks[i];
        while (cur) {
            info->free_memory += cur->size;
            info->free_block_count++;
            cur = cur->next;
        }
    }
    info->used_memory  = total_size - info->free_memory;
    info->block_count  = info->free_block_count + info->current_blocks;
}

static memory_manager_t buddy_manager = {
    .init     = buddy_init,
    .malloc   = buddy_malloc,
    .free     = buddy_free,
    .get_info = buddy_get_info
};

memory_manager_t* get_buddy_system_manager(void) {
    return &buddy_manager;
}
