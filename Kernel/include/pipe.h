#ifndef PIPE_H
#define PIPE_H

#include <stdint.h>
#include <stddef.h>

#define MAX_PIPES 10
#define PIPE_EMPTY -1
#define PIPE_FULL -2
#define BUFFER_SIZE 2048    //Tal vez medio grande
#define MAX_FDS 2 * MAX_PIPES

typedef enum { FD_UNUSED, FD_READ, FD_WRITE } FDType;

typedef struct {    
    char buffer[BUFFER_SIZE]; 
    size_t read_pos;
    size_t write_pos;
    int initialized;
    int index;
    int is_read_open;   
    int is_write_open;
    int size;
    //Podria faltar algo de bloqueo  
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

#endif  // PIPE_H
