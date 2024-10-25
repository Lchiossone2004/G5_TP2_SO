
#include "../include/shell.h"
#include "../include/libc.h"

static char buffer[100] = {0};
static char *letra;
static int index;

void shell() { 
        while(1){
            getKey();
        }
}

void getKey(){
    getChar(letra);
    if(*letra == 0 && index >0){
        index -= 1;
        buffer[index] = 0;
    }
    else{
    buffer[index++] = *letra;
    }
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

