#include <stdint.h>
#include <stdlib.h>
#include <keyboard.h>
#include <syscallsManager.h>
#include <videoDriver.h>
#include <naiveConsole.h>


size_t sys_read(unsigned int fd, char *buffer, size_t count) {
    
    // int i = 0;
    // while(buffer_has_next() && i < count) {
    //     buffer[i] = current();
    //     i++;
    // }
    // return i;

}
size_t sys_write(unsigned int fd, const char *buffer, size_t count) {
    imprimirVideo(buffer,count);
return 0;
}

uint64_t syscallsManager(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx) {
    switch(rdi) {
        case 3: sys_read(rsi, rdx, rcx);
        case 4: sys_write(rsi, rdx, rcx);
    }
    

    return 0;

}