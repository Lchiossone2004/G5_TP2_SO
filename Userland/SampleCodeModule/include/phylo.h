#ifndef PHYLO_H
#define PHYLO_H

#define MAX_DINER      10
#define MIN_DINER      3
#define SEM_GLOBAL     100
#define SEM_FORK(i)    (SEM_GLOBAL + 1 + (i))
#define ADD_VALUE      'a'
#define REMOVE_VALUE   'r'
#define QUIT_VALUE     'q'

int phylo_main();
void print_status();
void leave_forks(int idx);
void take_forks(int idx);
void phylo_process(int argc, char *argv[]);
int new_phylo(int idx);
void remove_phylo(int idx);
static void remove_all(int max); 
#endif 