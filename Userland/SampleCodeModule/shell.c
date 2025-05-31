#include <shell.h>
#include <time.h>
#include <c-lib.h>
#include <string-lib.h>
#include <memory-lib.h>
#include <shell-lib.h>

static char buffer[WORD_BUFFER_SIZE] = {0};
static char letra[1] = {0};
static int ultimaLetra;
static int index = 0;
static char *commands[] = {"help", "time ARG", "time UTC", "zoomin", "zoomout", "clear", "print registers", "div cero", "invalid op", "testmm", "exit"};

extern void syscall(__uint64_t rdi, ...);

void startShell(){
    char* args[] = {"hola", "bunas"};
    syscall(22,(void*)shell,2,args, "shell", 5);//prueba con prioridad dummy alta
}
void shell()
{
    print(NEW_LINE);
    while (1)
    {
        if (index == WORD_BUFFER_SIZE - 1)
        {
            clearBuffer(buffer,&ultimaLetra);
            chekCommand(buffer,&index,&ultimaLetra,commands);
        }
        getKey(letra, &index);
        if (*letra == 0 && index > 0)
        {
            index -= 1;
            buffer[index] = 0;
        }
        if (*letra == 1)
        {
            clearBuffer(buffer,&ultimaLetra);
            chekCommand(buffer,&index,&ultimaLetra,commands);
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
