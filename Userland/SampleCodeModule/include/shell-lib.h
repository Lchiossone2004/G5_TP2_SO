#ifndef SHELL_LIB_H
#define SHELL_LIB_H

#include <time.h>
#include <c-lib.h>
#include <string-lib.h>
#include <memory-lib.h>
#include <time-lib.h>
#include <c-lib.h>

#define WORD_BUFFER_SIZE 1024
#define NUMBER_OF_COMMANDS 10
#define STDIN 0
#define STDOUT 1
#define STDERR 2
#define TAB "     "
#define NEW_LINE "shell: >"
#define STDIN 0
#define STDOUT 1
#define STDERR 2
#define MAX_ARG_LEN 124
#define MAX_ARG 5

static char *commands[] = {"help", "time", "zoom", "clear", "print registers", "div cero", "invalid op", "testmm", "kill", "ps"};

void help(uint64_t argc, char *argv[], char* command);
void whatTime(uint64_t argc, char *argv[], char* command);
void zoom(uint64_t argc, char *argv[], char* command);
void clear(uint64_t argc, char *argv[], char* command);
void printReg(uint64_t argc, char *argv[], char* command);
void divCero(uint64_t argc, char *argv[], char* command);
void invalidOperation(uint64_t argc, char *argv[], char* command);
void test(uint64_t argc, char *argv[], char* command);
void kill(uint64_t argc, char *argv[], char* command);
void ps(uint64_t argc, char *argv[], char* command);
void invalid(uint64_t argc, char *argv[], char* command);
void trowError(uint64_t argc, char *argv[]);






#endif