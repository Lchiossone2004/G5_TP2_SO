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
#include "../include/scheduler.h"
#include "../include/sem.h"
#include "../include/pipe.h"

#define ROJO    0xFF0000
#define BLANCO  0xFFFFFF
#define VERDE   0x00FF00
#define TAB "     "
#define PIPE_FD_START 3

#define STDIN 0
#define STDOUT 1
#define STDERR 2

#define PARA_ALEATORIOS_1 1664525
#define PARA_ALEATORIOS_2 1013904223   
#define EOF 4

static size_t total_allocated = 0;
static size_t total_freed = 0;
static size_t current_blocks = 0;

typedef uint64_t (*syscall_fn)(uint64_t, uint64_t, uint64_t, uint64_t, uint64_t, uint64_t);

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
    [13] = sys_getTime,
    [14] = sys_getKey,
    [15] = sys_ranN,
    [16] = sys_clearBuffer,
    [17] = sys_delete_video,
    [18] = sys_test_mm,
    [19] = sys_malloc,
    [20] = sys_free,
    [21] = sys_get_memory_info,
    [22] = sys_create,
    [23] = sys_kill,
    [24] = sys_getPid,
    [25] = sys_endProcess,
    [26] = sys_modifyPriority,
    [27] = sys_block,
    [28] = sys_unblock,
    [29] = sys_getProcesses,
    [30] = sys_fork,
    [31] = sys_quitCPU,
    [32] = sys_wait,
    [33] = sys_get_foreground,
    [34] = sys_sem_open,
    [35] = sys_sem_close,
    [36] = sys_sem_wait,
    [37] = sys_sem_post,
    [38] = sys_sem_get_value,
    [39] = sys_go_middle,
    [40] = sys_create_pipe,
    [41] = sys_print,
    [42] = sys_readLine
};

#define SYSCALL_TABLE_SIZE (sizeof(syscall_table) / sizeof(syscall_fn))

uint64_t syscallsManager(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9, uint64_t r10) {
    if (rdi < SYSCALL_TABLE_SIZE && syscall_table[rdi]) {
        return syscall_table[rdi](rsi, rdx, rcx, r8, r9, r10); 
    }
    return 0;
}

uint64_t sys_registers_print(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9, uint64_t r10 ){
    unsigned int fd = (unsigned int) rsi;
    if(fd == STDOUT){
        printRegisters();
    }
    return 0;
}

