#ifndef SHELL_LIB_H
#define SHELL_LIB_H

#include <time.h>
#include <c-lib.h>
#include <string-lib.h>
#include <memory-lib.h>
#include <time-lib.h>
#include <c-lib.h>

#define WORD_BUFFER_SIZE 1024
#define NUMBER_OF_COMMANDS 20
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

static char *commands[] = {
    "help", "time", "zoom", "clear", "regs", "div cero", "invalid op", "test", "block", 
    "unblock", "nice", "yeild", "kill", "ps", "loop", "mem", "pipe", "cat", "wc", "filter" 
};

static char *commandDescrition[] = {
    "Shows list of available commands.",
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
    "Loops and shows it PID and a special message",
    "Shows the current state of allocated memory.",
    "Creates a pipe between two commands.",  
    "Reads from STDIN and writes to STDOUT.",
    "Counts the number of lines in the input.",
    "Filters the vowels in the input."
};


static char *commandArgs[] = {"all", "UTC - ARG", "IN - OUT", "none", "none", "none", "none", "MM & max_mem - Prio - Proc - Sync", "pid", "pid", "pid & new priority",  "none","pid", "none", "none", "none", "none", "none", "none"};



extern uint64_t syscall(__uint64_t rdi, ...);
extern void invalidOp();


void help(uint64_t argc, char *argv[], char* command, int is_foregorund);
void whatTime(uint64_t argc, char *argv[], char* command, int is_foregorund);
void zoom(uint64_t argc, char *argv[], char* command, int is_foregorund);
void clear(uint64_t argc, char *argv[], char* command, int is_foregorund);
void printReg(uint64_t argc, char *argv[], char* command, int is_foregorund);
void divCero(uint64_t argc, char *argv[], char* command, int is_foregorund);
void invalidOperation(uint64_t argc, char *argv[], char* command, int is_foregorund);
void test(uint64_t argc, char *argv[], char* command, int is_foregorund);
void block(uint64_t argc, char *argv[], char* command, int is_foregorund);
void unblock(uint64_t argc, char *argv[], char* command, int is_foregorund);
void nice(uint64_t argc, char *argv[], char* command, int is_foregorund);
void yeild(uint64_t argc, char *argv[], char* command, int is_foregorund);
void kill(uint64_t argc, char *argv[], char* command, int is_foregorund);
void ps(uint64_t argc, char *argv[], char* command, int is_foregorund);
void loop(uint64_t argc, char *argv[], char* command, int is_foregorund); //Falta hacer
void mem(uint64_t argc, char *argv[], char* command, int is_foregorund);
void invalid(uint64_t argc, char *argv[], char* command, int is_foregorund);
void argsError(uint64_t argc, char *argv[]);
void commandInfo(int i, int j);
void cat(uint64_t argc, char *argv[], char* command, int is_foreground);
void wc(uint64_t argc, char *argv[], char* command, int is_foreground);
void filter(uint64_t argc, char *argv[], char* command, int is_foreground);
void pipeCommand(uint64_t argc, char *argv[], char *command, int is_foregorund);


#endif
