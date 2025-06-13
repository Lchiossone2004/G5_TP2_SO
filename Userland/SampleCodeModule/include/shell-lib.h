#ifndef SHELL_LIB_H
#define SHELL_LIB_H

#include <time.h>
#include <c-lib.h>
#include <string-lib.h>
#include <memory-lib.h>
#include <time-lib.h>
#include <c-lib.h>

#define WORD_BUFFER_SIZE 1024
#define NUMBER_OF_COMMANDS 22
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
    "help", "time", "zoom", "clear", "regs", "divCero", "invalidOp", "test", "block", 
    "unblock", "nice", "yeild", "kill", "ps", "loop", "mem", "cat", "wc", "filter","shell", "phylo","echo"
};

static char *commandDescrition[] = {
    "Shows list of available commands.",
    "It prints the actual time.",
    "Zooms in or out dependng on the argument.",
    "Clears the screen.",
    "Prints the saved cpu registers",
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
    "Reads from STDIN and writes to STDOUT.",
    "Counts the number of lines in the input.",
    "Filters the vowels in the input.",
    "Opens a new shell.",
    "Dining philosophers problem.",
    "Prints to STDOUT"
};


static char *commandArgs[] = {"none", "UTC - ARG", "IN - OUT", "none", "none", "none", "none", "MM & max_mem - Prio - Proc - Sync", "pid", "pid", 
                            "pid & new priority",  "none","pid", "none", "none", "none", "none", "none","none", "none", "none", "what to print"};



extern uint64_t syscall(__uint64_t rdi, ...);
extern void invalidOp();


int help(uint64_t argc, char *argv[], char* command, int is_foregorund);
int whatTime(uint64_t argc, char *argv[], char* command, int is_foregorund);
int zoom(uint64_t argc, char *argv[], char* command, int is_foregorund);
int clear(uint64_t argc, char *argv[], char* command, int is_foregorund);
int printReg(uint64_t argc, char *argv[], char* command, int is_foregorund);
int divCero(uint64_t argc, char *argv[], char* command, int is_foregorund);
int invalidOperation(uint64_t argc, char *argv[], char* command, int is_foregorund);
int test(uint64_t argc, char *argv[], char* command, int is_foregorund);
int block(uint64_t argc, char *argv[], char* command, int is_foregorund);
int unblock(uint64_t argc, char *argv[], char* command, int is_foregorund);
int nice(uint64_t argc, char *argv[], char* command, int is_foregorund);
int yeild(uint64_t argc, char *argv[], char* command, int is_foregorund);
int kill(uint64_t argc, char *argv[], char* command, int is_foregorund);
int ps(uint64_t argc, char *argv[], char* command, int is_foregorund);
int loop(uint64_t argc, char *argv[], char* command, int is_foregorund);
int mem(uint64_t argc, char *argv[], char* command, int is_foregorund);
int invalid(uint64_t argc, char *argv[], char* command, int is_foregorund);
int argsError(uint64_t argc, char *argv[]);
int commandInfo(int i);
int cat(uint64_t argc, char *argv[], char* command, int is_foreground);
int wc(uint64_t argc, char *argv[], char* command, int is_foreground);
int filter(uint64_t argc, char *argv[], char* command, int is_foreground);
int echo(uint64_t argc, char *argv[], char* command, int is_foreground);
int newShell(uint64_t argc, char *argv[], char *command, int is_foregorund);
int phylosophers(uint64_t argc, char *argv[], char *command, int is_foregorund);


#endif
