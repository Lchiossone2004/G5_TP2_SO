#include <memory-lib.h>
#include <stdlib.h>
#include <stdint.h>
#include <string-lib.h>
#include <c-lib.h>

extern uint64_t syscall(uint64_t rdi, ...);

void* usr_malloc(int size){
    void *ptr;
    syscall(19, &ptr, size);
    return ptr;
}

void usr_free(void* ptr){
    syscall(20,ptr);
}

void print_usr_mem_info(){
    memory_info_t info;
    syscall(21,&info);
    char buffer[100];

    size_t_to_str(info.current_blocks, buffer);
    print("Current Blocks: ");
    print(buffer);
    nlPrint();

    size_t_to_str(info.block_count, buffer);
    print("Block Count: ");
    print(buffer);
    nlPrint();

    size_t_to_str(info.free_block_count, buffer);
    print("Free Block Count: ");
    print(buffer);
    nlPrint();

    size_t_to_str(info.free_memory, buffer);
    print("Free Memory: ");
    print(buffer);
    nlPrint();

    size_t_to_str(info.total_allocated, buffer);
    print("Total Allocated: ");
    print(buffer);
    nlPrint();
}