#include <memory-lib.h>
#include <stdlib.h>
#include <stdint.h>
#include <string-lib.h>
#include <c-lib.h>

#define TAB "   "
extern uint64_t syscall(uint64_t rdi, ...);

void* usr_malloc(int size){
    void *aux = (void*) syscall(10, size);
    return aux;
}

void usr_free(void* ptr){
    syscall(11,ptr);
}

void print_usr_mem_info(){
    memory_info_t info;
    syscall(12,&info);
    char buffer[100];

    size_t_to_str(info.current_blocks, buffer);
    print(TAB);
    print("Current Blocks: ");
    print(buffer);
    print("\n");

    size_t_to_str(info.block_count, buffer);
     print(TAB);
    print("Block Count: ");
    print(buffer);
    print("\n");

    size_t_to_str(info.free_block_count, buffer);
    print(TAB);
    print("Free Block Count: ");
    print(buffer);
    print("\n");

    size_t_to_str(info.free_memory, buffer);
    print(TAB);
    print("Free Memory: ");
    print(buffer);
    print("\n");

    size_t_to_str(info.total_allocated, buffer);
    print(TAB);
    print("Total Allocated: ");
    print(buffer);
    print("\n");
}
void usr_memset(void * destination, int32_t c, uint64_t length) {

}