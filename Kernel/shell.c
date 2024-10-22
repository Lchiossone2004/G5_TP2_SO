#include <videoDriver.h>
#include <keyboard.h>

void shell() {
    

}
char* getCommand(){
    char pressed;
    char* command;
    while (1) {
        pressed = getKey(); // Lee una tecla
        if(pressed ==28){
            for (int i=1; matriz[actual_line][i] && i <=BORDER_X; i++){
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
}

