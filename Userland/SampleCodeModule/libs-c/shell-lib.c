#include <shell-lib.h>
#include "test.h"
extern void syscall(__uint64_t rdi, ...);
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

int processCommand(char *buffer, int *ultimaLetra, int * index, char *commands[])
{
    int found = 0;
    int ret = 0;
    char *aux;
    if (ultimaLetra == 0 || (index == 0 && buffer[0] == 0))
    {
        found = 1;
        ret = -1;
    }
    while (ret < NUMBER_OF_COMMANDS && !found)
    {
        aux = commands[ret++];
        found = strCompare(aux, buffer);
    }
    if (found == 0)
    {
        ret = 0;
    }
    return ret;
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