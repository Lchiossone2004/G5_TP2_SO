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
int create_pipe(const char* pipe_id) {
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
            return i;
        }
    }
    return -1;
}

int open_pipe(const char* pipe_id, int* pipefd) {
    for (int i = 0; i < MAX_PIPES; i++) {
        if (pipes[i] && strcmp(pipes[i]->id, pipe_id) == 0) {
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

size_t write_to_pipe(PipeBuffer* pipe, const char* buffer, size_t count) {
    size_t i;
    for (i = 0; i < count; ++i) {
        sem_wait(pipe->write_sem);

        if (pipe->write_pos >= pipe->buffer_size) {
            return PIPE_FULL;
        }

        pipe->write_buffer[pipe->write_pos++] = buffer[i];
        sem_post(pipe->read_sem);
    }
    return i;
}

size_t read_from_pipe(PipeBuffer* pipe, char* buffer, size_t count) {
    size_t i;
    for (i = 0; i < count; ++i) {
        sem_wait(pipe->read_sem);

        if (pipe->read_pos >= pipe->write_pos) {
            return PIPE_EMPTY;
        }

        buffer[i] = pipe->read_buffer[pipe->read_pos++];
        sem_post(pipe->write_sem);
    }
    return i;
}
