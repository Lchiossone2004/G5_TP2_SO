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
    if (!e) {
        return;
    }
    e->proc = proc;
    e->wake_tick = wake_tick;
    sleep_entry **it = &sleep_list;
    while (*it && (*it)->wake_tick <= wake_tick) {
        it = &(*it)->next;
    }
    e->next = *it;
    *it = e;
}

void sleep_queue_tick_handler(void) {
    uint64_t now;
    now = ticks_elapsed();
    while (sleep_list && sleep_list->wake_tick <= now) {
        sleep_entry *wake = sleep_list;
        sleep_list = wake->next;
        unblock_process(wake->proc->pid);
        add_to_ready_list(wake->proc);
        mm_free(wake);
    }
}