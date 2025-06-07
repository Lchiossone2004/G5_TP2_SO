#include "process-lib.h"
#include "test_util.h"
#include "test.h"
#include "c-lib.h"
#include <stdlib.h>
#include <stdint.h>
#include <shell.h>
#include <time.h>
#include <c-lib.h>
#include <string-lib.h>
#include <process-lib.h>
#include <shell-lib.h>

#define MINOR_WAIT 1000000 // TODO: Change this value to prevent a process from flooding the screen
#define WAIT 10000000      // TODO: Change this value to make the wait long enough to see theese processes beeing run at least twice

#define TOTAL_PROCESSES 3
#define CRITIC 8  // TODO: Change as required
#define HIGH 4  // TODO: Change as required
#define NORMAL 2 // TODO: Change as required
#define LOW 1

int64_t prio[TOTAL_PROCESSES + 1] = {CRITIC, HIGH, NORMAL, LOW};

void test_prio() {
  int64_t pids[TOTAL_PROCESSES];
  char *argv[] = {0};
  uint64_t i;

  for (i = 0; i < TOTAL_PROCESSES; i++)
    pids[i] = my_create_process("endless_loop_print", 0, argv, endless_loop_print, 0, 1);

  bussy_wait(WAIT);
  print("\nCHANGING PRIORITIES...\n");

  for (i = 0; i < TOTAL_PROCESSES; i++)
    my_nice(pids[i], prio[i]);

  bussy_wait(WAIT);
  print("\nBLOCKING...\n");

  for (i = 0; i < TOTAL_PROCESSES; i++)
    my_block(pids[i]);

  print("CHANGING PRIORITIES WHILE BLOCKED...\n");

  for (i = 0; i < TOTAL_PROCESSES; i++)
    my_nice(pids[i], NORMAL);

  print("UNBLOCKING...\n");

  for (i = 0; i < TOTAL_PROCESSES; i++)
    my_unblock(pids[i]);

  bussy_wait(WAIT);
  print("\nKILLING...\n");

  for (i = 0; i < TOTAL_PROCESSES; i++)
    my_kill(pids[i]);
}