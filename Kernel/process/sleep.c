#include "../include/sleep.h"
#include "../include/lib.h"
#include "../include/scheduler.h"
#include "../include/process.h"
#include "../include/time.h"
#include "../memory/memory_manager.h"

typedef struct sleep_entry {
    p_info *proc;
    uint64_t wake_tick;
    struct sleep_entry *next;
} sleep_entry;

static sleep_entry *sleep_list = NULL;

void sleep_queue_init(void) {
    sleep_list = NULL;
}

void sleep_queue_add(p_info *proc, uint64_t wake_tick) {
    sleep_entry *e = mm_malloc(sizeof(*e));
    e->proc      = proc;
    e->wake_tick = wake_tick;
    e->next      = sleep_list;
    sleep_list   = e;
}

void sleep_queue_tick_handler(void) {
    uint64_t now = ticks_elapsed();
    sleep_entry **it = &sleep_list;
    while (*it) {
        if ((*it)->wake_tick <= now) {
            sleep_entry *wake = *it;
            *it = wake->next;
            unblock_process(wake->proc->pid);
            add_to_ready_list(wake->proc);
            mm_free(wake);
        } else {
            it = &(*it)->next;
        }
    }
}