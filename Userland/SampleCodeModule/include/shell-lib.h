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
#define TAB "     "
#define NEW_LINE "shell: >"
#define STDIN 0
#define STDOUT 1
#define STDERR 2

static char *commands[] = {"help", "time", "zoom", "clear", "print registers", "div cero", "invalid op", "test", "kill", "ps"};

static char *commandDescrition[] ={ "It gives a list of the possible commands that can be used is this Shell.",
                                    "It prints the actual time.",
                                    "Zooms in or out dependng on the argument.",
                                    "Clears the screen.",
                                    "Prints the saved cpu registers (registers can be saved with 'esc' key).",
                                    "Causes a division by cero to show the error handeling.",
                                    "Causes a invalid operation to show the error handeling.",
                                    "Allows to run tests on diferent functionalities.",
                                    "Kills the selected process.",
                                    "Shows a list of the existing processes"
                                };

static char *commandArgs[] = {"all", "UTC - ARG", "IN - OUT", "none", "none", "none", "none", "MM - Prio - Processes - Sync", "pid", "none"};

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
void argsError(uint64_t argc, char *argv[]);
void commandInfo(int commandNum);






#endif