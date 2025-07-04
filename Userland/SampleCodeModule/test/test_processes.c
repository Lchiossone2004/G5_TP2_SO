
#include <stdio.h>
#include "../include/test_util.h"
#include "command-lib.h"
#include"test.h"
#include "memory.h"
enum State { RUNNING,
             BLOCKED,
             KILLED };

typedef struct P_rq {
  int32_t pid;
  enum State state;
} p_rq;


int64_t test_processes(uint64_t argc, char *argv[]) {
  uint8_t rq;
  uint8_t alive = 0;
  uint8_t action;
  uint64_t max_processes;
  char *argvAux[] = {0};

  if (argc != 1) 
    return -1;

  if ((max_processes = satoi(argv[0])) <= 0)
    return -1;

  p_rq p_rqs[max_processes];

  while (1) {

        // Create max_processes processes
        for (rq = 0; rq < max_processes; rq++) {
        int32_t pid = usr_create_process(endless_loop,0,argvAux,"endless_loop",3,0);
           
        p_rqs[rq].pid = pid;
        //p_rqs[rq].pid = usr_create_process(endless_loop,0,argvAux,"endless_loop",3,0);
        if (p_rqs[rq].pid == -1) {
            print("test_processes: ERROR creating process\n");
            return -1;
        } else {
            p_rqs[rq].state = RUNNING;
            alive++;
        }
        }

        // Randomly kills, blocks or unblocks processes until every one has been killed
        while (alive > 0) {

            for (rq = 0; rq < max_processes; rq++) {
                action = GetUniform(100) % 2;

                switch (action) {
                case 0:
                    if (p_rqs[rq].state == RUNNING || p_rqs[rq].state == BLOCKED) {
                    if (usr_kill(p_rqs[rq].pid) == -1) {
                        print("test_processes: ERROR killing process\n");
                        return -1;
                    }
                    p_rqs[rq].state = KILLED;
                    alive--;
                    }
                    break;

                case 1:
                    if (p_rqs[rq].state == RUNNING) {
                    if (usr_block_process(p_rqs[rq].pid) == -1) {
                
                        print("test_processes: ERROR blocking process\n");
                        return -1;
                    }
                    p_rqs[rq].state = BLOCKED;
                    }
                    break;
                }
            }

            // Randomly unblocks processes
            for (rq = 0; rq < max_processes; rq++){
                if (p_rqs[rq].state == BLOCKED && GetUniform(100) % 2) {
                if (usr_unblock_process(p_rqs[rq].pid) == -1) {
                    print("test_processes: ERROR unblocking process\n");
                    return -1;
                }
                p_rqs[rq].state = RUNNING;
                }
            }
        }
   
    }
printShell();
return 0;

}