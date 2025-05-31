#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#define NEW_LINE "shell: >"
#define TAB "     "
#define WORD_BUFFER_SIZE 1024
#define NUMBER_OF_COMMANDS 12
#define STDIN 0
#define STDOUT 1
#define STDERR 2

void startShell();
void shell();
void getKey(char *buffer, int index);
void chekCommand();
void clearBuffer();
int processCommand();
void clear();
void help();
