#include <shell-lib.h>
#include <stdint.h>
#include "string-lib.h"
#include "test.h"
#include "c-lib.h"
#include "shell.h"
#include "command-lib.h"
#include "phylo.h"

#define EOF '\0'

static int shellCounter = 0;
void help(uint64_t argc, char *argv[], char* command, int is_foregorund){
    if(argc == 0){
        print("     Here is a list of the commands:");
        for (int i = 0; i < NUMBER_OF_COMMANDS; i++)
        {
            print("\n");
            print("     #) ");
            print(commands[i]);
        }
        print("\n");
        print("To know more about any command type [command] -info");
        print("\n");
    }
    else if(argc == 1 && strCompare(argv[0], "all")){
        for(int i = 0; i < NUMBER_OF_COMMANDS; i++){
            if(i<NUMBER_OF_COMMANDS-1){
                commandInfo(i,i+1);
                i++;
            }
            else{
                commandInfo(i,-1);
            }
            print("\n");
        }
    }
    else{
        argsError(argc,argv);
    }
}

void whatTime(uint64_t argc, char *argv[], char* command, int is_foregorund){
    if(argc != 1){
        argsError(argc,argv);
        return;
    }
    if(strCompare(argv[0], "ARG")){
        printTime(1);
    }
    else if(strCompare(argv[0], "UTC")){
        printTime(0);
    }
    else{
        argsError(argc,argv);
        return;
    }
    print(TAB);
}

void zoom(uint64_t argc, char *argv[], char* command, int is_foregorund){
    if(argc != 1){
        argsError(argc,argv);
    }
    if(strCompare(argv[0],"IN")){
        syscall(5, STDOUT);
    }
    if(strCompare(argv[0],"OUT")){
        syscall(5, STDOUT);
    }
    print("\n");
}

void clear(uint64_t argc, char *argv[], char* command, int is_foregorund){
    if(argc > 0){
        argsError(argc,argv);
    }
    syscall(6, STDOUT);
}

void printReg(uint64_t argc, char *argv[], char* command, int is_foregorund){
    if(argc > 0){
        argsError(argc,argv);
    }
    syscall(1, STDOUT);
}

void divCero(uint64_t argc, char *argv[], char* command, int is_foregorund){
    if(argc > 0){
        argsError(argc,argv);
    }
    int aux = 0/0;
}
void invalidOperation(uint64_t argc, char *argv[], char* command, int is_foregorund){
    if(argc > 0){
        argsError(argc,argv);
    }
    invalidOp();
}

int test(uint64_t argc, char *argv[], char* command, int is_foregorund){
    if(argc == 1){
        argsError(argc,argv);
    }
    if(strCompare(argv[0],"MM")){

        return usr_create_process((void*)test_mm, argc-1,argv+1, "memory test", PRIORITY_LOW, is_foregorund);
    }
    else if(strCompare(argv[0],"Prio")){

        return usr_create_process((void*)test_prio,argc,argv, "priority test", PRIORITY_LOW,is_foregorund);
    }
    else if(strCompare(argv[0],"Proc")){
        return usr_create_process((void*)test_processes,argc -1,argv+1, "processes test", PRIORITY_LOW, is_foregorund);

    }
    else if(strCompare(argv[0],"Sync")){
        argc = 2;
        argv[0] = argv[1];  //QUE ES ESTO?
        argv[1] = argv[2];
        return usr_create_process((void*)test_sync,argc,argv, "sync test", PRIORITY_LOW, is_foregorund);
    }
}

void block(uint64_t argc, char *argv[], char* command, int is_foregorund){
    if(argc != 1){
        argsError(argc,argv);
    }
    else{
        int pid = strToInt(argv[0]);
        usr_block_process(pid);
    }
}

void unblock(uint64_t argc, char *argv[], char* command, int is_foregorund){
    if(argc != 1){
        argsError(argc,argv);
    }
    else{
        int pid = strToInt(argv[0]);
        usr_unblock_process(pid);
    }
}

void nice(uint64_t argc, char *argv[], char* command, int is_foregorund){
    if(argc != 2){
        argsError(argc,argv);
    }
    else{
        int pid = strToInt(argv[0]);
        int newPrio = strToInt(argv[1]);
        usr_nice(pid, newPrio);
    }

}

