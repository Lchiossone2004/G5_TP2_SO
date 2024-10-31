#include "../include/libc.h"
#include <stdlib.h>
#include <stdio.h>

#define NEW_LINE "shell: >"
#define TAB "     "
#define WORD_BUFFER_SIZE 1024
#define NUMBER_OF_COMMANDS 9
#define STDOUT 0
#define STDIN 1
#define STDERR 2

void shell();
void getKey(char * buffer, int index);
void chekCommand();
void clearBuffer();
int processCommand();
void clear();
void help();
