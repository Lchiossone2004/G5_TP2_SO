#include "buddy_system.h"
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

typedef struct block_header {
    size_t size;
    bool is_free;
    struct block_header* next;
} block_header_t;

static block_header_t* free_blocks[MAX_ORDER + 1];
static void* memory_start;
static size_t total_size;
static size_t current_blocks = 0;

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
    
    // Inicializar lista de bloques libres
    for (int i = 0; i <= MAX_ORDER; i++) {
        free_blocks[i] = NULL;
    }
    
    // Crear bloque inicial
    block_header_t* initial_block = (block_header_t*)start;
    initial_block->size = size - sizeof(block_header_t);
    initial_block->is_free = true;
    initial_block->next = NULL;
    
    size_t order = get_order(initial_block->size);
    free_blocks[order] = initial_block;
    
    return start;
}

static void* buddy_malloc(size_t size) {
    if (size == 0) return NULL;
    
    size_t required_size = size + sizeof(block_header_t);
    size_t order = get_order(required_size);
    
    // Buscar el primer bloque libre del orden requerido o superior
    size_t current_order = order;
    while (current_order <= MAX_ORDER && free_blocks[current_order] == NULL) {
        current_order++;
    }
    
    if (current_order > MAX_ORDER) return NULL;
    
    // Tomar el primer bloque disponible
    block_header_t* block = free_blocks[current_order];
    free_blocks[current_order] = block->next;
    
    // Dividir el bloque si es necesario
    while (current_order > order) {
        current_order--;
        size_t split_size = (1 << current_order) * MIN_BLOCK_SIZE;
        
        block_header_t* buddy = (block_header_t*)((char*)block + split_size);
        buddy->size = split_size - sizeof(block_header_t);
        buddy->is_free = true;
        buddy->next = free_blocks[current_order];
        free_blocks[current_order] = buddy;
        
        block->size = split_size - sizeof(block_header_t);
    }
    
    block->is_free = false;
    block->next = NULL;
    current_blocks++;
    
    return (void*)((char*)block + sizeof(block_header_t));
}

static void buddy_free(void* ptr) {
    if (ptr == NULL) return;
    
    block_header_t* block = (block_header_t*)((char*)ptr - sizeof(block_header_t));
    block->is_free = true;
    current_blocks--;
    
    size_t order = get_order(block->size);
    
    // Intentar unir con el buddy
    while (order < MAX_ORDER) {
        size_t block_size = (1 << order) * MIN_BLOCK_SIZE;
        uintptr_t block_addr = (uintptr_t)block;
        uintptr_t buddy_addr = block_addr ^ block_size;
        
        block_header_t* buddy = (block_header_t*)buddy_addr;
        
        if (!buddy->is_free || buddy->size != block->size) {
            break;
        }
        
        // Remover buddy de la lista de libres
        block_header_t** prev = &free_blocks[order];
        while (*prev != buddy) {
            prev = &(*prev)->next;
        }
        *prev = buddy->next;
        
        // Unir los bloques
        if (block_addr > buddy_addr) {
            block = buddy;
        }
        
        order++;
        block->size = (1 << order) * MIN_BLOCK_SIZE - sizeof(block_header_t);
    }
    
    // Agregar el bloque a la lista de libres
    block->next = free_blocks[order];
    free_blocks[order] = block;
}

static void buddy_dump(void) {
    memory_info_t info = {0};
    info.total_memory = total_size;
    
    // Contar bloques libres y memoria libre
    for (int i = 0; i <= MAX_ORDER; i++) {
        block_header_t* current = free_blocks[i];
        while (current != NULL) {
            info.free_memory += current->size;
            info.free_block_count++;
            current = current->next;
        }
    }
    
    info.used_memory = total_size - info.free_memory;
    info.block_count = info.free_block_count + current_blocks;
    
    // Aquí podrías imprimir la información si lo deseas
}

static void buddy_get_info(memory_info_t* info) {
    if (info == NULL) return;
    
    info->total_memory = total_size;
    info->block_count = current_blocks;
    info->free_block_count = 0;
    info->free_memory = 0;
    
    // Contar bloques libres y memoria libre
    for (int i = 0; i <= MAX_ORDER; i++) {
        block_header_t* current = free_blocks[i];
        while (current != NULL) {
            info->free_memory += current->size;
            info->free_block_count++;
            current = current->next;
        }
    }
    
    info->used_memory = total_size - info->free_memory;
}

static memory_manager_t buddy_manager = {
    .init = buddy_init,
    .malloc = buddy_malloc,
    .free = buddy_free,
    .dump = buddy_dump,
    .get_info = buddy_get_info
};

memory_manager_t* get_buddy_system_manager(void) {
    return &buddy_manager;
} 