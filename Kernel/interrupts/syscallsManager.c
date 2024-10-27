#include <stdint.h>
#include <stdlib.h>
#include <keyboard.h>
#include <syscallsManager.h>
#include <videoDriver.h>
#include <naiveConsole.h>
#include <lib.h>

#define ROJO    0xFF0000
#define BLANCO  0xFFFFFF
extern void getCPURegisters(Reg *regs);
extern void activateSti();

void sys_registers_print(unsigned int fd){
    printHexaVideo(5);
    Reg aux;
    getCPURegisters(&aux);
    printHexaVideo(aux.rbx);
    printHexaVideo(aux.rbx);
    printHexaVideo(aux.rcx);
    printHexaVideo(aux.rdx);
    printHexaVideo(aux.rsi);
    printHexaVideo(aux.rdi);
    printHexaVideo(aux.rbp);
    printHexaVideo(aux.r8);
    printHexaVideo(aux.r9);
    printHexaVideo(aux.r10);
    printHexaVideo(aux.r11);
    printHexaVideo(aux.r12);
    printHexaVideo(aux.r13);
    printHexaVideo(aux.r14);
    printHexaVideo(aux.r15);
    return;
}

void sys_getChar(unsigned int fd, char * buffer, size_t count){
    activateSti();
    while(isBufferEmpty());
    *buffer = getBuffer();
    if(*buffer == 0 && count >= 1){
        deleteVideo();
    }
    if (*buffer == 1)
    {
        nlVideo();
    }
    if(*buffer != 0 && *buffer != 1){
    imprimirVideo(buffer,1,BLANCO);
    }
    return;
}


void sys_read(unsigned int fd, char * buffer, size_t count) {
return;
}

void sys_write(unsigned int fd, const char *buffer, size_t count) {
    if(fd == 1){
    imprimirVideo(buffer,count,BLANCO);
    }
    if(fd == 2){
    imprimirVideo(buffer,count,ROJO);
    }
    return;
}

void sys_zoomIn(unsigned int fd){
    zoomIN();
}

void sys_zoomOut(unsigned int fd){
    zoomOUT();
}

void sys_newLine(){
    nlVideo();
}

void sys_history(unsigned int fd){
    printMatriz();
}

void sys_clear(unsigned int fd){
    if(fd == 1){
         videoClear(768,1024);
    }
}

void sys_putPixel(int posx, int posy, uint32_t hexColor) {
    putPixel(hexColor, posx, posy);
}

void sys_getSecs(uint64_t buffer) {
  buffer = getSec();
}

void sys_getMins(uint64_t buffer) {
   buffer = getMins();
}

void sys_getTime() {
    printTimeVideo(getHours(), getMins(), getSec());
}
void sys_getKey(uint64_t buffer) {
    buffer = getKey();
}

uint64_t syscallsManager(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx) {
    switch(rdi) {
        case 1: sys_registers_print(rsi); return;
        case 2: sys_getChar(rsi,rdx,rcx); return;
        case 3: sys_read(rsi, rdx, rcx); return;
        case 4: sys_write(rsi, (const char *)rdx, rcx); return;
        case 5: sys_newLine(rdi); return;
        case 6: sys_zoomIn(rsi); return;
        case 7: sys_zoomOut(rsi); return;
        case 8: sys_history(rsi); return;
        case 9: sys_clear(rsi); 
        case 10: sys_putPixel(rsi, rdx, rcx); return;
        case 11: sys_getSecs(rsi); return;
        case 12: sys_getMins(rsi); return;
        case 13: sys_getTime(); return;
        case 14: sys_getKey(rsi); return;
    }
    return;
}