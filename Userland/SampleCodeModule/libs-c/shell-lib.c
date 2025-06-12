#include <shell-lib.h>
#include <stdint.h>
#include "string-lib.h"
#include "test.h"
#include "c-lib.h"
#include "shell.h"
#include "command-lib.h"
#include "phylo.h"




int help(uint64_t argc, char *argv[], char* command, int is_foregorund){
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
        return 0;
    }
    else{
        argsError(argc,argv);
        return -1;
    }
}

int whatTime(uint64_t argc, char *argv[], char* command, int is_foregorund){
    if(argc != 1){
        argsError(argc,argv);
        return -1;
    }
    if(strCompare(argv[0], "ARG")){
        printTime(1);
    }
    else if(strCompare(argv[0], "UTC")){
        printTime(0);
    }
    else{
        argsError(argc,argv);
        return -1;
    }
    print(TAB);
    return 0;
}

int zoom(uint64_t argc, char *argv[], char* command, int is_foregorund){
    if(argc != 1){
        argsError(argc,argv);
        return -1;
    }
    if(strCompare(argv[0],"IN")){
        syscall(5, STDOUT);
    }
    if(strCompare(argv[0],"OUT")){
        syscall(5, STDOUT);
    }
    print("\n");
    return 0;
}

int clear(uint64_t argc, char *argv[], char* command, int is_foregorund){
    if(argc > 0){
        argsError(argc,argv);
        return -1;
    }
    syscall(6, STDOUT);
    return 0;
}

int printReg(uint64_t argc, char *argv[], char* command, int is_foregorund){
    if(argc > 0){
        argsError(argc,argv);
        return -1;
    }
    syscall(1, STDOUT);
    return 0;
}

int divCero(uint64_t argc, char *argv[], char* command, int is_foregorund){
    if(argc > 0){
        argsError(argc,argv);
        return -1;
    }
    int aux = 0/0;
    return 0;
}
int invalidOperation(uint64_t argc, char *argv[], char* command, int is_foregorund){
    if(argc > 0){
        argsError(argc,argv);
        return -1;
    }
    invalidOp();
    return 0;
}

int test(uint64_t argc, char *argv[], char* command, int is_foregorund){
    if(argc == 1){
    if(strCompare(argv[0],"MM")){

        return usr_create_process((void*)test_mm, argc-1,argv+1, "memory test", PRIORITY_LOW, is_foregorund);
    }
    else if(strCompare(argv[0],"Prio")){

        return usr_create_process((void*)test_prio,argc,argv, "priority test", PRIORITY_LOW,is_foregorund);
    }
    else if(strCompare(argv[0],"Proc")){
        return usr_create_process((void*)test_processes,argc -1,argv+1, "processes test", PRIORITY_LOW, is_foregorund);

    }
} 
    else if(strCompare(argv[0],"Sync") && argc == 3){
        argc = 2;
        argv[0] = argv[1]; 
        argv[1] = argv[2];
        return usr_create_process((void*)test_sync,argc,argv, "sync test", PRIORITY_LOW, is_foregorund);
    }
    argsError(argc,argv);
    return -1;
}

int block(uint64_t argc, char *argv[], char* command, int is_foregorund){
    if(argc != 1){
        argsError(argc,argv);
        return -1;
    }
    else{
        int pid = strToInt(argv[0]);
        usr_block_process(pid);
        return 0;
    }
}

int unblock(uint64_t argc, char *argv[], char* command, int is_foregorund){
    if(argc != 1){
        argsError(argc,argv);
        return -1;
    }
    else{
        int pid = strToInt(argv[0]);
        usr_unblock_process(pid);
        return 0;
    }
}

int nice(uint64_t argc, char *argv[], char* command, int is_foregorund){
    if(argc != 2){
        argsError(argc,argv);
        return -1;
    }
    else{
        int pid = strToInt(argv[0]);
        int newPrio = strToInt(argv[1]);
        usr_nice(pid, newPrio);
        return 0;
    }

}

int yeild(uint64_t argc, char *argv[], char* command, int is_foregorund){
    if(argc > 0){
        argsError(argc,argv);
        return -1;
    }
    else{
        usr_yield();
        return 0;
    }
}

int kill(uint64_t argc, char *argv[], char* command, int is_foregorund){
    if(argc != 1){
        argsError(argc,argv);
        return -1;
    }
    else{
        print(argv[0]);
        int pid = strToInt(argv[0]);
        if(pid >0){
            int notFoundProcess = usr_kill(pid);
            print(TAB);
        if(notFoundProcess) {
            printErr("kill: process not found");
            return -1;
        }
        }
        else{
            printErr("Invalid PID");
            return -1;
        }
        print("\n");
        return 0;
    }
}

int ps(uint64_t argc, char *argv[], char* command, int is_foregorund){
    if(argc > 0){
        argsError(argc,argv);
        return -1;
    }
    else{
        syscall(20);
        return 0;
    }

}

int mem(uint64_t argc, char *argv[], char* command, int is_foregorund){
    if(argc > 0){
        argsError(argc,argv);
        return -1;
    }
    else{
        return usr_create_process((void*)mem_command, argc, argv, "mem", PRIORITY_LOW, is_foregorund);
    }
}

int loop(uint64_t argc, char *argv[], char* command, int is_foregorund){
   if(argc > 0) {
        argsError(argc,argv);
        return -1;
    } else {
        return usr_create_process((void*)loop_command, argc, argv, "loop", PRIORITY_LOW, is_foregorund);
    }
   }

int cat(uint64_t argc, char *argv[], char* command, int is_foreground) {
    if(argc > 0){
        argsError(argc,argv);
        return -1;
    } else 
    return usr_create_process((void*)cat_command, argc, argv, "cat", PRIORITY_LOW, is_foreground);
}

int wc(uint64_t argc, char *argv[], char* command, int is_foreground) {
     if(argc > 0){
        argsError(argc,argv);
        return -1;
    } else 
    return usr_create_process((void*)wc_command, argc, argv, "wc", PRIORITY_LOW, is_foreground);
}

int filter(uint64_t argc, char *argv[], char* command, int is_foreground) {
     if(argc > 0){
        argsError(argc,argv);
        return -1;
    } else 
    return usr_create_process((void*)filter_command, argc, argv, "filter", PRIORITY_LOW, is_foreground);
}

int invalid(uint64_t argc, char *argv[], char* command, int is_foregorund){
    print(TAB);
    printErr("command: [");
    printErr(command);
    printErr("] not found.\n");
    return 0;
}

int argsError(uint64_t argc, char *argv[]){
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
    return 0;
}

int commandInfo(int i){
    (void) commandDescrition;
    (void) commandArgs;
            i--;
            print(TAB);
            print("- This is the [");
            print(commands[i]);
            print("] command.");
            print("\n");
            print(TAB);
            print("- ");
            print(commandDescrition[i]);
            print("\n");
            print(TAB);
            print("- Possible arguments: ");
            print(commandArgs[i]);
            print("\n");
            return 0;
}  

int newShell(uint64_t argc, char *argv[], char *command, int is_foregorund){
    char * aux[] = {NULL};
    usr_create_process((void*)shell,1,aux,"shell_son", PRIORITY_NORMAL,1);
    return -1;
}


int phylosophers(uint64_t argc, char *argv[], char *command, int is_foregorund) {
    if (argc > 0) {
        argsError(argc, argv);
        return -1;
    }
    return usr_create_process((void*)phylo_main, argc, argv, "phylosophers", PRIORITY_LOW, is_foregorund);
}