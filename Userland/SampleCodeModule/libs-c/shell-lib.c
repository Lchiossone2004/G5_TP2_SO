#include <shell-lib.h>
#include "string-lib.h"
#include "test.h"

extern void invalidOp();

void help(uint64_t argc, char *argv[], char* command){
    if(argc == 0){
        print("     Here is a list of the commands:");
        for (int i = 0; i < NUMBER_OF_COMMANDS; i++)
        {
            nlPrint();
            print("     #) ");
            print(commands[i]);
        }
        nlPrint();
        print("To know more about any command type [command] -info");
        nlPrint();
    }
    else if(argc == 1 && strCompare(argv[0], "all")){
        clear(0,NULL,NULL);             //Limpio por que sino no alcanza la pantalla
        for(int i = 0; i < NUMBER_OF_COMMANDS; i++){
            commandInfo(i);
            nlPrint();
        }
    }
    else{
        argsError(argc,argv);
    }
}

void whatTime(uint64_t argc, char *argv[], char* command){
    if(argc != 1){
        argsError(argc,argv);
        return;
    }
    if(strCompare(argv[0], "-info")){
        commandInfo(command);
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
    nlPrint();
}

void zoom(uint64_t argc, char *argv[], char* command){
    if(argc != 1){
        argsError(argc,argv);
    }
    if(strCompare(argv[0],"IN")){
        syscall(6, STDOUT);
    }
    if(strCompare(argv[0],"OUT")){
        syscall(7, STDOUT);
    }
    nlPrint();
}

void clear(uint64_t argc, char *argv[], char* command){
    if(argc > 0){
        argsError(argc,argv);
    }
    syscall(9, STDOUT);
    nlPrint();
}

void printReg(uint64_t argc, char *argv[], char* command){
    if(argc > 0){
        argsError(argc,argv);
    }
    syscall(1, STDOUT);
     nlPrint();
}

void divCero(uint64_t argc, char *argv[], char* command){
    if(argc > 0){
        argsError(argc,argv);
    }
    int aux = 0/0;
    nlPrint();
}
void invalidOperation(uint64_t argc, char *argv[], char* command){
    if(argc > 0){
        argsError(argc,argv);
    }
    invalidOp();
    nlPrint();
}

void test(uint64_t argc, char *argv[], char* command){
    if(argc != 1){
        argsError(argc,argv);
    }
    if(strCompare(argv[0],"MM")){
        usr_create_process((void*)test_mm, argc,argv, "memory test", 3, 0);
    }
    if(strCompare(argv[0],"Prio")){
        usr_create_process((void*)test_prio,argc,argv, "priority test", 3,0);
    }
    if(strCompare(argv[0],"Processes")){
        usr_create_process((void*)test_processes,argc,argv, "processes test", 3, 0);
    }
    if(strCompare(argv[0],"Sync")){
        usr_create_process((void*)test_processes,argc,argv, "sync test", 3, 0);
    }
    nlPrint();
}

void block(uint64_t argc, char *argv[], char* command){
    if(argc != 1){
        argsError(argc,argv);
    }
    else{
        int pid = strToInt(argv[0]);
        usr_block_process(pid);
    }
}

void unblock(uint64_t argc, char *argv[], char* command){
    if(argc != 1){
        argsError(argc,argv);
    }
    else{
        int pid = strToInt(argv[0]);
        usr_unblock_process(pid);
    }
}

void nice(uint64_t argc, char *argv[], char* command){
    if(argc != 2){
        argsError(argc,argv);
    }
    else{
        int pid = strToInt(argv[0]);
        int newPrio = strToInt(argv[1]);
        usr_nice(pid, newPrio);
    }

}

void yeild(uint64_t argc, char *argv[], char* command){
    if(argc > 0){
        argsError(argc,argv);
    }
    else{
        usr_yeild();
    }
}

void kill(uint64_t argc, char *argv[], char* command){
    if(argc != 1){
        argsError(argc,argv);
    }
    else{
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
        nlPrint();
    }
}

void ps(uint64_t argc, char *argv[], char* command){
    if(argc > 0){
        argsError(argc,argv);
    }
    else{
        syscall(29);
        nlPrint();
    }
}

void mem(uint64_t argc, char *argv[], char* command){
        if(argc > 0){
        argsError(argc,argv);
    }
    else{
        print_usr_mem_info();
    }
}

void invalid(uint64_t argc, char *argv[], char* command){
    print(TAB);
    printErr("command: [");
    printErr(command);
    printErr("] not found.");
    nlPrint();
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
    nlPrint();
}

void commandInfo(int commandNum){
    print(TAB);
    print("- This is the [");
    print(commands[commandNum]);
    print("] command.");
    nlPrint();
    print(TAB);
    print("- ");
    print(commandDescrition[commandNum]);
    nlPrint();
    print(TAB);
    print("- Possible arguments: ");
    print(commandArgs[commandNum]);
    nlPrint();
}  