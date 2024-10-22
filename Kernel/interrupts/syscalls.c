#include <stdint.h>
#include <stdlib.h>
//uint64_t sysCallManager()
typedef struct {
    uint64_t r15, r14, r13, r12, r11, r10, r9, r8, rsi, rdi, rbp, rdx, rcx, rbx, rax;
} Reg;

size_t sys_read(unsigned int fd, char *buffer, size_t count) {
    
    int i = 0;
    while(buffer_has_next() && i < count) {
        buffer[i] = current();
        i++;
    }
    return i;
//OJO FALTA IMPLEMENTAR EL KEYBOARD MANAGER (driver)
}
size_t sys_write(unsigned int fd, const char *buffer, size_t count) {

}