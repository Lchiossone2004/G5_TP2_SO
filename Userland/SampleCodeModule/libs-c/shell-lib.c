#include <shell-lib.h>
#include "string-lib.h"
#include "test.h"

extern void invalidOp();

void help(uint64_t argc, char *argv[], char* command)
{
    print("     Here is a list of the commands:");
    for (int i = 0; i < NUMBER_OF_COMMANDS; i++)
    {
        nlPrint();
        print("     #) ");
        print(commands[i]);
    }
    nlPrint();
}

void whatTime(uint64_t argc, char *argv[], char* command){
    if(argc != 1){
        trowError(argc,argv);
        return;
    }
    if(strCompare(argv[0], "ARG")){
        printTime(1);
    }
    else if(strCompare(argv[0], "UTC")){
        printTime(0);
    }
    else{
        trowError(argc,argv);
        return;
    }
    print(TAB);
    nlPrint();
}

void zoom(uint64_t argc, char *argv[], char* command){
    syscall(6, STDOUT);
     nlPrint();
}

void clear(uint64_t argc, char *argv[], char* command)
{
    syscall(9, STDOUT);
    nlPrint();
}

void printReg(uint64_t argc, char *argv[], char* command){
    syscall(1, STDOUT);
     nlPrint();
}

void divCero(uint64_t argc, char *argv[], char* command){
    int aux = 0/0;
    nlPrint();
}
void invalidOperation(uint64_t argc, char *argv[], char* command){
    invalidOp();
    nlPrint();
}

void test(uint64_t argc, char *argv[], char* command){
    print_usr_mem_info();
    nlPrint();
}

void kill(uint64_t argc, char *argv[], char* command){
    char *argStr = command + 5;
    
    int valid = 1;
    for (int i = 0; argStr[i] != 0; i++) {
        if (argStr[i] < '0' || argStr[i] > '9') {
            valid = 0;
            break;
        }
    }

    if (!valid) {
        printErr("kill: invalid PID");
     
    } else {

    int pid = strToInt(argStr);
    int notFoundProcess = syscall(23, pid);
    if(notFoundProcess) {
        printErr("kill: process not found");
    }
    }
    nlPrint();
}

void ps(uint64_t argc, char *argv[], char* command){
    syscall(29);
    nlPrint();
}

void invalid(uint64_t argc, char *argv[], char* command){
    print(TAB);
    printErr("command: [");
    printErr(command);
    printErr("] not found.");
    nlPrint();
}

void trowError(uint64_t argc, char *argv[]){
    print(TAB);
    printErr("Invalid Args:");
    for(int i= 0; i<argc;i++){
            print(" ");
            printErr(argv[i]);
    }
    nlPrint();
}