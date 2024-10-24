#include <stdint.h>
#include <stdlib.h>
#include <keyboard.h>
#include <syscallsManager.h>
#include <videoDriver.h>
#include <naiveConsole.h>

static char* savedLetter;

size_t sys_read(unsigned int fd, const char * buffer, size_t count) {
    *savedLetter=toLetter(buffer);
    *(savedLetter+1)=0;
    return 0;
}


size_t sys_write(unsigned int fd, const char *buffer, size_t count) {
    imprimirVideo(buffer,count);
return 0;
}

uint64_t syscallsManager(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx) {
    switch(rdi) {
        case 3: sys_read(rsi, rdx, rcx);
        case 4: sys_write(rsi, (const char *)rdx, rcx);

    }
    return 0;

}