#include <stdint.h>
#include <stdlib.h>
#include <keyboard.h>
#include <syscallsManager.h>
#include <videoDriver.h>
#include <naiveConsole.h>

extern void getCPURegisters(Reg *regs);

void sys_read(unsigned int fd, char * buffer, size_t count) {
    buffer[0] = 'a';
    buffer[1] = 'b';
    buffer[2] = 0;
return;
}

void sys_write(unsigned int fd, const char *buffer, size_t count) {
    imprimirVideo(buffer,count);
    return;
}

void sys_registers_print(){
    Reg aux;
    getCPURegisters(&aux);
    sys_write(1,aux.rbx,8);
    imprimirVideo(aux.rbx,8);
    imprimirVideo(aux.rcx,8);
    imprimirVideo(aux.rdx,8);
    imprimirVideo(aux.rsi,8);
    imprimirVideo(aux.rdi,8);
    imprimirVideo(aux.rbp,8);
    imprimirVideo(aux.r8,8);
    imprimirVideo(aux.r9,8);
    imprimirVideo(aux.r10,8);
    imprimirVideo(aux.r11,8);
    imprimirVideo(aux.r12,8);
    imprimirVideo(aux.r13,8);
    imprimirVideo(aux.r14,8);
    imprimirVideo(aux.r15,8);
}

uint64_t syscallsManager(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx) {
    switch(rdi) {
        case 1: sys_registers_print();
        case 3: sys_read(rsi, rdx, rcx);
        case 4: sys_write(rsi, (const char *)rdx, rcx);

    }
    return;
}