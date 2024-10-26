
#include "../include/shell.h"
#include "../include/libc.h"
#include <stdlib.h>
#include <stdio.h>

static char buffer[WORD_BUFFER_SIZE] = {0};
static char *letra;
static int ultimaLetra;
static int index = 0;
static char* commands[] = {"help", "time", "zoomin", "zoomout","history"};

void shell() { 
        print(NEW_LINE,sizeof(NEW_LINE));
        while(1){
        getChar(letra,index);
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
    if(command == 0){
        printErr("command: [", 10);
        printErr(buffer, strSize(buffer));
        printErr("] not found.", 12);
    }
    nlPrint();
    print(NEW_LINE,sizeof(NEW_LINE));
    index = 0;
}

void clearBuffer(){
    int i = 0;
    int j = 0;
    while(buffer[i] == ' '){
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
    while(ret < NUMBER_OF_COMMANDS && !found){
        aux = commands[ret++];
        found = strCompare(aux,buffer);
    }
    if(found == 0){
        ret = 0;
    }
    return ret;
}
