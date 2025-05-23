#include "group_memory_manager.h"
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

typedef struct block_header
{
    size_t size;
    bool is_free;
    struct block_header *next;
    struct block_header *prev;
} block_header_t;

static block_header_t *free_list = NULL;
static void *memory_start;
static size_t total_size;
static size_t current_blocks = 0;
static size_t total_allocated = 0;
static size_t total_freed = 0;

static void *group_init(void *start, size_t size)
{
    memory_start = start;
    total_size = size;
    block_header_t *initial_block = (block_header_t *)start;
    initial_block->size = size - sizeof(block_header_t);
    initial_block->is_free = true;
    initial_block->next = NULL;
    initial_block->prev = NULL;

    free_list = initial_block;
    return start;
}

static void *group_malloc(size_t size)
{
    if (size == 0 || current_blocks >= MAX_BLOCKS)
        return NULL;
    block_header_t *current = free_list;
    while (current != NULL)
    {
        if (current->is_free && current->size >= size)
        {
            if (current->size > size + sizeof(block_header_t) + MIN_BLOCK_SIZE)
            {
                block_header_t *new_block = (block_header_t *)((char *)current + sizeof(block_header_t) + size);
                new_block->size = current->size - size - sizeof(block_header_t);
                new_block->is_free = true;
                new_block->next = current->next;
                new_block->prev = current;
                if (current->next)
                {
                    current->next->prev = new_block;
                }
                current->next = new_block;
                current->size = size;
            }

            current->is_free = false;
            current_blocks++;
            total_allocated += current->size;
            return (void *)((char *)current + sizeof(block_header_t));
        }
        current = current->next;
    }

    return NULL;
}

static size_t group_free(void *ptr)
{
    if (ptr == NULL)
        return 0;

    block_header_t *block = (block_header_t *)((char *)ptr - sizeof(block_header_t));
    block->is_free = true;
    size_t block_size = block->size;
    total_freed += block_size;
    if (block->next && block->next->is_free)
    {
        block->size += sizeof(block_header_t) + block->next->size;
        block->next = block->next->next;
        if (block->next)
        {
            block->next->prev = block;
        }
    }

    if (block->prev && block->prev->is_free)
    {
        block->prev->size += sizeof(block_header_t) + block->size;
        block->prev->next = block->next;
        if (block->next)
        {
            block->next->prev = block->prev;
        }

        block = block->prev;
    }

    current_blocks--;
    return block_size;
}

static void group_get_info(memory_info_t *info)
{
    if (info == NULL)
        return;

    info->total_memory = total_size;
    info->current_blocks = current_blocks;
    info->free_block_count = 0;
    info->free_memory = 0;
    info->total_allocated = total_allocated;
    info->total_freed = total_freed;
    info->memory_leak = (total_allocated != total_freed);

    block_header_t *current = free_list;
    while (current != NULL)
    {
        if (current->is_free)
        {
            info->free_memory += current->size;
            info->free_block_count++;
        }
        current = current->next;
    }

    info->used_memory = total_size - info->free_memory;
    info->block_count = info->free_block_count + info->current_blocks;
}

static memory_manager_t group_manager = {
    .init = group_init,
    .malloc = group_malloc,
    .free = group_free,
    .get_info = group_get_info};

memory_manager_t *get_group_memory_manager(void)
{
    return &group_manager;
}