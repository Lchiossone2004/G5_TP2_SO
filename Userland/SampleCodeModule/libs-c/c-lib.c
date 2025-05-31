#include <c-lib.h>
#include <stdlib.h>
#include <stdint.h>
#include <string-lib.h>

#define STDIN 0
#define STDOUT 1
#define STDERR 2

extern uint64_t syscall(uint64_t rdi, ...);

void printRegisters()
{
    syscall(1, STDOUT);
}

void print(char *word)
{
    syscall(4, STDOUT, word, strSize(word));
}

void read(char *buffer, int size)
{
    syscall(3, STDOUT, buffer, size);
    return;
}

void printErr(char *word)
{
    syscall(4, STDERR, word, strSize(word));
}

void nlPrint()
{
    syscall(5, STDOUT);
}

void sleep(int ticks)
{
    syscall(8, ticks);
    return;
}
int sem_open_lib(int id, int initial_value) 
{
    return syscall(33, id, initial_value);
}

int sem_close_lib(int id) 
{
    return syscall(34, id);
}

int sem_wait_lib(int id) 
{
    return syscall(35, id);
}

int sem_post_lib(int id) 
{
    return syscall(36, id);
}

int sem_getvalue_lib(int id) 
{
    return syscall(37, id);
}
