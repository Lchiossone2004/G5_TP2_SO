#include <shell-lib.h>
#include "test.h"
extern uint64_t syscall(__uint64_t rdi, ...);
extern void invalidOp();


void getKey(char *letra, int *index)
{
    syscall(2, STDIN, letra, index);
}

void chekCommand(char *buffer, int *index, int *ultimaLetra, char *commands[])
{
    int command = processCommand(buffer,ultimaLetra,index,commands);
    if (command == 1)
    {
        help(commands);
    }
    if (command == 2)
    {
        print(TAB);
        printTime(1);
    }
    if (command == 3)
    {
        print(TAB);
        printTime(0);
    }
    if (command == 4)
    {
        syscall(6, STDOUT);
    }
    if (command == 5)
    {
        syscall(7, STDOUT);
    }
    if (command == 6)
    {
        clear();
    }
    if (command == 7)
    {
        syscall(1, STDOUT);
    }
    if (command == 8)
    {
        int i = 0 / 0;
    }
    if (command == 9)
    {
        invalidOp();
    }
    if (command == 10)
    {
        void * aux = test_mm;
        char* args[] = {"hola", "bunas"};
        syscall(22,aux,2,args, "test_mm", 3);
        print_usr_mem_info();
    }
    if (command == 11)
    {
        nlPrint();
        print(TAB);
        print("closing shell...");
        return;
    }
    if (command == 12) {

    char *argStr = buffer + 5;
    
    int valid = 1;
    for (int i = 0; argStr[i] != 0; i++) {
        if (argStr[i] < '0' || argStr[i] > '9') {
            valid = 0;
            break;
        }
    }

    if (!valid) {
        printErr("kill: invalid PID");
     
    } else {

    int pid = strToInt(argStr);
    int notFoundProcess = syscall(23, pid);
    if(notFoundProcess) {
        printErr("kill: process not found");
    }
    }
}

    if (command == 13) {
        syscall(29);
    }
    if (command == 0)
    {
        print(TAB);
        printErr("command: [");
        printErr(buffer);
        printErr("] not found.");
    }
    if (command != 6 && command != -1 && command != 4 && command != 5 && command != 7 && command != 8)
    {
        nlPrint();
    }
    print(NEW_LINE);
    *index = 0;
    *ultimaLetra = 0;
}

void clearBuffer(char *buffer, int *ultimaLetra)
{
    int i = 0;
    int j = 0;
    while (buffer[i] == ' ' && i <= *ultimaLetra)
    {
        i++;
    }
    while (i <= *ultimaLetra)
    {
        buffer[j++] = buffer[i++];
    }
    buffer[j - 1] = 0;
    return;
}

int processCommand(char *buffer, int *ultimaLetra, int *index, char *commands[]) {
    if (ultimaLetra == 0 || (index == 0 && buffer[0] == 0)) {
        return -1;
    }

    for (int j = 0; j < NUMBER_OF_COMMANDS; j++) {
        if (strCompare(commands[j], buffer)) {
            return j + 1; 
        }
    }

   
    char commandPart[WORD_BUFFER_SIZE] = {0};
    int i = 0;
    while (buffer[i] != ' ' && buffer[i] != 0 && i < WORD_BUFFER_SIZE - 1) {
        commandPart[i] = buffer[i];
        i++;
    }
    commandPart[i] = 0;

  
    for (int j = 0; j < NUMBER_OF_COMMANDS; j++) {
        if (strCompare(commands[j], commandPart)) {
            return j + 1;
        }
    }

    return 0;
}


void clear()
{
    syscall(9, STDOUT);
}

void help(char *commands[])
{
    print("     Here is a list of the commands:");
    for (int i = 0; i < NUMBER_OF_COMMANDS; i++)
    {
        nlPrint();
        print("     #) ");
        print(commands[i]);
    }
}
int strToInt(const char *str) {
    int res = 0;
    while (*str >= '0' && *str <= '9') {
        res = res * 10 + (*str - '0');
        str++;
    }
    return res;
}
