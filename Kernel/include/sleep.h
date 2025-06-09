#ifndef SLEEP_QUEUE_H
#define SLEEP_QUEUE_H

#include <stdint.h>
#include "process.h"

void sleep_queue_init(void);
void sleep_queue_add(p_info *proc, uint64_t wake_tick);
void sleep_queue_tick_handler(void);

#endif