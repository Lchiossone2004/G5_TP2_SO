#include <stdint.h>
#include "c-lib.h"
#include "test_util.h"
#include "command-lib.h"
#include "memory-lib.h"

#define STDOUT 1
#define STDIN 0
#define EOF '\0'
extern uint64_t syscall(uint64_t rdi, ...);

void wc_command() {
    int line_count = 0;
    char c = '0';
    int n = 1;
    char num[10];
    while ((n = read(STDIN, &c, 1)) > 0 && c != EOF) {
        char temp[2] = {c, '\0'}; 
        print(temp);
        if (c == '\n') {
            line_count++;
        }
       
        
    }
    line_count++;
    print("\n");
    intToString(line_count, num, sizeof(num));
    print(num);
    print(" lines");
    print("\n");
    return;
    
}

void cat_command() {
    char c = '\0';
    int n = 1;

    while ((n = read(STDIN, &c, 1)) > 0) {
        char temp[2] = {c, '\0'}; 
        print(temp);
        if (c == '\n' || c == EOF) {
            char temp[2] = {'\0', '\0'}; 
            write(temp,STDOUT,1);
            return;
        }
    } 
    return;
}

void filter_command() {
    char buffer[10];
    char c;
    int n;
    int pos = 0;
    
    while ((n = read(STDIN, &c, 1)) >= 0) {
         if(n == 0){
            continue;
        }
        char temp[2] = {c, '\0'}; 
        print(temp);
        if (c >= 'A' && c <= 'Z') {
        c += 32; 
        }
        if(isVowel(c)) {
        buffer[pos++] = c;
        removeRepeated(buffer);
        }
        if (c == '\n' || c == EOF)  {
            print("\n");
            break;
        }
      
    }
    buffer[pos] = '\0';
    print(buffer);
    printShell();
    return;
    }

void loop_command() {
    int pid = syscall(15);
    int len = 124;
    char buffer[len];
    intToString(pid,buffer,len);
    while(1){
        print(buffer);
        print("\n");
        print("Hello there.");
        print("\n");
        sleep(10);
    }
}
void mem_command() {
    print_usr_mem_info();
    printShell();
    return;
}

void echo_command(int argc,char* argv[]){
    for(int i = 0; i<argc; i++){
        print(argv[i]);
        print(" ");
    }
    print("\n");
    printShell();
    return;
}
void printShell(){
    print("\n");
    print("shell: >");
    return;
}