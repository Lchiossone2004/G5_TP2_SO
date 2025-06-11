#include <stdint.h>
#include "c-lib.h"
#include "test_util.h"
#include "command-lib.h"

#define STDOUT 1
#define STDIN 0
#define EOF '\0'

void wc_command() {
    char buffer[128];
    int line_count = 0;
    char c;
    int n;
    int pos = 0;
    char num[10];
    while ((n = read(STDIN, &c, 1)) > 0) {
        char temp[2] = {c, '\0'}; 
        print(temp);
        buffer[pos++] = c;
        if (c == '\n') {
            line_count++;
        }
        if(c == EOF || pos >= sizeof(buffer) - 1) {
            line_count++;
            print("\n");
            break;
        }
    }
    intToString(line_count, num, sizeof(num));
    print(num);
    print(" lines");
    print("\n");
    printShell();
    return;
}

void cat_command() {
    char buffer[128];
    int pos = 0;
    char c;
    int n;

    while ((n = read(STDIN, &c, 1)) > 0) {
        char temp[2] = {c, '\0'}; 
        print(temp);
        buffer[pos++] = c;

        if (c == '\n' || pos >= (int)sizeof(buffer) - 1 || c == EOF) {
            print("\n");
            buffer[pos] = '\0';  
            print(buffer);
            print("\n");
            printShell();
            return;
        }
    } 
    if (pos > 0) {
        print(buffer);
    }
    printShell();
    return;
}

void filter_command() {
    char buffer[128];
    char c;
    int n;
    int pos = 0;
    
    while ((n = read(STDIN, &c, 1)) > 0) {
        char temp[2] = {c, '\0'}; 
        print(temp);
        buffer[pos++] = c;
        if (c == '\n' || pos >= sizeof(buffer) - 1 || c == EOF) 
        break;
    }
    buffer[pos] = '\0';
    int newLen = removeRepeated(buffer);
    for(int i = 0; i < newLen; i++) {
        if (isVowel(buffer[i])) {
            char temp[2] = {buffer[i], '\0'}; 
            print(temp);
        }
    }
    printShell();
    return;
}

void loop_command() {
    int pid = syscall(15);
    int len = 124;
    char buffer[len];
    intToString(pid,buffer,len);
    while(1){
    //     print(buffer);
    //     print("\n");
    //print("These Aren't the Droids You're Looking For");
    //     print("\n");
    //     sleep(10);
    //for(int i = 0 ; i < 10000000; i ++){}
    }
}
void mem_command() {
    print_usr_mem_info();
    return;
}
void printShell(){
    print("\n");
    print("shell: >");
    return;
}