#include <stdint.h>
#include <stdlib.h>
#include <keyboard.h>
#include <syscallsManager.h>
#include <videoDriver.h>
#include <naiveConsole.h>

void sys_getChar(unsigned int fd, char * buffer, size_t count){
        buffer[0]= getFromBuffer();
        buffer[1]= 0;
        return;
}

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

void syscallsManager(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx) {
    switch(rdi) {
        case 1: sys_getChar(rsi,rdx,rcx);
        case 3: sys_read(rsi, rdx, rcx);
        case 4: sys_write(rsi, (const char *)rdx, rcx);

    }
    return;

}