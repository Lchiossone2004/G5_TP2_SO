#ifndef SEM_H
#define SEM_H

#include <stdint.h>
#include "defs.h"
#include "scheduler.h"
#define SEM_MAX 32
#define BUFF 2048

#define pid_t int16_t
typedef struct semaphore {
    int value;
    int8_t lock;
    pid_t waiting[MAX_PROCESSES];
    int head, tail, count, initialized;
} semaphore;

void init_semaphores(void);
int16_t sem_open(int16_t initial_value) ;
int16_t sem_close(int16_t id);
int16_t sem_wait(int16_t id);
int16_t sem_post(int16_t id);
int16_t sem_get_value(int16_t id);

#endif