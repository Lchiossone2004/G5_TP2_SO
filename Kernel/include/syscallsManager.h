#ifndef SYSCALLS_H
#define SYSCALLS_H

#include <stdint.h>
#include <stdlib.h>
#include "../memory/memory_manager.h"
#include "../include/keyboard.h"

typedef struct {
    uint64_t rdi, rsi, rdx, rcx, r8, r9;
} Reg;

// Syscall main entry
uint64_t syscallsManager(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9);

// Todas las funciones con firma unificada
uint64_t sys_registers_print(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8);
uint64_t sys_getChar(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8);
uint64_t sys_read(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8);
uint64_t sys_write(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8);
uint64_t sys_newLine(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8);
uint64_t sys_zoomIn(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8);
uint64_t sys_zoomOut(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8);
uint64_t sys_sleep(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8);
uint64_t sys_clear(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8);
uint64_t sys_putPixel(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8);
uint64_t sys_getTime(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8);
uint64_t sys_getKey(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8);
uint64_t sys_ranN(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8);
uint64_t sys_clearBuffer(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8);
uint64_t sys_delete_video(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8);
uint64_t sys_test_mm(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8);
uint64_t sys_malloc(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8);
uint64_t sys_free(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8);
uint64_t sys_get_memory_info(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8);
uint64_t sys_create(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9);

#endif // SYSCALLS_H