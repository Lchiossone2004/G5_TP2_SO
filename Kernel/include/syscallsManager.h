#include <stdint.h>
#include <stdlib.h>

typedef struct {
    uint64_t rdi, rsi, rdx, rcx, r8, r9;
} Reg;
void sys_registers_print(unsigned int fd);
void sys_getChar(unsigned int fd, char * buffer, size_t count);
void sys_read(unsigned int fd, char * buffer, size_t count);
void sys_write(unsigned int fd, const char *buffer, size_t count);
void sys_newLine(unsigned int fd);
void sysCallManager(Reg * registers);
void sys_zoomIn();
void sys_zoomOut();
void videoClear();
void sys_sleep(int seconds);
void sys_getSecs(uint64_t buffer);
void sys_getMins(uint64_t buffer);
void sys_getTime(unsigned int fd, char * ret);
void sys_getKey(unsigned int fd, char* buffer);
void sys_ranN(int* toRan);
void sys_clearBuffer();