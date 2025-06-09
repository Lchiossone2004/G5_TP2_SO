#include <c-lib.h>
#include <stdbool.h>
#define STDIN 0
#define STDOUT 1
#define STDERR 2
#define SEM_MAX 10

extern uint64_t syscall(uint64_t rdi, ...);
static bool is_semaphore[SEM_MAX] = { false };
//Other

void printRegisters(){
    syscall(1, STDOUT);
}

void print(char *word){
    syscall(4, STDOUT, word, strSize(word));
    //syscall(41, word, strSize(word));
}

void write(char *word, int fd, int size){
    syscall(4, fd, word, strSize(word));
}

int read(char *buffer,int fd ,int size){
    int ret = (int) syscall(3, fd, buffer, size);
    return ret;
}

void printErr(char *word){
    syscall(4, STDERR, word, strSize(word));
}

void sleep(int ticks){
    syscall(7, ticks);
    return;
}
char get_char() {
    char letter;
    syscall(2, STDIN, &letter, 1, 0, 0, 0);
    return letter;
}
int readLine(char *buffer, size_t size){
    return syscall(42, STDIN, buffer, size, 0, 0, 0);
}

//Proc

int32_t usr_create_process(void* fn, uint64_t argc, char *argv[], char * name, int prio, int is_foreground){
    int32_t pid = (int32_t)syscall(14,fn,argc,argv, name, prio, is_foreground);
    return pid;
}

int usr_block_process(int pid){
    return syscall(19,pid);
}

int usr_unblock_process(int pid){
    return syscall(20,pid);

}

int usr_kill(int pid){
    int toRet = -1;
    if(pid >0){
        toRet = syscall(15, pid);
    }
    return toRet;

}

void usr_yield(){
    syscall(22);
}

void usr_nice(int pid, int newPrio){
    if(pid > 0 && newPrio > 0){
        syscall(26,pid,newPrio); //FALTA
    }
}
//Sems

int usr_sem_open(int id, int initial_value) {
    int ret = syscall(25, id, initial_value);
    if (ret >= 0 && id >= 0 && id < SEM_MAX) {
        is_semaphore[id] = true;
    }
    return ret;
}

int usr_sem_close(int id) {
    int ret = syscall(26, id);
    if (ret >= 0 && id >= 0 && id < SEM_MAX) {
        is_semaphore[id] = false;
    }
    return ret;
}

int usr_sem_wait(int id) {
    if (id >= 0 && id < SEM_MAX && is_semaphore[id]) {
        return syscall(36, id);     //QUE?    
    } else {
        return syscall(24, id);      //QUE?
    }
}
int usr_sem_post(int id) {
    if (id >= 0 && id < SEM_MAX && is_semaphore[id]) {
        return syscall(28, id);        
    }
    return -1;                      
}

int usr_sem_getvalue(int id){
    return syscall(29, id);
}

//Pipes

int usr_open_pipe(int *fd_read, int* fd_write){
    return syscall(31,fd_read, fd_write); 
}