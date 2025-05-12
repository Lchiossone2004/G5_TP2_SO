#ifndef SYSCALLS_H
#define SYSCALLS_H

#include <stdint.h>
#include <stdlib.h>
#include "../memory/memory_manager.h"

typedef struct {
    uint64_t rdi, rsi, rdx, rcx, r8, r9;
} Reg;

// Syscall main entry
uint64_t syscallsManager(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8);

// Todas las funciones con firma unificada
void sys_registers_print(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8);
void sys_getChar(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8);
void sys_read(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8);
void sys_write(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8);
void sys_newLine(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8);
void sys_zoomIn(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8);
void sys_zoomOut(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8);
void sys_sleep(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8);
void sys_clear(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8);
void sys_putPixel(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8);
void sys_getTime(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8);
void sys_getKey(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8);
void sys_ranN(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8);
void sys_clearBuffer(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8);
void sys_delete_video(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8);
void sys_test_mm(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8);
void sys_malloc(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8);
void sys_free(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8);
void sys_get_memory_info(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8);

#endif // SYSCALLS_H