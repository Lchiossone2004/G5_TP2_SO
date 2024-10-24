#include <stdint.h>
#include <stdlib.h>
typedef struct {
    uint64_t r15, r14, r13, r12, r11, r10, r9, r8, rsi, rdi, rbp, rdx, rcx, rbx, rax;
} Reg;
size_t sys_read(unsigned int fd, const char * buffer, size_t count);
size_t sys_write(unsigned int fd, const char *buffer, size_t count);
uint64_t sysCallManager(Reg * registers);