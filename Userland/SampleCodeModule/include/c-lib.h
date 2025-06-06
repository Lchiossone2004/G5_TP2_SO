#ifndef C_LIB
#define C_LIB

#include <stdlib.h>
#include <stdint.h>
#include <string-lib.h>

//Other
void print(char *buffer);
void read(char *buffer, int fd, int size);
void write(char *word, int fd,int size);
void printRegisters();
void nlPrint();
void printErr(char *buffer);
void sleep(int ticks);

//Proc
int32_t usr_create_process(void* fn, uint64_t argc, char *argv[], char * name, int prio, int is_foreground);
int usr_block_process(int pid);
int usr_unblock_process(int pid);
int usr_kill(int pid);
void usr_yield();
void usr_nice(int pid, int newPrio);

//Sems
int usr_sem_open(int id, int initial_value);
int usr_sem_close(int id);
int usr_sem_wait(int id);
int usr_sem_post(int id);
int usr_sem_getvalue(int id);

//Pipes

int usr_open_pipe(int *fd_read, int* fd_write);


#endif