#include <stdint.h>
#include <stdio.h>
#include "../include/test_util.h"
#include "test.h"


#define MINOR_WAIT 1000000 // TODO: Change this value to prevent a process from flooding the screen
#define WAIT 10000000      // TODO: Change this value to make the wait long enough to see theese processes beeing run at least twice

#define TOTAL_PROCESSES 3
#define LOWEST 0  // TODO: Change as required
#define MEDIUM 1  // TODO: Change as required
#define HIGHEST 2 // TODO: Change as required

int64_t prio[TOTAL_PROCESSES] = {LOWEST, MEDIUM, HIGHEST};

void test_prio() {
  int64_t pids[TOTAL_PROCESSES];
  char *argv[] = {0};
  uint64_t i;

  for (i = 0; i < TOTAL_PROCESSES; i++)
    pids[i] = usr_create_process(endless_loop,0,argv,"endless_loop_print", 3,0);

  bussy_wait(WAIT);
  print("\nCHANGING PRIORITIES...\n");

  for (i = 0; i < TOTAL_PROCESSES; i++)
    usr_nice(pids[i], prio[i]);

  bussy_wait(WAIT);
  print("\nBLOCKING...\n");

  for (i = 0; i < TOTAL_PROCESSES; i++)
    usr_block_process(pids[i]);

  print("CHANGING PRIORITIES WHILE BLOCKED...\n");

  for (i = 0; i < TOTAL_PROCESSES; i++)
    usr_nice(pids[i], MEDIUM);

  print("UNBLOCKING...\n");

  for (i = 0; i < TOTAL_PROCESSES; i++)
    usr_unblock_process(pids[i]);

  bussy_wait(WAIT);
  print("\nKILLING...\n");

  for (i = 0; i < TOTAL_PROCESSES; i++)
    usr_kill(pids[i]);
}