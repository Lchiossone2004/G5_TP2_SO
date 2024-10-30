#include <stdint.h>
#include <stdlib.h>
#include <keyboard.h>
#include <syscallsManager.h>
#include <videoDriver.h>
#include <naiveConsole.h>
#include <lib.h>
#include <time.h>
#include <interrupts.h>
#include <sound.h>

#define ROJO    0xFF0000
#define BLANCO  0xFFFFFF

#define PARA_ALEATORIOS_1 1664525
#define PARA_ALEATORIOS_2 1013904223   

extern uint64_t * getCPURegisters();
extern void _sti();

static int seed = 0;

static char * regs[8] = {"RAX: ", "RBX: ", "RCX: ", "RDX: ", "RSI: ","RDI: ", "RSP: ","RBP: "};

void sys_registers_print(unsigned int fd){
    uint64_t * registers = getCPURegisters();
    for(int i = 0; i<8; i++){
    imprimirVideo(regs[i],5,BLANCO);
    printHexaVideo(registers[i]);
    nlVideo();
    }
    return;
}

void sys_getChar(unsigned int fd, char * buffer, size_t count){
    _sti();
    while(isBufferEmpty());
    *buffer = getBuffer();
    if(*buffer == 0 && count > 0){
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
    rePrint();
}

void sys_zoomOut(unsigned int fd){
    zoomOUT();
    rePrint();
}

void sys_newLine(){
    nlVideo();
}

void sys_sleep(int ticks){
    _sti();
    int max = ticks_elapsed() + ticks;
    while(max > ticks_elapsed());
}

void sys_clear(unsigned int fd){
    if(fd == 1){
        videoClear();
    }
}

void sys_putPixel(int posx, int posy, uint32_t hexColor) {
    putPixel(hexColor, posx, posy);
}
void sys_beep(int flag) {
    _sti();
    beep(flag);
}


void sys_getTime(time * ret) {
    ret->hours = getHours();
    ret->mins = getMins();
    ret->sec = getSec();
}
void sys_getKey(int* buffer) {
    _sti();
    *buffer = getKey();
}
void seed_changer() {
    seed = (getHours() * 3600 + getMins() * 60 + getSec());
}
void sys_ranN(int* toRan){
    seed_changer();
    *toRan = (PARA_ALEATORIOS_1 * seed + PARA_ALEATORIOS_2);
    if(*toRan<0){
        *toRan=-*toRan;
    }
    return;
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
        case 8: sys_sleep(rsi); return;
        case 9: sys_clear(rsi); return;
        case 10: sys_putPixel(rsi, rdx, rcx); return;
        case 11: sys_beep(rsi);return;
        case 13: sys_getTime(rsi); return;
        case 14: sys_getKey(rsi); return;
        case 15: sys_ranN(rsi); return;
    }
    return;
}