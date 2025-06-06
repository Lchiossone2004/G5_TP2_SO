#include "../include/pipe.h"
#include "../memory/memory_manager.h"
#include "../include/sem.h"

#define MAX_PIPES 10

Pipe* pipes[MAX_PIPES] = {0};

char* strdup(const char* str) {
    if (str == NULL) {
        return NULL;
    }

    size_t len = strlen(str) + 1;

    char* copy = (char*)mm_malloc(len);
    if (!copy) {
        return NULL;
    }
    memcpy(copy, str, len);

    return copy;
}

int create_pipe(const char* pipe_id, uint16_t writer_pid) {
    for (int i = 0; i < MAX_PIPES; i++) {
        if (pipes[i] == NULL) {
            pipes[i] = (Pipe*)mm_malloc(sizeof(Pipe));
            if (!pipes[i]) {
                return -1;
            }

            pipes[i]->id = strdup(pipe_id);
            pipes[i]->internal_pipe = (PipeBuffer*)mm_malloc(sizeof(PipeBuffer));
            if (!pipes[i]->internal_pipe) {
                mm_free(pipes[i]);
                return -1;
            }

            init_pipe(pipes[i]->internal_pipe, 1024);

            pipes[i]->writer_pid = writer_pid;
            pipes[i]->reader_pid = -1;  

            return i;
        }
    }
    return -1;
}

int open_pipe(const char* pipe_id, int* pipefd, uint16_t reader_pid) {
    for (int i = 0; i < MAX_PIPES; i++) {
        if (pipes[i] && strcmp(pipes[i]->id, pipe_id) == 0) {
            pipes[i]->reader_pid = reader_pid;

            pipefd[0] = i; 
            pipefd[1] = i; 
            return 0;
        }
    }
    return -1;
}

int init_pipe(PipeBuffer* pipe, size_t buffer_size) {
    pipe->read_buffer = (char*)mm_malloc(buffer_size);
    pipe->write_buffer = (char*)mm_malloc(buffer_size);
    if (!pipe->read_buffer || !pipe->write_buffer) {
        return -1;
    }
    pipe->read_pos = 0;
    pipe->write_pos = 0;
    pipe->buffer_size = buffer_size;
    pipe->in_use = 1;

    
    pipe->read_sem = 0; 
    pipe->write_sem = buffer_size;  

    return 0;
}

size_t write_to_pipe(Pipe* pipe, const char* buffer, size_t count, uint16_t writer_pid) {
    size_t i;
    if (pipe->writer_pid != writer_pid) {
        return 0; 
    }

    for (i = 0; i < count; ++i) {
        sem_wait(pipe->internal_pipe->write_sem);

        if (pipe->internal_pipe->write_pos >= pipe->internal_pipe->buffer_size) {
            return PIPE_FULL;
        }

        pipe->internal_pipe->write_buffer[pipe->internal_pipe->write_pos++] = buffer[i];
        sem_post(pipe->internal_pipe->read_sem);
    }
    return i;
}

size_t read_from_pipe(Pipe* pipe, char* buffer, size_t count, uint16_t reader_pid) {
    size_t i;
    if (pipe->reader_pid != reader_pid) {
        return 0;  
    }

    for (i = 0; i < count; ++i) {
        sem_wait(pipe->internal_pipe->read_sem);

        if (pipe->internal_pipe->read_pos >= pipe->internal_pipe->write_pos) {
            return PIPE_EMPTY;
        }

        buffer[i] = pipe->internal_pipe->read_buffer[pipe->internal_pipe->read_pos++];
        sem_post(pipe->internal_pipe->write_sem);
    }
    return i;
}
