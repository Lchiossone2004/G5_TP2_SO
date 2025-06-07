#include "../include/sem.h"
#include <stddef.h>
#include "../include/process.h"
extern void lock_acquire(int8_t *lock);
extern void lock_release(int8_t *lock);

static semaphore semaphores[SEM_MAX];

void init_semaphores(void) {
    for (int i = 0; i < SEM_MAX; i++) {
        semaphores[i].value = -1;
        semaphores[i].lock  = 1;
        semaphores[i].head  = 0;
        semaphores[i].tail  = 0;
        semaphores[i].count = 0;
    }
}

int8_t sem_open(int8_t id, int8_t initial_value) {
    if (id <= 0 || id >= SEM_MAX) return -1;
    semaphore *s = &semaphores[id];

    lock_acquire(&s->lock);
    if (s->value != -1) {
        lock_release(&s->lock);
        return id;
    }
    s->value = initial_value;
    s->head  = s->tail = s->count = 0;
    lock_release(&s->lock);
    return id;
}

int8_t sem_close(int8_t id) {
    if (id <= 0 || id >= SEM_MAX) return -1;
    semaphore *s = &semaphores[id];

    lock_acquire(&s->lock);
    if (s->value == -1) {
        lock_release(&s->lock);
        return -1;
    }
    while (s->count > 0) {
        pid_t pid = s->waiting[s->head];
        s->head = (s->head + 1) % MAX_PROCESSES;
        s->count--;
        unblock_process(pid);
    }
    s->value = -1;
    lock_release(&s->lock);
    return 0;
}

int8_t sem_post(int8_t id) {
    if (id <= 0 || id >= SEM_MAX) return -1;
    semaphore *s = &semaphores[id];

    lock_acquire(&s->lock);
    if (s->value == -1) {
        lock_release(&s->lock);
        return -1;
    }
    if (s->count > 0) {
        pid_t pid = s->waiting[s->head];
        s->head = (s->head + 1) % MAX_PROCESSES;
        s->count--;
        unblock_process(pid);
    } else {
        s->value++;
    }
    lock_release(&s->lock);
    return 0;
}

int8_t sem_wait(int8_t id) {
    if (id <= 0 || id >= SEM_MAX) return -1;
    semaphore *s = &semaphores[id];
    pid_t pid = get_pid();

    lock_acquire(&s->lock);
    if (s->value == -1) {
        lock_release(&s->lock);
        return -1;
    }
    if (s->value == 0) {
        if (s->count >= MAX_PROCESSES) {
            lock_release(&s->lock);
            return -1;
        }
        s->waiting[s->tail] = pid;
        s->tail = (s->tail + 1) % MAX_PROCESSES;
        s->count++;
        lock_release(&s->lock);
        block_process(pid);
    } else {
        s->value--;
        lock_release(&s->lock);
    }
    return 0;
}

int8_t sem_get_value(int8_t id) {
    if (id <= 0 || id >= SEM_MAX) return -1;
    semaphore *s = &semaphores[id];
    int8_t v;
    lock_acquire(&s->lock);
    v = s->value;
    lock_release(&s->lock);
    return v;
}
