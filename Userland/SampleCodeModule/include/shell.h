#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <c-lib.h>
#include <string-lib.h>
#include <memory-lib.h>
#include <shell-lib.h>

#define NEW_LINE "shell: >"
#define MAX_ARG_LEN 124
#define MAX_ARGS 5

typedef void (*shell_fn)(uint64_t, char*[], char*);

static shell_fn shell_table[] = {
    [0] = invalid,
    [1] = help,
    [2] = whatTime,
    [3] = zoom,
    [4] = clear,
    [5] = printReg,
    [6] = divCero,
    [7] = invalidOperation,
    [8] = test,
    [9] = kill,
    [10] = ps
};

typedef struct {
    char command[124];
    char ** args;
    int arg_count;
} Command;



#define SHELL_TABLE_SIZE (sizeof(shell_table) / sizeof(shell_fn))

void shell();
void getKey(char *letra, int *index);
void chekCommand(char *buffer, int *index, Command aux);
int processCommand(char *buffer, int * index);
Command parseCommand(char * input);
void freeCommand(Command *cmd);

