#ifndef PIPE_H
#define PIPE_H

#include <stdint.h>
#include <stddef.h>

#define MAX_PIPES 10
#define PIPE_EMPTY -1
#define PIPE_FULL -2
#define BUFFER_SIZE 2048
#define MAX_FDS 2 * MAX_PIPES

typedef enum { FD_UNUSED, FD_READ, FD_WRITE } FDType;

typedef struct {    
    char buffer[BUFFER_SIZE]; 
    size_t read_pos;
    size_t write_pos;
    int initialized;
    int read_open;   
    int write_open;
    int size;
    int16_t sem_free_space;
    int16_t sem_data_available;
    int id;
} Pipe;

typedef struct {
    Pipe* pipe;
    FDType type;
    int in_use;
} FDEntry;

int create_pipe(int* fd_read, int*fd_write);
int pipe_init(Pipe *pipe);
int pipe_write(int fd, const char *buffer, int count);
int pipe_read(int fd, char *buffer, int count);
int pipe_accses(int pid,int fd);
int pipe_close(int fd);
void pipe_destroy(int fd);
int can_accsess(int fd);

#endif 