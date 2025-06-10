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
    for (int i = 2; i < MAX_FDS && (fd_r == -1 || fd_w == -1); i++) {
        if (!fd_table[i].in_use) {
            if (fd_w == -1){
                fd_w = i;
            }
            else{
            fd_r = i;
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
    int empty1 = -1, empty2 = -1;
    for (int i = 0; i < MAX_BUFF * 2; i++) {
        if (current_proc->fd_table[i] != -1)
            continue;

        if (empty1 == -1)
            empty1 = i;
        else {
            empty2 = i;
            break;
        }
    }
    if(empty1 == -1 || empty2 == -1){
        return -1;
    }

    current_proc->fd_table[empty1] = fd_r;
    current_proc->fd_table[empty2] = fd_w;
    *fd_read = fd_r;
    *fd_write = fd_w;

    return 0;
}

int pipe_init(Pipe *pipe) {
    if (!pipe) return -1;

    memset(pipe, 0, sizeof(Pipe)); 

    pipe->read_pos = 0;
    pipe->write_pos = 0;
    pipe->read_open = 1;
    pipe->write_open = 1; 
    pipe->initialized = 1;
    pipe->index = 0;
    pipe->size = BUFFER_SIZE;
    pipe->sem_data_available = 1;
    sem_open(pipe->sem_data_available,0);
    pipe->sem_free_space = 2;
    sem_open(pipe->sem_free_space,BUFFER_SIZE);
    return 0;
}

int pipe_write(int fd, const char *buffer, int count) {
    if (fd < 0 || fd >= MAX_FDS || !fd_table[fd].in_use || fd_table[fd].type != FD_WRITE) {
        return -1;
    }
    Pipe *pipe = fd_table[fd].pipe;
    if (pipe->write_open == 0 && can_accsess(fd) != 0) {
        return -1;
    }

    int written = 0;
    for (int i = 0; i < count; i++) {
        sem_wait(pipe->sem_free_space);  
        pipe->buffer[pipe->write_pos] = buffer[i];
        pipe->write_pos = (pipe->write_pos + 1) % BUFFER_SIZE;
        written++;
        pipe->index++;
        sem_post(pipe->sem_data_available);  
    }
    return written;
}

int pipe_read(int fd, char *buffer, int count) {
    if (fd < 0 || fd >= MAX_FDS || !fd_table[fd].in_use || fd_table[fd].type != FD_READ) {
        return -1;
    }
    Pipe *pipe = fd_table[fd].pipe;
    if (pipe->read_open == 0 && can_accsess(fd) != 0) {
        return -1;
    }

    int read = 0;
    for (int i = 0; i < count; i++) {
        sem_wait(pipe->sem_data_available);
        buffer[i] = pipe->buffer[pipe->read_pos];
        pipe->read_pos = (pipe->read_pos + 1) % BUFFER_SIZE;
        read++;
        pipe->index--;
        sem_post(pipe->sem_free_space); 
    }
    return read;
}

int pipe_close(int fd){
    if(fd>MAX_FDS || fd< 0){
        return -1;
    }
    Pipe *pipe = fd_table[fd].pipe;
    p_info * curr_proc = get_current_process();

    for(int i = 0; i < MAX_BUFF; i++){
        if(curr_proc->fd_table[i] == fd){
            curr_proc->fd_table[i] == -1;
            if(fd_table[fd].type ==  FD_READ){   //READ
                pipe->read_open--;
            }
            if(fd_table[fd].type == FD_WRITE ){   //Write
                pipe->write_open--;
            }
            if(pipe->read_open == 0 && pipe->write_open == 0){
                pipe_destroy(fd);
            }
    return 0;
        }
    }
    return -1;
}

void pipe_destroy(int fd){          //LIBERAR LOS PROCESOS LOCKEADOS    [FALTA]
    Pipe *pipe = fd_table[fd].pipe;
    fd_table[fd].in_use = 0;
    fd_table[fd].pipe = NULL;
    fd_table[fd].type = FD_UNUSED;
    mm_free(pipe);
}


int can_accsess(int fd){
    p_info * curr_proc = get_current_process();
    for(int i = 0; i < MAX_BUFF; i++){
        if(curr_proc->fd_table[i] == fd){
            return 0;
        }
    }
    return -1;
}