#include <stdint.h>
#include <stdlib.h>
#include "../include/pipe.h"
#include "../memory/memory_manager.h"
#include "../include/sem.h" 

#define PIPE_FULL -1
#define PIPE_EMPTY 0


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
            return PIPE_FULL;  //aca se supone q no se lega nunca
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
            return PIPE_EMPTY; //tmb se supone q no se llega nunca aca
        }

        buffer[i] = pipe->read_buffer[pipe->read_pos++];
        sem_post(pipe->write_sem);
    }
    return i;
}

