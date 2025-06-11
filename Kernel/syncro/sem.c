#include "../include/sem.h"
#include <stddef.h>
#include "../include/process.h"

extern void lock_acquire(uint8_t *lock);
extern void lock_release(uint8_t *lock);

static semaphore semaphores[SEM_MAX];

void init_semaphores(void) {
    for (int i = 0; i < SEM_MAX; i++) {
        semaphores[i].value = -1;
        semaphores[i].lock  = 1;
        semaphores[i].head  = 0;
        semaphores[i].tail  = 0;
        semaphores[i].count = 0;
        initialize_zero(semaphores[i].waiting, sizeof(semaphores[i].waiting) / sizeof(semaphores[i].waiting[0]));
    }
}

int16_t sem_open(int16_t id, int16_t initial_value) {
    if (id < 0 || id >= SEM_MAX) return -1;
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

int16_t sem_close(int16_t id) {
    if (id < 0 || id >= SEM_MAX) return -1;
    semaphore *s = &semaphores[id];
    pid_t to_wake[MAX_PROCESSES];
    int n = 0;

    lock_acquire(&s->lock);
    if (s->value == -1) {
        lock_release(&s->lock);
        return -1;
    }
    while (s->count > 0) {
        to_wake[n++] = s->waiting[s->head];
        s->head = (s->head + 1) % MAX_PROCESSES;
        s->count--;
    }
    s->value = -1;
    lock_release(&s->lock);

    for (int i = 0; i < n; i++) {
        if(foundprocess(i) !=-1){
            unblock_process(to_wake[i]);
        }
    }
    return 0;
}

int16_t sem_post(int16_t id) {
    if (id < 0 || id >= SEM_MAX) return -1;
    semaphore *s = &semaphores[id];
    pid_t pid;

    lock_acquire(&s->lock);
    if (s->value == -1) {
        lock_release(&s->lock);
        return -1;
    }
    if (s->count > 0) {

        for(int i = 0; i<s->count; i++){
            pid = s->waiting[s->head - i];
            if(get_process_by_pid(pid) != NULL){
                s->head = (s->head + 1) % MAX_PROCESSES;
                s->count--;
                lock_release(&s->lock);
                unblock_process(pid);
            }
        }        
    } else {
        s->value++;
        lock_release(&s->lock);
    }
    return 0;
}

int16_t sem_wait(int16_t id) {
    if (id < 0 || id >= SEM_MAX) return -1;
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
        callScheduler();
    } else {
        s->value--;
        lock_release(&s->lock);
    }
    return 0;
}

int16_t sem_get_value(int16_t id) {
    if (id < 0 || id >= SEM_MAX) return -1;
    semaphore *s = &semaphores[id];
    int16_t v;
    lock_acquire(&s->lock);
    v = s->value;
    lock_release(&s->lock);
    return v;
}
