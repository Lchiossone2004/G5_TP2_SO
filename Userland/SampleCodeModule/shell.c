#include <shell.h>
#include <time.h>
#include <c-lib.h>
#include <string-lib.h>
#include <memory-lib.h>

extern void syscall(__uint64_t rdi, ...);
extern void invalidOp();

static char buffer[WORD_BUFFER_SIZE] = {0};
static char letra[1] = {0};
static int ultimaLetra;
static int index = 0;
static char *commands[] = {"help", "time ARG", "time UTC", "zoomin", "zoomout", "clear", "print registers", "div cero", "invalid op", "testmm", "exit"};

void shell()
{
    print(NEW_LINE);
    while (1)
    {
        if (index == WORD_BUFFER_SIZE - 1)
        {
            clearBuffer();
            chekCommand();
        }
        getKey(letra, index);
        if (*letra == 0 && index > 0)
        {
            index -= 1;
            buffer[index] = 0;
        }
        if (*letra == 1)
        {
            clearBuffer();
            chekCommand();
        }
        if (*letra != 0 && *letra != 1)
        {
            if (*letra == ' ')
            {
                buffer[index++] = ' ';
            }
            else
            {
                buffer[index++] = *letra;
                ultimaLetra = index;
            }
            buffer[index] = 0;
        }
    }
}

void getKey(char *buffer, int index)
{
    syscall(2, STDIN, buffer, index);
}

void chekCommand()
{
    int command = processCommand();
    if (command == 1)
    {
        help();
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
        // Ejecutar test de memoria
        print(TAB);
        print("testeando\n");
        int* block1 = usr_malloc(10);
        int* block2 = usr_malloc(10);
        int* block3 = usr_malloc(10);
        usr_free(block1);  // Free first block
        usr_free(block3);  // Free last block, leaving middle block allocated
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
    index = 0;
    ultimaLetra = 0;
}

void clearBuffer()
{
    int i = 0;
    int j = 0;
    while (buffer[i] == ' ' && i <= ultimaLetra)
    {
        i++;
    }
    while (i <= ultimaLetra)
    {
        buffer[j++] = buffer[i++];
    }
    buffer[j - 1] = 0;
    return;
}

int processCommand()
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

void help()
{
    print("     Here is a list of the commands:");
    for (int i = 0; i < NUMBER_OF_COMMANDS; i++)
    {
        nlPrint();
        print("     #) ");
        print(commands[i]);
    }
}