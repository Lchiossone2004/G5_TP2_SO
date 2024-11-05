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
#define VERDE   0x00FF00
#define TAB "     "

#define STDIN 0
#define STDOUT 1
#define STDERR 2


#define PARA_ALEATORIOS_1 1664525
#define PARA_ALEATORIOS_2 1013904223   

extern void _sti();

static int seed = 0;

void sys_registers_print(unsigned int fd){
    if(fd == STDOUT){
    printRegisters();
    }
    return;
}

void sys_getChar(unsigned int fd, char * letter, size_t count){
    _sti();
    if(fd == STDIN){
    while(isBufferEmpty());
    *letter = getBuffer();
    if(*letter == 0 && count > 0){
        deleteVideo();
    }
    if (*letter == 1)
    {
        nlVideo();
    }
    if(*letter != 0 && *letter != 1){
    imprimirVideo(letter,1,BLANCO);
    }
    }
    return;
}


void sys_read(unsigned int fd, char * buffer, size_t count) {
    if(fd == STDIN){
    if(isBufferEmpty()) {
        return;
    }
    int current = getCurr();
    for(int i = 0; i < count && i <= current ; i++) {
        buffer[i] = getFromBuffer(i);
    }
    }
}

void sys_write(unsigned int fd, const char *buffer, size_t count) {
    if(fd == STDOUT){
    imprimirVideo(buffer,count,BLANCO);
    }
    if(fd == STDERR){
    imprimirVideo(buffer,count,ROJO);
    }
    return;
}

void sys_zoomIn(){
    zoomIN();
    rePrint();
}

void sys_zoomOut(){
    zoomOUT();
    rePrint();
}

void sys_newLine(unsigned int fd){
    if(fd == STDOUT){
    nlVideo();
    }
    return;
}

void sys_sleep(int ticks){
    _sti();
    int max = ticks_elapsed() + ticks;
    while(max > ticks_elapsed()){
        _hlt();
    }
}

void sys_clear(unsigned int fd){
    if(fd == STDOUT){
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


void sys_getTime(unsigned int fd, time * ret, int area) {
    if(fd == STDIN){
    int aux = getHours();
    if(area == 1){
    ret->hours = aux - 3;
    if(ret->hours < 0){
        ret->hours = aux + 0x21;
    }
    }
    else{
        ret->hours = aux;
    }
    ret->mins = getMins();
    ret->sec = getSec();
    }
}
void sys_getKey(unsigned int fd, char* buffer) {
    _sti();
    if(fd == STDIN){
    if(!isBufferEmpty()){
        *buffer = getBuffer();
    }
    }
    return;
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

void sys_clearBuffer(){
    while(!isBufferEmpty()){
        getBuffer();
    }
}

uint64_t syscallsManager(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx) {
    switch(rdi) {
        case 1: sys_registers_print(rsi); break;
        case 2: sys_getChar(rsi,rdx,rcx); break;
        case 3: sys_read(rsi, rdx, rcx); break;
        case 4: sys_write(rsi, rdx, rcx); break;
        case 5: sys_newLine(rsi); break;
        case 6: sys_zoomIn(); break;
        case 7: sys_zoomOut(); break;
        case 8: sys_sleep(rsi); break;
        case 9: sys_clear(rsi); break;
        case 10: sys_putPixel(rsi, rdx, rcx); break;
        case 11: sys_beep(rsi);break;
        case 12: sys_getTime(rsi,rdx,rcx); break;
        case 14: sys_getKey(rsi,rdx); break;
        case 15: sys_ranN(rsi); break;
        case 16: sys_clearBuffer(); break;
    }
    return;
}