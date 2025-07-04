#include "memory-lib.h"
#include "test_util.h"
#include "command-lib.h"
#include "test.h"
#include "c-lib.h"
#include <stdlib.h>
#include <stdint.h>
#include <shell.h>
#include <time.h>
#include <c-lib.h>
#include <string-lib.h>
#include <memory-lib.h>
#include <shell-lib.h>

#define MAX_BLOCKS 128

typedef struct MM_rq {
  void* address;
  uint32_t size;
} mm_rq;

uint64_t test_mm(uint64_t argc, char *argv[]) {
  mm_rq mm_rqs[MAX_BLOCKS];
  uint8_t rq;
  uint32_t total;
  uint64_t max_memory;

  if (argc != 1) {
    print("test_mm: ERROR: Invalid number of arguments\n");
    return -1;
  }

  if ((max_memory = satoi(argv[0])) <= 0) {
    print("test_mm: ERROR: Invalid memory size\n");
    return -1;
  }

  while (1) {
    rq = 0;
    total = 0;

    // Request as many blocks as we can
    while (rq < MAX_BLOCKS && total < max_memory) {

      mm_rqs[rq].size = GetUniform(max_memory - total - 1) + 1;
      void * aux = usr_malloc(mm_rqs[rq].size);
      mm_rqs[rq].address = aux;


      if (mm_rqs[rq].address) {
        total += mm_rqs[rq].size;
        rq++;
      }
    }

    // Set
    uint32_t i;
    for (i = 0; i < rq; i++)
      if (mm_rqs[i].address)
        usr_memset(mm_rqs[i].address, i, mm_rqs[i].size);

    // Check
    for (i = 0; i < rq; i++)
      if (mm_rqs[i].address)
        if (!memcheck(mm_rqs[i].address, i, mm_rqs[i].size)) {
          print("test_mm ERROR\n");
          return -1;
        }

    // Free
    for (i = 0; i < rq; i++)
      if (mm_rqs[i].address)
        usr_free(mm_rqs[i].address);

    print("OK");
  }

  printShell();
  return 0;
}