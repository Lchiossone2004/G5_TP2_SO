
#include "../include/shell.h"
#include "../include/snake.h"
#include "../include/time.h"

extern void syscall(__uint64_t rdi, ...);

static char buffer[WORD_BUFFER_SIZE] = {0};
static char letra[1] = {0};
static int ultimaLetra;
static int index = 0;
static char* commands[] = {"help", "time ARG","time UTC", "zoomin", "zoomout","clear", "snake","print registers","exit"};
static shell_is_active=1;

void shell() { 
        print(NEW_LINE,sizeof(NEW_LINE)-1);
        while(shell_is_active){
        if(index == WORD_BUFFER_SIZE-1){
            clearBuffer();
            chekCommand();
        }
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
        //help(); 
        div0(); 
        //int i = 0/0;
    }
    if(command == 2){
        print(TAB,sizeof(TAB)-1);
        printTime(1);
      
    }
    if(command == 3){
        print(TAB,sizeof(TAB)-1);
        printTime(0);
    }
    if(command==4){
        zoomIn();
        
    }
    if (command==5)
    {
        zoomOut();
        
    }
    if(command == 6){
        clear();
       
    }
    if(command == 7) {
        nlPrint();
        char buffer;
        do {
        print("type '1' for one player, '2' for two players",44);
        nlPrint();
        syscall(2,1,&buffer,1);
        nlPrint();
        }while (buffer != '1' && buffer != '2');
        play(buffer);
        syscall(16);
        syscall(9,STDIN);
    }
    if(command == 8){
        syscall(1);  
    }
    if(command == 9){
        nlPrint();
        print(TAB,sizeof(TAB)-1);
        print("closing shell...", 16);
        return;
    }
    if(command == 0){
        print(TAB,sizeof(TAB)-1);
        printErr("command: [", 10);
        printErr(buffer, strSize(buffer));
        printErr("] not found.", 12);
    }
    if(command != 6 && command != -1 && command != 4 && command != 5 && command != 7){
    nlPrint();
    }
    print(NEW_LINE,sizeof(NEW_LINE)-1);
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
    if(ultimaLetra == 0 || (index == 0 && buffer[0] == 0)){
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

void help(){
    print("     Here is a list of the commands:",36);
    nlPrint();
    for(int i = 0; i<NUMBER_OF_COMMANDS; i++){
        print("     #) ",7);
        print(commands[i],strSize(commands[i]));
        nlPrint();
    }
}