void yeild(uint64_t argc, char *argv[], char* command, int is_foregorund){
    if(argc > 0){
        argsError(argc,argv);
    }
    else{
        usr_yield();
    }
}

void kill(uint64_t argc, char *argv[], char* command, int is_foregorund){
    if(argc != 1){
        argsError(argc,argv);
    }
    else{
        print(argv[0]);
        int pid = strToInt(argv[0]);
        if(pid >0){
            int notFoundProcess = usr_kill(pid);
            print(TAB);
        if(notFoundProcess) {
            printErr("kill: process not found");
        }
        }
        else{
            printErr("Invalid PID");
        }
        print("\n");
    }
}

void ps(uint64_t argc, char *argv[], char* command, int is_foregorund){
    if(argc > 0){
        argsError(argc,argv);
    }
    else{
        syscall(20);
    }
}

int mem(uint64_t argc, char *argv[], char* command, int is_foregorund){
    if(argc > 0){
        argsError(argc,argv);
    }
    else{
        return usr_create_process((void*)mem_command, argc, argv, "mem", PRIORITY_LOW, is_foregorund);
    }
}

int loop(uint64_t argc, char *argv[], char* command, int is_foregorund){
   if(argc > 0) {
        argsError(argc,argv);
    } else {
        return usr_create_process((void*)loop_command, argc, argv, "loop", PRIORITY_LOW, is_foregorund);
    }
   }

int cat(uint64_t argc, char *argv[], char* command, int is_foreground) {
    if(argc > 0){
        argsError(argc,argv);
    } else 
    return usr_create_process((void*)cat_command, argc, argv, "cat", PRIORITY_LOW, is_foreground);
}

int wc(uint64_t argc, char *argv[], char* command, int is_foreground) {
     if(argc > 0){
        argsError(argc,argv);
    } else 
    return usr_create_process((void*)wc_command, argc, argv, "wc", PRIORITY_LOW, is_foreground);
}

int filter(uint64_t argc, char *argv[], char* command, int is_foreground) {
     if(argc > 0){
        argsError(argc,argv);
    } else 
    return usr_create_process((void*)filter_command, argc, argv, "filter", PRIORITY_LOW, is_foreground);
}

void invalid(uint64_t argc, char *argv[], char* command, int is_foregorund){
    print(TAB);
    printErr("command: [");
    printErr(command);
    printErr("] not found.\n");
}

void argsError(uint64_t argc, char *argv[]){
    print(TAB);
    if(argc == 0){
        printErr("Missing Arguments.");
    }
    else{
        printErr("Invalid Args:");
        for(int i= 0; i<argc;i++){
                print(" ");
                printErr(argv[i]);
        }
    }
    print("\n");
}

void commandInfo(int i,int j){
    (void) commandDescrition;
    (void) commandArgs;

            print(TAB);
            print("- This is the [");
            print(commands[i]);
            print("] command.");
            if(j != -1){
                syscall(39);
                print("- This is the [");
                print(commands[j]);
                print("] command.");
            }
            print("\n");
            print(TAB);
            print("- ");
            print(commandDescrition[i]);
            if(j!= -1){
                syscall(39);
                print("- ");
                print(commandDescrition[j]);
            }
            print("\n");
            print(TAB);
            print("- Possible arguments: ");
            print(commandArgs[i]);
            if(j!= -1){
                 syscall(39);
                print("- Possible arguments: ");
                print(commandArgs[j]);
            }
            print("\n");
}  

int newShell(uint64_t argc, char *argv[], char *command, int is_foregorund){
    char * aux[] = {};
    char * number[4];
    return usr_create_process((void*)shell,1,aux,"shell_son", PRIORITY_NORMAL,1);
}


void phylosophers(uint64_t argc, char *argv[], char *command, int is_foregorund) {
    if (argc > 0) {
        argsError(argc, argv);
        return;
    }
    usr_create_process((void*)phylo_main, argc, argv, "phylosophers", PRIORITY_LOW, is_foregorund);
}