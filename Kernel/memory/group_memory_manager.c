#include "group_memory_manager.h"
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

// Se agrega campo user_size para seguir el tamaño real pedido por el usuario
typedef struct block_header {
    size_t size;
    size_t user_size;
    bool is_free;
    struct block_header *next;
    struct block_header *prev;
    struct block_header *free_next;
    struct block_header *free_prev;
} block_header_t;

static block_header_t *free_list = NULL;
static void *memory_start;
static size_t total_size;
static size_t current_blocks = 0;
static size_t total_allocated = 0;
static size_t total_freed = 0;

static inline size_t align8(size_t sz) {
    return (sz + 7) & ~((size_t)7);
}

static void insert_into_free_list(block_header_t *b) {
    b->is_free = true;
    b->user_size = 0;
    if (!free_list) {
        b->free_next = b->free_prev = b;
        free_list = b;
        return;
    }
    block_header_t *tail = free_list->free_prev;
    b->free_next = free_list;
    b->free_prev = tail;
    tail->free_next = b;
    free_list->free_prev = b;
    free_list = b;
}

static void remove_from_free_list(block_header_t *b) {
    if (b->free_next == b) {
        free_list = NULL;
    } else {
        b->free_prev->free_next = b->free_next;
        b->free_next->free_prev = b->free_prev;
        if (free_list == b) free_list = b->free_next;
    }
    b->free_next = b->free_prev = NULL;
    b->is_free = false;
}

static void *group_init(void *start, size_t size) {
    memory_start = start;
    total_size = size;
    current_blocks = total_allocated = total_freed = 0;
    block_header_t *initial = (block_header_t *)start;
    initial->size = size - sizeof(block_header_t);
    initial->user_size = 0;
    initial->is_free = true;
    initial->next = initial->prev = NULL;
    initial->free_next = initial->free_prev = initial;
    free_list = initial;
    return start;
}

static void *group_malloc(size_t size) {
    size_t req = align8(size);
    if (req == 0 || current_blocks >= MAX_BLOCKS || !free_list) return NULL;
    block_header_t *cur = free_list;
    do {
        if (cur->size >= req) {
            size_t rem = cur->size - req;
            if (rem > sizeof(block_header_t) + MIN_BLOCK_SIZE) {
                block_header_t *nb = (void *)((char *)cur + sizeof(block_header_t) + req);
                nb->size = rem - sizeof(block_header_t);
                nb->user_size = 0;
                nb->is_free = true;
                nb->next = cur->next;
                nb->prev = cur;
                if (cur->next) cur->next->prev = nb;
                cur->next = nb;
                insert_into_free_list(nb);
                cur->size = req;
            }
            remove_from_free_list(cur);
            cur->user_size = req;
            total_allocated += cur->user_size;
            current_blocks++;
            return (char *)cur + sizeof(block_header_t);
        }
        cur = cur->free_next;
    } while (cur != free_list);
    return NULL;
}

static size_t group_free(void *ptr) {
    if (!ptr) return 0;
    block_header_t *b = (block_header_t *)((char *)ptr - sizeof(block_header_t));

    size_t freed_size = b->user_size;

    b->is_free = true;
    b->user_size = 0;
    if (b->next && b->next->is_free) {
        remove_from_free_list(b->next);
        b->size += sizeof(block_header_t) + b->next->size;
        b->next = b->next->next;
        if (b->next) b->next->prev = b;
    }
    if (b->prev && b->prev->is_free) {
        remove_from_free_list(b->prev);
        b->prev->size += sizeof(block_header_t) + b->size;
        b->prev->next = b->next;
        if (b->next) b->next->prev = b->prev;
        b = b->prev;
    }
    insert_into_free_list(b);
    current_blocks--;
    total_freed += freed_size;

    return freed_size;
}

static void group_get_info(memory_info_t *info) {
    if (!info) return;
    info->total_memory     = total_size;
    info->used_memory      = total_size - 0;
    info->free_memory      = 0;
    info->block_count      = current_blocks;
    info->free_block_count = 0;
    info->total_allocated  = total_allocated;
    info->total_freed      = total_freed;
    info->memory_leak      = (total_allocated != total_freed);
    if (free_list) {
        block_header_t *cur = free_list;
        do {
            info->free_memory += cur->size;
            info->free_block_count++;
            cur = cur->free_next;
        } while (cur != free_list);
    }
    info->used_memory = total_size - info->free_memory;
    info->block_count = info->free_block_count + current_blocks;
}

static memory_manager_t group_manager = {
    .init     = group_init,
    .malloc   = group_malloc,
    .free     = group_free,
    .get_info = group_get_info
};

memory_manager_t *get_group_memory_manager(void) {
    return &group_manager;
}