uint64_t sys_getChar(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9, uint64_t r10){
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

uint64_t sys_read(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9, uint64_t r10) {
    unsigned int fd = (unsigned int) rsi;
    char *buffer = (char *) rdx;
    size_t count = (size_t) rcx;

    if (fd == STDIN) {
    p_info *proc = get_current_process();
    int actual_stdin = proc ? proc->stdin : STDIN;

    if (actual_stdin >= PIPE_FD_START) {
        return pipe_read(actual_stdin, buffer, count);
    }

    // Si no está redirigido, seguir leyendo del buffer del teclado
    if (isBufferEmpty()) return 0;

    int current = getCurr();
    size_t bytesToRead = (count <= current + 1) ? count : current + 1;

    for (size_t i = 0; i < bytesToRead; i++) {
        buffer[i] = getFromBuffer(i);
    }

    while (!isBufferEmpty()) {
        getBuffer();
    }

    return bytesToRead;
}

    else if (fd >= PIPE_FD_START) {
        return pipe_read(fd, buffer, count);  
    }

    return -1;  
}


uint64_t sys_write(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9, uint64_t r10) {
    unsigned int fd = (unsigned int) rsi;
    char *buffer = (char *) rdx;
    size_t count = (size_t) rcx;

    if(fd == STDOUT){
        imprimirVideo(buffer, count, BLANCO);
    }
    else if(fd == STDERR){
        imprimirVideo(buffer, count, ROJO);
    } 
    else if (fd >= PIPE_FD_START) {
        pipe_write(fd,buffer,count);
    }

    return 0;
}

uint64_t sys_zoomIn(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9, uint64_t r10){ 
    unsigned int fd = (unsigned int) rsi;
    if(fd == STDOUT) {
        zoomIN();
        rePrint();
    }
    return 0;
}

uint64_t sys_zoomOut(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9, uint64_t r10){
    unsigned int fd = (unsigned int) rsi;
    if(fd == STDOUT) {
        zoomOUT();
        rePrint();
    }
    return 0;
}

uint64_t sys_newLine(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9, uint64_t r10){
    unsigned int fd = (unsigned int) rsi;
    if(fd == STDOUT){
        nlVideo();
    }
    return 0;
}

uint64_t sys_sleep(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9, uint64_t r10){
    int ticks = (int) rsi;

    _sti();
    int max = ticks_elapsed() + ticks;
    while(max > ticks_elapsed()){
        _hlt();
    }
    return 0;
}

uint64_t sys_clear(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9, uint64_t r10){
    unsigned int fd = (unsigned int) rsi;
    if(fd == STDOUT){
        videoClear();
    }
    return 0;
}

uint64_t sys_putPixel(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9, uint64_t r10) {
    int posx = (int) rsi;
    int posy = (int) rdx;
    uint32_t color = (uint32_t) rcx;
    unsigned int fd = (unsigned int) r8;

    if(fd == STDOUT) {
        putPixel(color, posx, posy);
    }
    return 0;
}

uint64_t sys_getTime(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9, uint64_t r10) {
    unsigned int fd = (unsigned int) rsi;
    char *ret = (char *) rdx;

    if(fd == STDIN){
        ret[0] = getHours();
        ret[1] = getMins();
        ret[2] = getSec();
    }
    return 0;
}

uint64_t sys_getKey(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9, uint64_t r10) {
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

uint64_t sys_ranN(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9, uint64_t r10){
    int *toRan = (int *) rsi;
    seed_changer();
    *toRan = (PARA_ALEATORIOS_1 * seed + PARA_ALEATORIOS_2);
    if(*toRan < 0){
        *toRan = -*toRan;
    }
    return 0;
}

uint64_t sys_clearBuffer(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9, uint64_t r10){
    while(!isBufferEmpty()){
        getBuffer();
    }
    return 0;
}

uint64_t sys_delete_video(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9, uint64_t r10){
    unsigned int cant = (unsigned int) rsi;
    for(int i = 0; i < cant - 1; i++){
        deleteVideo();
    }
    return 0;
}

uint64_t sys_test_mm(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9, uint64_t r10){
    // aún no implementada
    return 0;
}

uint64_t sys_malloc(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9, uint64_t r10){
    size_t size = (size_t) rdx;

    if (size == 0) return 0;

    void* ptr = mm_malloc(size);
    if (ptr != NULL) {
        total_allocated += size;
        current_blocks++;
    }
    return ptr;
}

uint64_t sys_free(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9, uint64_t r10){
    void *ptr = (void *) rsi;
    if (ptr == NULL) return 0;

    size_t freed_size = mm_free(ptr);
    total_allocated -= freed_size;
    total_freed += freed_size;
    current_blocks--;
    return 0;
}

uint64_t sys_get_memory_info(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9, uint64_t r10){
    memory_info_t *info = (memory_info_t *) rsi;
    if (info == NULL) return 0;

    mm_get_info(info);
    info->total_allocated = total_allocated;
    info->total_freed = total_freed;
    info->current_blocks = current_blocks;
    info->memory_leak = (total_allocated > total_freed);
    return 0;
}


uint64_t sys_create(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9, uint64_t r10) {
    return createProcess((void (*)(uint8_t, char**))rsi, (uint8_t)rdx, (char**)rcx, (char*)r8, (int)r9, (int)r10);
}
uint64_t sys_kill(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9, uint64_t r10) {
    return kill_process(rsi);
}
uint64_t sys_getPid(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9, uint64_t r10) {
    return get_pid();
}
uint64_t sys_endProcess(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9, uint64_t r10) {
    exit_process();
    return 0;
}
uint64_t sys_modifyPriority(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9, uint64_t r10){
    return modify_priority((uint16_t)rsi, (int)rdx);
}
uint64_t sys_block(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9, uint64_t r10) {
    return block_process((uint16_t)rsi);
}
uint64_t sys_unblock(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9, uint64_t r10) {
    return unblock_process((uint16_t)rsi);
}
uint64_t sys_getProcesses(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9, uint64_t r10) {
    get_processes();
    return 0;
}
uint64_t sys_fork(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9, uint64_t r10) {
    return fork();
}
uint64_t sys_quitCPU(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9, uint64_t r10) {
    return quitCPU();

}
uint64_t sys_wait(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9, uint64_t r10) {
    return wait();
}

uint64_t sys_get_foreground(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9, uint64_t r10){
    p_info* toRet = get_foreground_process();
    return  toRet->pid;
}
uint64_t sys_sem_open(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9, uint64_t r10) {
    return sem_open(rsi, rdx);
}
uint64_t sys_sem_close(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9, uint64_t r10) {
    return sem_close(rsi);
}
uint64_t sys_sem_wait(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9, uint64_t r10) {
    return sem_wait(rsi);
}
uint64_t sys_sem_post(uint64_t rsi, uint64_t rdx, uint64_t rcx,uint64_t r8, uint64_t r9, uint64_t r10) {
    return sem_post(rsi);
}
uint64_t sys_sem_get_value(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9, uint64_t r10) {
    return sem_get_value(rsi);
}

uint64_t sys_go_middle(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9, uint64_t r10) {
    return goMiddle();
}



uint64_t sys_create_pipe(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9, uint64_t r10) {
    if(create_pipe((int*)rsi, (int*)rdx) == -1){
        return -1;
    }
    return 0;
}

uint64_t sys_print(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9, uint64_t r10){
    char *buffer = (char *) rsi;
    size_t count = (size_t) rdx;
    p_info* current_proc = get_current_process();
    pipe_write(current_proc->stdout,buffer,count);
}
uint64_t sys_readLine(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9, uint64_t r10){
    unsigned int fd = (unsigned int) rsi;
    char *buffer = (char *) rdx;
    size_t count = (size_t) rcx;

    _sti();
    size_t index = 0;

    if(fd == STDIN){
        char letter;
        do {
            while(isBufferEmpty());
            letter = getBuffer();

            if (letter == '\b') { 
                if (index > 0) index--;
                deleteVideo();
            }
            else if (letter == 1 || letter == '\n') { 
                buffer[index++] = '\n';
                nlVideo();
                break;
            } else if (letter == EOF) {  
                if (index == 0) {
                    return 0; 
                } 
            } else if(letter != 0) {
                buffer[index++] = letter;
                imprimirVideo(&letter, 1, BLANCO); 
            }
        } while (index < count - 1);

        buffer[index] = '\0'; 
    }

    return index;  
}


