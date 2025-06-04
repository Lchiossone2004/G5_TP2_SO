#ifndef SHELL_LIB_H
#define SHELL_LIB_H

#include <time.h>
#include <c-lib.h>
#include <string-lib.h>
#include <memory-lib.h>
#include <time-lib.h>
#include <c-lib.h>

#define WORD_BUFFER_SIZE 1024
#define NUMBER_OF_COMMANDS 15
#define TAB "     "
#define NEW_LINE "shell: >"
#define STDIN 0
#define STDOUT 1
#define STDERR 2

typedef enum {
    PRIORITY_CRITICAL = 0,  
    PRIORITY_HIGH = 1,     
    PRIORITY_NORMAL = 2,      
    PRIORITY_LOW = 3         
} PriorityLevel;

static char *commands[] = {"help", "time", "zoom", "clear", "regs", "div cero", "invalid op", "test", "block" , "unblock", "nice" ,"yeild","kill", "ps","mem"};

static char *commandDescrition[] ={ "Shows list of available commands.",
                                    "It prints the actual time.",
                                    "Zooms in or out dependng on the argument.",
                                    "Clears the screen.",
                                    "Prints the saved cpu registers/",
                                    "Causes a division by cero to show the error handeling.",
                                    "Causes a invalid operation to show the error handeling.",
                                    "Allows to run tests on diferent functionalities.",
                                    "Blocks a specific process",
                                    "Unblock a sprecific process",
                                    "Changes a process priority",
                                    "Gives up the Cpu",
                                    "Kills the selected process.",
                                    "Shows a list of the existing processes.",
                                    "Shows the current state of allocated memory."
                                };

static char *commandArgs[] = {"all", "UTC - ARG", "IN - OUT", "none", "none", "none", "none", "MM - Prio - Processes - Sync", "pid", "pid", "pid & new priority",  "none","pid", "none","none"};

void help(uint64_t argc, char *argv[], char* command);
void whatTime(uint64_t argc, char *argv[], char* command);
void zoom(uint64_t argc, char *argv[], char* command);
void clear(uint64_t argc, char *argv[], char* command);
void printReg(uint64_t argc, char *argv[], char* command);
void divCero(uint64_t argc, char *argv[], char* command);
void invalidOperation(uint64_t argc, char *argv[], char* command);
void test(uint64_t argc, char *argv[], char* command);
void block(uint64_t argc, char *argv[], char* command);
void unblock(uint64_t argc, char *argv[], char* command);
void nice(uint64_t argc, char *argv[], char* command);
void yeild(uint64_t argc, char *argv[], char* command);
void kill(uint64_t argc, char *argv[], char* command);
void ps(uint64_t argc, char *argv[], char* command);
void mem(uint64_t argc, char *argv[], char* command);
void invalid(uint64_t argc, char *argv[], char* command);
void argsError(uint64_t argc, char *argv[]);
void commandInfo(int i,int j);






#endif