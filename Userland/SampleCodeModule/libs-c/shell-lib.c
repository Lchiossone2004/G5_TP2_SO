#include <shell-lib.h>
#include "string-lib.h"
#include <stdint.h>
#include "test.h"
#include "c-lib.h"
#include "phylo.h"

#define EOF -1
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

void phylo(uint64_t argc, char *argv[], char* command, int is_foregorund){
    usr_create_process((void*)phylo_main,argc,argv, "phylosofers", PRIORITY_LOW,is_foregorund);
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
        print_usr_mem_info();
    }
}

void loop(uint64_t argc, char *argv[], char* command, int is_foregorund){
    int pid = syscall(16);
    int len = 124;
    char buffer[len];
    intToString(pid,buffer,len);
    while(1){
        print(buffer);
        print("\n");
        print("These Aren't the Droids Your Looking For");
        print("\n");
        for(int i = 0; i <100000000; i++);
    }
}
void cat(uint64_t argc, char *argv[], char* command, int is_foreground) {
    char buffer[128];
    int bytesRead;

    while((bytesRead = read(buffer, STDIN, sizeof(buffer))) > 0){
        write(buffer, STDOUT, bytesRead);
    }

    print("\n");
}

void wc(uint64_t argc, char *argv[], char* command, int is_foreground) {
    char buffer[128];
    int line_count = 0;
    int bytesRead;

    while ((bytesRead = read(buffer, STDIN, sizeof(buffer))) > 0) {
        line_count++;
    }
    char result[16];
    intToString(line_count, result, sizeof(result));
    print(result);
    print("\n");
}
void filter(uint64_t argc, char *argv[], char* command, int is_foreground) {
    char buffer[128];
    int bytesRead;

    while ((bytesRead = read(buffer, STDIN, sizeof(buffer))) > 0) {
        for (int i = 0; i < bytesRead; i++) {
            char c = buffer[i];
            if (c == '\n') 
            break;
            if (isVowel(c)) {
            char temp[2] = {c, '\0'}; 
            print(temp);
            }
           
        }
         
        print("\n");  
    }
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




static void remove_blanks(char *s) {
    size_t len = strlen(s);
    if (len > 0) {
        char *end = s + len - 1;
        while (len > 0 && isBlank(*end)) {
            *end = '\0';
            end--;
            len--;
        }
    }
    char *src = s;
    while (*src && isBlank(*src)) {
        src++;
    }
    if (src != s) {
        char *dst = s;
        while (*src) {
            *dst++ = *src++;
        }
        *dst = '\0';
    }
}


void pipeCommand(uint64_t argc, char *argv[], char *command) {
    // if (argc != 1) {
    //     argsError(argc, argv);
    //     return;
    // }
    int pipe_fd[2] = {NULL};
    char *buffer = "HOLA BUENAS TESTEANDO";
    usr_open_pipe(&pipe_fd[0], &pipe_fd[1]);      // 0= READ 1= WRITE
    write(buffer,pipe_fd[1],strSize(buffer));
    char buffer2[25];
    read(buffer2,pipe_fd[0],strSize(buffer));
    write(buffer2,STDOUT,strSize(buffer));
    print("\n");
    // char *full_command = argv[0];
    // char *pipe_pos = strchr(full_command, '|');

    // if (pipe_pos == NULL) {
    //     printErr("Pipe symbol '|' not found.\n");
    //     return;
    // }
    // *pipe_pos = '\0';
    // char *cmd1 = full_command;
    // char *cmd2 = pipe_pos + 1; 
    // remove_blanks(cmd1);
    // remove_blanks(cmd2);

    // if (!cmd1 || !cmd2) {
    //     printErr("Invalid commands.\n");
    //     return;
    // }

   
    // int pipefd[2];  
    // int pipe_index = syscall(40, cmd1, 0); 
    // if (pipe_index < 0) {
    //     printErr("Failed to create pipe.\n");
    //     return;
    // }

    // if (syscall(41, cmd2, 0, pipefd) < 0) { 
    //     printErr("Failed to open pipe.\n");
    //     return;
    // }

   
    // syscall(4, pipefd[1], cmd1, 0);  
    // syscall(3, pipefd[0], cmd2, 0);  

  
    // syscall(32, 0);  
    // syscall(32, 0);  
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