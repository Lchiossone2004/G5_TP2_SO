#include <stdint.h>
#include <stdlib.h>
typedef struct {
    uint64_t r15, r14, r13, r12, r11, r10, r9, r8, rsi, rdi, rbp, rdx, rcx, rbx, rax;
} Reg;
void sys_getChar(unsigned int fd, char * buffer, size_t count);
void sys_read(unsigned int fd, char * buffer, size_t count);
void sys_write(unsigned int fd, const char *buffer, size_t count);
void sys_registers_print();
void sysCallManager(Reg * registers);