
#include "../include/shell.h"
#include "../include/libc.h"
#include <stdlib.h>
#include <stdio.h>

static char buffer[WORD_BUFFER_SIZE] = {0};
static char word[WORD_BUFFER_SIZE];
static char *letra;
static int ultimaLetra;
static int index = 0;
static char* commands[] = {"help", "time", "zoomin", "zoomout"};

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
                if(*letra == ' '){
                buffer[index++] = 0;
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
    if(command == 0){
        printErr("command: [", 10);
        printErr(word, strSize(word));
        printErr("] not found.", 12);
    }
    nlPrint();
    print(NEW_LINE,sizeof(NEW_LINE));
    index = 0;
}

void getCommand(){
    int i = 0;
    int j = 0;
    while(buffer[i] == 0){
        i++;
    }
    while(buffer[i] != 0){
        word[j++] = buffer[i++];
    }
    word[j] = 0;
    return word;
}

int processCommand(){
    int found = 0;
    int ret = 0;
    char * s1;
    getCommand();
    while(ret < NUMBER_OF_COMMANDS && !found){
        s1 = commands[ret++];
        found = strCompare(s1,word);
    }
    if(found == 0){
        ret = 0;
    }
    return ret;
}
