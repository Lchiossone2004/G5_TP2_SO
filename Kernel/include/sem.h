#ifndef SEM_H
#define SEM_H

#include <stdint.h>
#include "defs.h"
#include "scheduler.h"
#define SEM_MAX 32

#define pid_t int16_t
typedef struct semaphore {
    int8_t value;
    int8_t lock;
    pid_t waiting[MAX_PROCESSES];
    int head, tail, count;
} semaphore;

void init_semaphores(void);
int8_t sem_open(int8_t id, int8_t initial_value);
int8_t sem_close(int8_t id);
int8_t sem_wait(int8_t id);
int8_t sem_post(int8_t id);
int8_t sem_get_value(int8_t id);

#endif