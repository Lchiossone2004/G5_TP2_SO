#ifndef C_LIB
#define C_LIB

#include <stdlib.h>
#include <stdint.h>
#include <string-lib.h>

//Other
void print(char *buffer);
void read(char *buffer, int size);
void printRegisters();
void nlPrint();
void printErr(char *buffer);
void sleep(int ticks);

//Proc
void usr_create_process(void* fn, uint64_t argc, char *argv[], char * name, int prio, int is_foreground);
void usr_block_process(int pid);
void usr_unblock_process(int pid);
int usr_kill(int pid);
void usr_yeild();
void usr_nice(int pid, int newPrio);

//Sems
int usr_sem_open(int id, int initial_value);
int usr_sem_close(int id);
int usr_sem_wait(int id);
int usr_sem_post(int id);
int usr_sem_getvalue(int id);


#endif