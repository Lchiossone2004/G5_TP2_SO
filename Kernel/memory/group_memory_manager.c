#include "memory_manager.h"
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define ALIGNMENT 8
#define MAX_BLOCKS     1024
#define MIN_BLOCK_SIZE 16

#define ALIGN_UP(x) (((x) + (ALIGNMENT - 1)) & ~(ALIGNMENT - 1))

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
static void *memory_start = NULL;
static size_t total_size = 0;
static size_t current_blocks = 0;
static size_t total_allocated = 0;
static size_t total_freed = 0;

static const size_t HEADER_SIZE = ALIGN_UP(sizeof(block_header_t));

static void insert_into_free_list(block_header_t *b) {
    b->free_next = b->free_prev = NULL;
    b->is_free = true;
    b->user_size = 0;
    if (!free_list) {
        b->free_next = b->free_prev = b;
        free_list = b;
    } else {
        block_header_t *tail = free_list->free_prev;
        b->free_next = free_list;
        b->free_prev = tail;
        tail->free_next = b;
        free_list->free_prev = b;
        free_list = b;
    }
}

static void remove_from_free_list(block_header_t *b) {
    if (!b || !b->is_free) return;
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
    memory_start   = start;
    total_size     = size;
    current_blocks = total_allocated = total_freed = 0;

    block_header_t *initial = (block_header_t *)start;
    initial->size      = ALIGN_UP(size - HEADER_SIZE);
    initial->user_size = 0;
    initial->is_free   = true;
    initial->next = initial;
    initial->prev = initial;

    free_list = NULL;
    insert_into_free_list(initial);

    return start;
}

static void group_destroy(void) {
    free_list = NULL;
    memory_start = NULL;
    total_size = current_blocks = total_allocated = total_freed = 0;
}

static void *group_malloc(size_t size) {
    if (size == 0 || !free_list || current_blocks >= MAX_BLOCKS) return NULL;
    size_t req = ALIGN_UP(size);
    block_header_t *cur = free_list;
    do {
        if (cur->size >= req) {
            size_t rem = cur->size - req;
            if (rem > HEADER_SIZE + MIN_BLOCK_SIZE) {
                block_header_t *nb = (void *)((char *)cur + HEADER_SIZE + req);
                nb->size      = ALIGN_UP(rem - HEADER_SIZE);
                nb->user_size = 0;
                nb->is_free   = true;
                nb->next = cur->next;
                nb->prev = cur;
                cur->next->prev = nb;
                cur->next = nb;
                insert_into_free_list(nb);
                cur->size = req;
            }
            remove_from_free_list(cur);
            cur->user_size = size;
            total_allocated += size;
            current_blocks++;
            return (char *)cur + HEADER_SIZE;
        }
        cur = cur->free_next;
    } while (cur && cur != free_list);
    return NULL;
}

static size_t group_free(void *ptr) {
    if (!ptr) return 0;
    char *base = (char *)memory_start;
    char *p = (char *)ptr;
    if (p < base + HEADER_SIZE || p >= base + total_size) return 0;

    block_header_t *b = (block_header_t *)(p - HEADER_SIZE);
    if (b->is_free || b->user_size == 0) return 0;

    size_t freed_size = b->user_size;
    b->user_size = 0;
    if (b->next && b->next->is_free) {
        remove_from_free_list(b->next);
        block_header_t *n = b->next;
        b->size += HEADER_SIZE + n->size;
        b->next = n->next;
        n->next->prev = b;
    }
    if (b->prev && b->prev->is_free) {
        remove_from_free_list(b->prev);
        block_header_t *pblock = b->prev;
        pblock->size += HEADER_SIZE + b->size;
        pblock->next = b->next;
        b->next->prev = pblock;
        b = pblock;
    }
    insert_into_free_list(b);
    current_blocks--;
    total_freed += freed_size;
    return freed_size;
}

static void group_get_info(memory_info_t *info) {
    if (!info) return;
    info->total_memory    = total_size;
    info->total_allocated = total_allocated;
    info->total_freed     = total_freed;
    info->memory_leak     = (total_allocated != total_freed);

    size_t free_mem = 0;
    size_t free_blocks = 0;
    if (free_list) {
        block_header_t *cur = free_list;
        do {
            free_mem += cur->size + HEADER_SIZE;
            free_blocks++;
            cur = cur->free_next;
        } while (cur != free_list);
    }
    info->free_memory      = free_mem;
    info->free_block_count = free_blocks;
    info->used_memory      = total_size - free_mem;
    info->block_count      = free_blocks + current_blocks;
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