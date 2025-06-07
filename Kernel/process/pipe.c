#include "../include/pipe.h"
#include "../memory/memory_manager.h"
#include "../include/sem.h"
#include "../include/process.h"
#include "../include/lib.h"

#define MAX_PIPES 10

Pipe* pipes[MAX_PIPES] = { NULL };

FDEntry fd_table[MAX_FDS] = { NULL};

int create_pipe(int* fd_read, int*fd_write) {
    int pipe_index = -1;

    for (int i = 0; i < MAX_PIPES; i++) {
        if (pipes[i] == NULL) {
            pipes[i] = mm_malloc(sizeof(Pipe));
            if (!pipes[i]) return -1;
            if (pipe_init(pipes[i]) != 0) {
                mm_free(pipes[i]);
                pipes[i] = NULL;
                return -1;
            }
            pipe_index = i;
            break;
        }
    }
    if (pipe_index == -1) return -1;

    int fd_r = -1, fd_w = -1;
    for (int i = 3; i < MAX_FDS && (fd_r == -1 || fd_w == -1); i++) {
        if (!fd_table[i].in_use) {
            if (fd_r == -1){
                fd_r = i;
            }
            else{
            fd_w = i;
            }
        }
    }
    if (fd_r == -1 || fd_w == -1) {
        mm_free(pipes[pipe_index]);
        pipes[pipe_index] = NULL;
        return -1;
    }

    fd_table[fd_r].pipe = pipes[pipe_index];
    fd_table[fd_r].type = FD_READ;
    fd_table[fd_r].in_use = 1;

    fd_table[fd_w].pipe = pipes[pipe_index];
    fd_table[fd_w].type = FD_WRITE;
    fd_table[fd_w].in_use = 1;

    p_info* current_proc = get_current_process();
    int empty1,empty2 = -1;
    for(int i = 0; i < MAX_BUFF; i++){
        if(current_proc->buffers[i] == 0 && empty1 == -1){
            empty1 = i;
        }
        else if(current_proc->buffers[i] == 0 && empty1 != -1){
            empty2 = i;
            break;
        }
    }
    if(empty1 == -1 || empty2 == -1){
        return -1;
    }

    current_proc->buffers[empty1] = fd_r;
    current_proc->buffers[empty2] = fd_w;
    *fd_read = fd_r;
    *fd_write = fd_w;

    return 0;
}

int pipe_init(Pipe *pipe) {
    if (!pipe) return -1;

    memset(pipe, 0, sizeof(Pipe)); 

    pipe->read_pos = 0;
    pipe->write_pos = 0;
    pipe->is_read_open = 1;
    pipe->is_write_open = 1; 
    pipe->initialized = 1;
    pipe->size = MAX_BUFF;
    return 0;
}

int pipe_write(int fd, const char *buffer, int count) {                                         //FALTA VERIFICAION PARA QUE SE USE DE A 1
    if (fd < 0 || fd >= MAX_FDS || !fd_table[fd].in_use || fd_table[fd].type != FD_WRITE) {
        return -1;
    }
    Pipe *pipe = fd_table[fd].pipe;
    if(!pipe->is_write_open){
        return -1;
    }
    int written = 0;

    for (int i = 0; i < count; i++) {
        size_t next_pos = (pipe->write_pos + 1) % BUFFER_SIZE;
        if (next_pos == pipe->read_pos) {
            break;
        }
        pipe->buffer[pipe->write_pos] = buffer[i];
        pipe->write_pos = next_pos;
        written++;
    }
    return written;
}

int pipe_read(int fd, char *buffer, int count) {
    if (fd < 0 || fd >= MAX_FDS || !fd_table[fd].in_use || fd_table[fd].type != FD_READ) {
        return -1;
    }
    Pipe *pipe = fd_table[fd].pipe;
    if(!pipe->is_read_open){
        return -1;
    }
    int read = 0;

    while (read < count && pipe->read_pos != pipe->write_pos) {
        buffer[read++] = pipe->buffer[pipe->read_pos];
        pipe->read_pos = (pipe->read_pos + 1) % BUFFER_SIZE;
    }
    return read;
}

void pipe_close(int end, int fd){
        if (fd < 0 || fd >= MAX_FDS || !fd_table[fd].in_use || fd_table[fd].type != FD_READ) {
        return -1;
    }
    Pipe *pipe = fd_table[fd].pipe;
    if(end == 0 ){   //READ
        pipe->is_read_open = 0;
    }
    if(end == 1 ){   //Write
        pipe->is_write_open = 1;
    }
    return 0;
}