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
    uint64_t now = ticks_elapsed();
    sleep_entry *prev = NULL;
    sleep_entry *cur  = sleep_list;

    while (cur) {
        sleep_entry *next = cur->next;

        if (foundprocess(cur->proc->pid) == -1) {
            if (prev) prev->next = next;
            else      sleep_list = next;
            mm_free(cur);
        }
        else if (cur->wake_tick <= now) {
            if (prev) prev->next = next;
            else      sleep_list = next;

            unblock_process(cur->proc->pid);  
            mm_free(cur);
        }
        else {
            prev = cur;
        }

        cur = next;
    }
}