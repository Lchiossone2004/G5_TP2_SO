
#include "../include/shell.h"
#include "../include/libc.h"

static char buffer[100] = {0};
static char *letra;
static int index;
#define NEW_LINE " >"


void shell() { 
        print(NEW_LINE,sizeof(NEW_LINE));
        while(1){
         getChar(letra);
        if(*letra == 0 && index >0){
            index -= 1;
            buffer[index] = 0;
        }
        else if (*letra == 1){
            index = 0;
            chekCommand();
        }  
        else{
            buffer[index++] = *letra;
        }
    }
}

void chekCommand(){
    print(NEW_LINE,sizeof(NEW_LINE));
    index = 0;
}
char* getCommand(char* buffer){
    char pressed;
    char* command;
   // print(read());
    /*
    while (1) {
        pressed = getKey(); // Lee una tecla
        print(pressed);
        matriz[actual_line][pos_x++]=pressed;
        if(pressed ==28){
            actual_line++;
            for (int i=0; matriz[actual_line][i] && i <=BORDER_X; i++){
                command[i]=matriz[actual_line][i];
            }
            if (strcmp(command, "exit") == 0) {
                imprimirVideo("Saliendo de la shell...\n");
    
            } else if (strcmp(command, "app1") == 0) {
                imprimirVideo("Iniciando aplicacion 1...\n");
            } else if (strcmp(command, "app2") == 0) {
                imprimirVideo("Iniciando aplicacion 2...\n");
            } else {
                imprimirVideo("Comando no reconocido\n");
            }
        }

    }

    */
}

