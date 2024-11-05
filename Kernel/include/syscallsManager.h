#include <stdint.h>
#include <stdlib.h>


typedef struct{
    char aux;
    char hours;
    char mins;
    char sec;
}time;

typedef struct {
    uint64_t r15, r14, r13, r12, r11, r10, r9, r8, rsi, rdi, rbp, rdx, rcx, rbx, rax;
} Reg;
void sys_registers_print(unsigned int fd);
void sys_getChar(unsigned int fd, char * buffer, size_t count);
void sys_read(unsigned int fd, char * buffer, size_t count);
void sys_write(unsigned int fd, const char *buffer, size_t count);
void sys_newLine();
void sysCallManager(Reg * registers);
void sys_zoomIn();
void sys_zoomOut();
void videoClear();
void sys_sleep(int seconds);
void sys_getSecs(uint64_t buffer);
void sys_getMins(uint64_t buffer);
void sys_getTime(unsigned int fd, time * ret, int area);
void sys_getKey(unsigned int fd, char* buffer);
void sys_ranN(int* toRan);
void sys_clearBuffer();