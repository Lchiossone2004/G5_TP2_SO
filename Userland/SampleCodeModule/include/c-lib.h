#ifndef C_LIB
#define C_LIB

void print(char *buffer);
void read(char *buffer, int size);
void printRegisters();
void nlPrint();
void printErr(char *buffer);
void sleep(int ticks);
int sem_open_lib(int id, int initial_value);
int sem_close_lib(int id);
int sem_wait_lib(int id);
int sem_post_lib(int id);
int sem_getvalue_lib(int id);
#endif