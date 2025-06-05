#ifndef PIPE_H
#define PIPE_H

#include <stdint.h>
#include <stddef.h>

#define MAX_PIPES 10
#define PIPE_EMPTY -1
#define PIPE_FULL -2

typedef struct {
    char *read_buffer;
    char *write_buffer;
    size_t read_pos;
    size_t write_pos;
    size_t buffer_size;
    int in_use;  
    int read_sem;  
    int write_sem; 
} PipeBuffer;

typedef struct {
    char *id;
    PipeBuffer *internal_pipe;
} Pipe;

extern Pipe* pipes[MAX_PIPES];

int create_pipe(const char* pipe_id);
int open_pipe(const char* pipe_id, int* pipefd);
size_t read_from_pipe(PipeBuffer* pipe, char* buffer, size_t count);
size_t write_to_pipe(PipeBuffer* pipe, const char* buffer, size_t count);

#endif  // PIPE_H
