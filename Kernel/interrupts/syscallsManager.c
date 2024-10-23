#include <stdint.h>
#include <stdlib.h>
#include <keyboard.h>
#include <syscallsManager.h>
#include <videoDriver.h>
#include <naiveConsole.h>


size_t sys_read(unsigned int fd, char *buffer, size_t count) {
    
    int i = 0;
    while(buffer_has_next() && i < count) {
        buffer[i] = current();
        i++;
    }
    return i;

}
size_t sys_write(unsigned int fd, const char *buffer, size_t count) {
    imprimirVideo(buffer, count);
return 0;
}

uint64_t syscallsManager(Reg * registers) {
    //switch(registers->rax) {
    //    case 3: sys_read(registers->rbx, registers->rcx, registers->rdx);
    //    case 4: sys_write(registers->rbx, registers->rcx, registers->rdx);
    //}
    ncPrint("Hola");

    return 0;

}