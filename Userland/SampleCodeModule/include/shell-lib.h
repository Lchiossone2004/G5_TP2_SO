#ifndef SHELL_LIB_H
#define SHELL_LIB_H

#include <time.h>
#include <c-lib.h>
#include <string-lib.h>
#include <memory-lib.h>
#include <time-lib.h>
#include <c-lib.h>

#define WORD_BUFFER_SIZE 1024
#define NUMBER_OF_COMMANDS 14
#define STDIN 0
#define STDOUT 1
#define STDERR 2
#define TAB "     "
#define NEW_LINE "shell: >"
#define STDIN 0
#define STDOUT 1
#define STDERR 2



void getKey(char *letra, int *index);
void chekCommand(char *buffer, int *index, int *ultimaLetra, char *commands[]);
void clearBuffer(char *buffer, int *ultimaLetra);
int processCommand(char *buffer, int *ultimaLetra, int * index, char *commands[]);
void clear();
void help(char *commands[]);

#endif