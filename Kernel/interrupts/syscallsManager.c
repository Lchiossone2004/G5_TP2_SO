#include <stdint.h>
#include <stdlib.h>
#include <keyboard.h>
#include <videoDriver.h>
#include <naiveConsole.h>
#include <lib.h>
#include <time.h>
#include <interrupts.h>
#include <sound.h>
#include <syscallsManager.h>
#include "../include/process.h"

#define ROJO    0xFF0000
#define BLANCO  0xFFFFFF
#define VERDE   0x00FF00
#define TAB "     "

#define STDIN 0
#define STDOUT 1
#define STDERR 2

#define PARA_ALEATORIOS_1 1664525
#define PARA_ALEATORIOS_2 1013904223   

static size_t total_allocated = 0;
static size_t total_freed = 0;
static size_t current_blocks = 0;

typedef uint64_t (*syscall_fn)(uint64_t, uint64_t, uint64_t, uint64_t);

extern void _sti();

static int seed = 0;


// Array de punteros a funciones, indexado por syscall ID
static syscall_fn syscall_table[] = {
    [1] = sys_registers_print,
    [2] = sys_getChar,
    [3] = sys_read,
    [4] = sys_write,
    [5] = sys_newLine,
    [6] = sys_zoomIn,
    [7] = sys_zoomOut,
    [8] = sys_sleep,
    [9] = sys_clear,
    [10] = sys_putPixel,
    [12] = sys_getTime,
    [14] = sys_getKey,
    [15] = sys_ranN,
    [16] = sys_clearBuffer,
    [17] = sys_delete_video,
    [18] = sys_test_mm,
    [19] = sys_malloc,
    [20] = sys_free,
    [21] = sys_get_memory_info
};

#define SYSCALL_TABLE_SIZE (sizeof(syscall_table) / sizeof(syscall_fn))

uint64_t syscallsManager(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8) {
    if (rdi < SYSCALL_TABLE_SIZE && syscall_table[rdi]) {
        syscall_table[rdi](rsi, rdx, rcx, r8);
    }
    return 0;
}

uint64_t sys_registers_print(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8){
    unsigned int fd = (unsigned int) rsi;
    if(fd == STDOUT){
        printRegisters();
    }
    return 0;
}

uint64_t sys_getChar(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8){
    unsigned int fd = (unsigned int) rsi;
    char *letter = (char *) rdx;
    size_t count = (size_t) rcx;

    _sti();
    if(fd == STDIN){
        while(isBufferEmpty());
        *letter = getBuffer();
        if(*letter == 0 && count > 0){
            deleteVideo();
        }
        if (*letter == 1) {
            nlVideo();
        }
        if(*letter != 0 && *letter != 1){
            imprimirVideo(letter, 1, BLANCO);
        }
    }
    return 0;
}

uint64_t sys_read(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8) {
    unsigned int fd = (unsigned int) rsi;
    char *buffer = (char *) rdx;
    size_t count = (size_t) rcx;

    if(fd == STDIN){
        if(isBufferEmpty()) {
            return 0;
        }
        int current = getCurr();
        for(int i = 0; i < count && i <= current ; i++) {
            buffer[i] = getFromBuffer(i);
        }
    }
    return 0;
}

uint64_t sys_write(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8) {
    unsigned int fd = (unsigned int) rsi;
    char *buffer = (char *) rdx;
    size_t count = (size_t) rcx;

    if(fd == STDOUT){
        imprimirVideo(buffer, count, BLANCO);
    }
    if(fd == STDERR){
        imprimirVideo(buffer, count, ROJO);
    }
    return 0;
}

static void counter(){
    int i = 0;
    while(1){
        i++;
    }
}


uint64_t sys_zoomIn(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8){ 
    unsigned int fd = (unsigned int) rsi;
    if(fd == STDOUT) {
        zoomIN();
        rePrint();
    }
    create_process(counter);
    return 0;
}

uint64_t sys_zoomOut(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8){
    unsigned int fd = (unsigned int) rsi;
    if(fd == STDOUT) {
        zoomOUT();
        rePrint();
    }
    return 0;
}

uint64_t sys_newLine(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8){
    unsigned int fd = (unsigned int) rsi;
    if(fd == STDOUT){
        nlVideo();
    }
    return 0;
}

uint64_t sys_sleep(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8){
    int ticks = (int) rsi;

    _sti();
    int max = ticks_elapsed() + ticks;
    while(max > ticks_elapsed()){
        _hlt();
    }
    return 0;
}

uint64_t sys_clear(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8){
    unsigned int fd = (unsigned int) rsi;
    if(fd == STDOUT){
        videoClear();
    }
    return 0;
}

uint64_t sys_putPixel(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8) {
    int posx = (int) rsi;
    int posy = (int) rdx;
    uint32_t color = (uint32_t) rcx;
    unsigned int fd = (unsigned int) r8;

    if(fd == STDOUT) {
        putPixel(color, posx, posy);
    }
    return 0;
}

uint64_t sys_getTime(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8) {
    unsigned int fd = (unsigned int) rsi;
    char *ret = (char *) rdx;

    if(fd == STDIN){
        ret[0] = getHours();
        ret[1] = getMins();
        ret[2] = getSec();
    }
    return 0;
}

uint64_t sys_getKey(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8) {
    unsigned int fd = (unsigned int) rsi;
    char *buffer = (char *) rdx;

    _sti();
    if(fd == STDIN && !isBufferEmpty()){
        *buffer = getBuffer();
    }
    return 0;
}

uint64_t seed_changer() {
    seed = (getHours() * 3600 + getMins() * 60 + getSec());
    return 0;
}

uint64_t sys_ranN(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8){
    int *toRan = (int *) rsi;
    seed_changer();
    *toRan = (PARA_ALEATORIOS_1 * seed + PARA_ALEATORIOS_2);
    if(*toRan < 0){
        *toRan = -*toRan;
    }
    return 0;
}

uint64_t sys_clearBuffer(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8){
    while(!isBufferEmpty()){
        getBuffer();
    }
    return 0;
}

uint64_t sys_delete_video(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8){
    unsigned int cant = (unsigned int) rsi;
    for(int i = 0; i < cant - 1; i++){
        deleteVideo();
    }
    return 0;
}

uint64_t sys_test_mm(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8){
    // aún no implementada
    return 0;
}

uint64_t sys_malloc(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8){
    void **ptr = (void **) rsi;
    size_t size = (size_t) rdx;

    if (size == 0) return 0;

    *ptr = mm_malloc(size);
    if (*ptr != NULL) {
        total_allocated += size;
        current_blocks++;
    }
    return 0;
}

uint64_t sys_free(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8){
    void *ptr = (void *) rsi;
    if (ptr == NULL) return 0;

    size_t freed_size = mm_free(ptr);
    total_allocated -= freed_size;
    total_freed += freed_size;
    current_blocks--;
    return 0;
}

uint64_t sys_get_memory_info(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8){
    memory_info_t *info = (memory_info_t *) rsi;
    if (info == NULL) return 0;

    mm_get_info(info);
    info->total_allocated = total_allocated;
    info->total_freed = total_freed;
    info->current_blocks = current_blocks;
    info->memory_leak = (total_allocated > total_freed);
    return 0;
}

uint64_t sys_create(){
    return 0;
    //create_process(sys_clear);
}