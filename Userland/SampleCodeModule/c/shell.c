#include "../include/shell.h"
#include "../include/libc.h"

static char buffer[100];

void shell() {    
    read(buffer);
    print(buffer);
    while(1);
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

