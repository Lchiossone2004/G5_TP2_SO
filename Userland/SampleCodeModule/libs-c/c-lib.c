#include <c-lib.h>

#define STDIN 0
#define STDOUT 1
#define STDERR 2

extern uint64_t syscall(uint64_t rdi, ...);

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

void read(char *buffer,int fd ,int size){
    syscall(3, fd, buffer, size);
    return;
}

void printErr(char *word){
    syscall(4, STDERR, word, strSize(word));
}

void nlPrint(){
    syscall(5, STDOUT);
}

void sleep(int ticks){
    syscall(8, ticks);
    return;
}

//Proc

int32_t usr_create_process(void* fn, uint64_t argc, char *argv[], char * name, int prio, int is_foreground){
        int32_t pid = (int32_t)syscall(22,fn,argc,argv, name, prio, is_foreground);
        return pid;
}

int usr_block_process(int pid){
    return syscall(27,pid);
}

int usr_unblock_process(int pid){
    return syscall(28,pid);

}

int usr_kill(int pid){
    int toRet = -1;
    if(pid >0){
        toRet = syscall(23, pid);
    }
    return toRet;

}

void usr_yield(){
    syscall(31);
}

void usr_nice(int pid, int newPrio){
    if(pid > 0 && newPrio > 0){
        syscall(26,pid,newPrio);
    }
}

//Sems

int usr_sem_open(int id, int initial_value) {
    return syscall(34, id, initial_value);
}

int usr_sem_close(int id){
    return syscall(35, id);
}

int usr_sem_wait(int id){
    return syscall(36, id);
}

int usr_sem_post(int id){
    return syscall(37, id);
}

int usr_sem_getvalue(int id){
    return syscall(38, id);
}

//Pipes

int usr_open_pipe(int *fd_read, int* fd_write){
    return syscall(40,fd_read, fd_write); 
}