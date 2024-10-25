#include <stdint.h>
#include <stdlib.h>
#include <keyboard.h>
#include <syscallsManager.h>
#include <videoDriver.h>
#include <naiveConsole.h>

extern void getCPURegisters(Reg *regs);
extern void activateSti();
static char myBuffer[100];

void sys_getChar(unsigned int fd, char * buffer, size_t count){
    activateSti();
    while(isBufferEmpty());
    buffer[0] = getBuffer();
    return;
}

void sys_read(unsigned int fd, char * buffer, size_t count) {
return;
}

void sys_write(unsigned int fd, const char *buffer, size_t count) {
    imprimirVideo(buffer,count);
    return;
}

void sys_registers_print(unsigned int fd){
    Reg aux;
    getCPURegisters(&aux);
    printHexaVideo(aux.rbx,myBuffer);
    printHexaVideo(aux.rbx,myBuffer);
    printHexaVideo(aux.rcx,myBuffer);
    printHexaVideo(aux.rdx,myBuffer);
    printHexaVideo(aux.rsi,myBuffer);
    printHexaVideo(aux.rdi,myBuffer);
    printHexaVideo(aux.rbp,myBuffer);
    printHexaVideo(aux.r8,myBuffer);
    printHexaVideo(aux.r9,myBuffer);
    printHexaVideo(aux.r10,myBuffer);
    printHexaVideo(aux.r11,myBuffer);
    printHexaVideo(aux.r12,myBuffer);
    printHexaVideo(aux.r13,myBuffer);
    printHexaVideo(aux.r14,myBuffer);
    printHexaVideo(aux.r15,myBuffer);
}

uint64_t syscallsManager(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx) {
    switch(rdi) {
        case 1: sys_registers_print(rsi); return;
        case 2: sys_getChar(rsi,rdx,rcx); return;
        case 3: sys_read(rsi, rdx, rcx); return;
        case 4: sys_write(rsi, (const char *)rdx, rcx); return;

    }
    return;
}