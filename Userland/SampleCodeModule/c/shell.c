
#include "../include/shell.h"


static char buffer[WORD_BUFFER_SIZE] = {0};
static char *letra;
static int ultimaLetra;
static int index = 0;
static char* commands[] = {"help", "time", "zoomin", "zoomout","history","clear"};

void shell() { 
        print(NEW_LINE,sizeof(NEW_LINE));
        while(1){
        getKey(letra,index);
        if(*letra == 0 && index > 0){
            index -= 1;
            buffer[index] = 0;
        }
        if (*letra == 1){
            clearBuffer();
            chekCommand();
        }  
        if(*letra != 0 && *letra != 1){
                if(*letra == ' '){
                buffer[index++] = ' ';
                }
                else{
                buffer[index++] = *letra;
                ultimaLetra = index;
                }
                buffer[index] = 0;
        }
    }
}

void getKey(char * buffer, int index){
    syscall(2,STDOUT,buffer,index);
}

void chekCommand(){
    int command = processCommand();
    if(command == 1){
        print("help", 4);
    }
    if(command == 2){
        print("time", 4);
    }
    if(command==3){
        zoomIn();
    }
    if (command==4)
    {
        zoomOut();
    }
    if (command==5)
    {
        history();
    }
    if(command == 6){
        clear();
    }
    if(command == 0){
        printErr("     command: [", 15);
        printErr(buffer, strSize(buffer));
        printErr("] not found.", 12);
    }
    if(command != 6 && command != -1){
    nlPrint();
    }
    print(NEW_LINE,sizeof(NEW_LINE));
    index = 0;
    ultimaLetra = 0;
}

void clearBuffer(){
    int i = 0;
    int j = 0;
    while(buffer[i] == ' ' && i <= ultimaLetra){
        i++;
    }
    while(i <= ultimaLetra){
        buffer[j++] = buffer[i++];
    }
    buffer[j-1] = 0;
    return;
}

int processCommand(){
    int found = 0;
    int ret = 0;
    char * aux;
    if(ultimaLetra == 0){
        found = 1;
        ret = -1;
    }
    while(ret < NUMBER_OF_COMMANDS && !found){
        aux = commands[ret++];
        found = strCompare(aux,buffer);
    }
    if(found == 0){
        ret = 0;
    }
    return ret;
}

void clear(){
    syscall(9,STDIN);
}