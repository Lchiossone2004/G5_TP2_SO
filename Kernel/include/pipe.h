#include<stdint.h>
#include <stdlib.h>
#define PIPE_FULL -1
#define PIPE_EMPTY -2


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
