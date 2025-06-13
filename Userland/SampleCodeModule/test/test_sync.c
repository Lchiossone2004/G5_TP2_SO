#include <stdint.h>
#include <stdio.h>
#include "../include/test_util.h"
#include "../include/test.h"
#include "command-lib.h"

#define SEM_ID 1
#define TOTAL_PAIR_PROCESSES 2

int64_t global; // shared memory

void slowInc(int64_t *p, int64_t inc) {
  uint64_t aux = *p;
  usr_yield(); // This makes the race condition highly probable
  aux += inc;
  *p = aux;
}

uint64_t my_process_inc(uint64_t argc, char *argv[]) {
  uint64_t n;
  int8_t inc;
  int8_t use_sem;

  if (argc != 3)
    return -1;

  if ((n = satoi(argv[0])) <= 0)
    return -1;
  if ((inc = satoi(argv[1])) == 0)
    return -1;
  if ((use_sem = satoi(argv[2])) < 0)
    return -1;

  if (use_sem)
    if (!usr_sem_open(SEM_ID, 1)) {
      print("test_sync: ERROR opening semaphore\n");
      return -1;
    }

  uint64_t i;
  for (i = 0; i < n; i++) {
    if (use_sem)
      usr_sem_wait(SEM_ID);
    slowInc(&global, inc);
    if (use_sem)
      usr_sem_post(SEM_ID);
  }

  if (use_sem)
    usr_sem_close(SEM_ID);

  return 0;
}

 uint64_t test_sync(uint64_t argc, char *argv[]) {

  uint64_t pids[2 * TOTAL_PAIR_PROCESSES];

  if (argc != 2) 
    return -1;

  char *argvDec[] = {argv[0], "-1", argv[1], NULL};
  char *argvInc[] = {argv[0], "1", argv[1], NULL};

  global = 0;

  uint64_t i;
  for (i = 0; i < TOTAL_PAIR_PROCESSES; i++) {
     pids[i] = usr_create_process((void*)my_process_inc, 4, argvDec,"my_process_inc",2,0);
     pids[i + TOTAL_PAIR_PROCESSES] = usr_create_process((void*)my_process_inc, 4, argvInc, "my_process_inc",2,0);
  }

  for (i = 0; i < TOTAL_PAIR_PROCESSES; i++) {
    usr_sem_wait(pids[i]);
    usr_sem_wait(pids[i + TOTAL_PAIR_PROCESSES]);
  }

  print("Final value:");
  char toPrint[4];
  intToString(global, toPrint, 4);
  print(toPrint);
  printShell();
  return 0;
}