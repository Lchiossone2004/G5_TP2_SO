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
#include "../include/sleep.h"
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
#define EOF '\0'

typedef uint64_t (*syscall_fn)(uint64_t, uint64_t, uint64_t, uint64_t, uint64_t, uint64_t);

extern void _sti();

static int seed = 0;

// Array de punteros a funciones, indexado por syscall ID
static syscall_fn syscall_table[] = {
    [1] = sys_registers_print,
    [2] = sys_getChar,
    [3] = sys_read,
    [4] = sys_write,
    [5] = sys_zoom,
    [6] = sys_clear,
    [7] = sys_sleep,
    [8] = sys_getTime,
    [9] = sys_ranN, 
    [10] = sys_malloc,
    [11] = sys_free,
    [12] = sys_get_memory_info,
    [13] = sys_create,
    [14] = sys_kill,
    [15] = sys_getPid,
    [16] = sys_endProcess,
    [17] = sys_modifyPriority,
    [18] = sys_block,
    [19] = sys_unblock,
    [20] = sys_getProcesses,
    [21] = sys_quitCPU,
    [22] = sys_wait,
    [23] = sys_get_foreground,
    [24] = sys_sem_open,
    [25] = sys_sem_close,
    [26] = sys_sem_wait,
    [27] = sys_sem_post,
    [28] = sys_sem_get_value,
    [29] = sys_create_pipe,
    [30] = sys_pipe_close,
    [31] = sys_dup,
    [32] = sys_memset,
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
    char *letter = (char *) rdx;
    _sti();    
    pipe_read(STDOUT,letter,1);

    return 0;
}

uint64_t sys_read(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9, uint64_t r10) {
    unsigned int fd = (unsigned int) rsi;
    char *buffer = (char *) rdx;
    size_t count = (size_t) rcx;
    if(fd == STDIN){
        p_info * proc = get_current_process();
        if(proc->stdin == STDIN && proc->is_foreground){
            return read_keyboard(buffer,count);
        }
        else{
            return (uint64_t) pipe_read(proc->stdin,buffer,count);
        }
    }
    else{
        return (uint64_t) pipe_read(fd,buffer,count);
    }
}


uint64_t sys_write(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9, uint64_t r10) {
    unsigned int fd = (unsigned int) rsi;
    char *buffer = (char *) rdx;
    size_t count = (size_t) rcx;

    if(fd == STDOUT){
        p_info * proc = get_current_process();
        if(proc->stdout == STDOUT && proc->is_foreground){ 
            imprimirVideo(buffer, count, BLANCO);
        }
        else{
            pipe_write(proc->stdout,buffer,count);
        }
    }
    else if(fd == STDERR){
        p_info * proc = get_current_process();
        if(proc->stdout == STDOUT && proc->is_foreground){ 
            imprimirVideo(buffer, count, ROJO);
        }
        else{
            pipe_write(proc->stdout,buffer,count);
        }
    } 
    else if (fd >= PIPE_FD_START) {
        pipe_write(fd,buffer,count);
    }

    return 0;
}

uint64_t sys_zoom(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9, uint64_t r10){ 
    unsigned int fd = (unsigned int) rsi;
    if(fd == STDOUT) {
        zoomIN();
        rePrint();
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

uint64_t sys_sleep(uint64_t rsi,uint64_t rdx, uint64_t rcx,uint64_t r8,uint64_t r9,uint64_t current_sp){
    p_info *proc = get_current_process();
    if (!proc)
        return (uint64_t)-1;

    uint64_t deadline = ticks_elapsed() + rsi;
    sleep_queue_add(proc, deadline);
    block_process(proc->pid);
    callScheduler();
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

uint64_t sys_malloc(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9, uint64_t r10){
    size_t size = (size_t) rsi;
    if (size == 0) return NULL;
    void* ptr = mm_malloc(size);
    return (uint64_t) ptr;
}

uint64_t sys_free(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9, uint64_t r10){
    void *ptr = (void *) rsi;
    if (ptr == NULL) return 0;
    size_t freed_size = mm_free(ptr);
    return 0;
}

uint64_t sys_get_memory_info(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9, uint64_t r10){
    memory_info_t *info = (memory_info_t *) rsi;
    if (info == NULL) return 0;

    mm_get_info(info);
    info->total_memory = info->total_memory;
    info->free_memory = info->free_memory;
    info->used_memory = info->used_memory;
    info->current_blocks=info->current_blocks;
    info->free_block_count= info->free_block_count;
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

uint64_t sys_quitCPU(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9, uint64_t r10) {
    return quitCPU();

}
uint64_t sys_wait(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9, uint64_t r10) {
    return wait();
}

uint64_t sys_get_foreground(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9, uint64_t r10) {
    p_info* toRet = get_foreground_process();
    return  toRet->pid;
}
uint64_t sys_sem_open(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9, uint64_t r10) {

    return sem_open(rsi,rdx);

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

uint64_t sys_pipe_close(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9, uint64_t r10){
    int fd = (int) rsi;
    return pipe_close(fd);
}

uint64_t sys_dup(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9, uint64_t r10){
    int pid = (int) rsi;
    int std = (int) rdx;
    int newFd = (int) rcx;
    p_info * proc = get_process_by_pid(pid);
    if(std == STDIN){
        proc->stdin = newFd;
        return pipe_accses(pid,newFd);
    }
    else if(std == STDOUT){
        proc->stdout = newFd;
        return pipe_accses(pid,newFd);
    }
    else{
        return -1;
    }
}
uint64_t sys_memset(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9, uint64_t r10) {
    return memset((void *) rsi, (int32_t) rdx,rcx);
}