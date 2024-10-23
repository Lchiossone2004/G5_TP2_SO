#include "../include/libc.h"
#include <stdlib.h>
#include <stdint.h>

extern uint64_t syscall(uint64_t rdi, ...);

static char buffer[100];

void printf(char * word){
    syscall(3);
}