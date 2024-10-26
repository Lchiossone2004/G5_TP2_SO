
#include "../include/shell.h"
#include "../include/libc.h"

static char buffer[WORD_BUFFER_SIZE] = {0};
static char *letra;
static int ultimaLetra;
static int index = 0;
static char* commands[] = {"help", "time", "deci spe", "zoomin", "zoomout"};

void shell() { 
        print(NEW_LINE,sizeof(NEW_LINE));
        while(1){
         getChar(letra,index);
        if(*letra == 0 && index > 0){
            index -= 1;
            buffer[index] = 0;
        }
        if (*letra == 1){
            chekCommand();
        }  
        if(*letra != 0 && *letra != 1){
            buffer[index++] = *letra;
            if(*letra != ' '){
                ultimaLetra = index;
            }
        }
    }
}

void chekCommand(){
    int command = compare();
    if(command == 1){
        print("help", 4);
    }
    if(command == 2){
        print("time", 4);
    }
    if(command == 3){
        print("sapee",5);
    }
    if(command==4){
        zoomIn();
    }
    if (command==5)
    {
        zoomOut();
    }
    if(command == 0){
        printErr("command not found",17);
    }
    nlPrint();
    print(NEW_LINE,sizeof(NEW_LINE));
    index = 0;
}

int compare(){
    int found = 0;
    int flag = 1;
    char * aux;
    int ret = 0;
    while(ret < 5 && !found){
        aux = commands[ret];
        flag = 1;
        if(ultimaLetra != size(aux)){
            flag = 0;
        }
        for(int i = 0; i<ultimaLetra && flag; i++){
            if(!(buffer[i] == aux[i])){
                flag = 0;
            }
        }
        if(flag == 1){
            found = 1;
        }
        ret++;
    }
    if(found == 0){
        ret = 0;
    }
    return ret;
}
int size(char * word){
    int toRet = 0;
    while(word[toRet] != 0){
        toRet++;
    }
    return toRet;
}
