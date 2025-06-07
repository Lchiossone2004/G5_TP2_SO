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

typedef void (*shell_fn)(uint64_t, char*[], char*,int);

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
    [9] = block,
    [10] = unblock,
    [11] = nice,
    [12] = yeild,
    [13] = kill,
    [14] = ps,
    [15] = loop,
    [16] = mem,
    [17] = pipeCommand
};

typedef struct {
    char command[124];
    char ** args;
    int arg_count;
} Command;



#define SHELL_TABLE_SIZE (sizeof(shell_table) / sizeof(shell_fn))

void shell();
void getKey();
void chekCommand(Command aux);
int processCommand(char *input);
void clearBuffer();
Command parseCommand(char * input);
void freeCommand(Command *cmd);


