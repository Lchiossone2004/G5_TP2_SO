#include <shell-lib.h>
#include <stdint.h>
#include "string-lib.h"
#include "test.h"
#include "c-lib.h"
#include "shell.h"
#include "command-lib.h"
#define EOF -1

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

void test(uint64_t argc, char *argv[], char* command, int is_foregorund){
    if(argc == 0){
        argsError(argc,argv);
    }
    if(strCompare(argv[0],"MM")){
        void * ptr = usr_malloc(12);
        usr_free(ptr);
        return;
        //usr_create_process((void*)test_mm, argc-1,argv+1, "memory test", PRIORITY_LOW, is_foregorund);
    }
    else if(strCompare(argv[0],"Prio")){

        usr_create_process((void*)test_prio,argc,argv, "priority test", PRIORITY_LOW,is_foregorund);
    }
    else if(strCompare(argv[0],"Proc")){
        usr_create_process((void*)test_processes,argc -1,argv+1, "processes test", PRIORITY_LOW, is_foregorund);

    }
    else if(strCompare(argv[0],"Sync")){
        argc = 2;
        argv[0] = argv[1];
        argv[1] = argv[2];
        usr_create_process((void*)test_sync,argc,argv, "sync test", PRIORITY_LOW, is_foregorund);
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
        syscall(21);
    }
}

void mem(uint64_t argc, char *argv[], char* command, int is_foregorund){
    if(argc > 0){
        argsError(argc,argv);
    }
    else{
        usr_create_process((void*)mem_command, argc, argv, "mem", PRIORITY_LOW, is_foregorund);
    }
}

void loop(uint64_t argc, char *argv[], char* command, int is_foregorund){
   if(argc > 0) {
        argsError(argc,argv);
    } else {
        usr_create_process((void*)loop_command, argc, argv, "loop", PRIORITY_LOW, is_foregorund);
    }
   }

void cat(uint64_t argc, char *argv[], char* command, int is_foreground) {
    if(argc != 1){
        argsError(argc,argv);
    } else 
    usr_create_process((void*)cat_command, argc, argv, "cat", PRIORITY_LOW, is_foreground);
}

void wc(uint64_t argc, char *argv[], char* command, int is_foreground) {
     if(argc != 1){
        argsError(argc,argv);
    } else 
    usr_create_process((void*)wc_command, argc, argv, "wc", PRIORITY_LOW, is_foreground);
    }

void filter(uint64_t argc, char *argv[], char* command, int is_foreground) {
     if(argc != 1){
        argsError(argc,argv);
    } else 
    usr_create_process((void*)filter_command, argc, argv, "filter", PRIORITY_LOW, is_foreground);
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





void pipeCommand(uint64_t argc, char *argv[], char *command, int is_foregorund) {
    if(argc != 3){
        argsError(argc, argv);
        return;
    }
    int pipe_pos = 0;
    for(int i = 0; i < argc; i++){
        if(strCompare(argv[i],"|")){
            pipe_pos = i;
        }
    }
    int new_pipe[2];
    usr_open_pipe(&new_pipe[0], &new_pipe[1]);  //int* fd_read, int*fd_write

    if (pipe_pos != 1) {
        printErr("Pipe symbol '|' must be in middle.\n");
        return;
    }
    return; 
}

void commandInfo(int i,int j){
    (void) commandDescrition;   //Bilardeada para sacar el warning
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

void newShell(uint64_t argc, char *argv[], char *command, int is_foregorund){
    char * aux[] = {};
    char * number[4];
    usr_create_process((void*)shell,1,aux,"shell'", PRIORITY_HIGH,1);
    usr_wait_children();
    shellCounter--;

}