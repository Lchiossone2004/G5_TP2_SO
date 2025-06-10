#include <stdint.h>
#include "c-lib.h"
#include "test_util.h"

#define STDOUT 1
#define STDIN 0
#define EOF ((char)-1)

void wc_command(char *fd) {
    int fd_num = (int)satoi(fd);
    char buffer[128];
    int line_count = 0;
    char c;
    int n;
    int pos = 0;
    char num[10];
     while ((n = read(fd_num, &c, 1)) > 0) {
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
        print("\n");
}

void cat_command(char *fd) {
    int fd_num = (int)satoi(fd);
    char buffer[128];
    int pos = 0;
    char c;
    int n;

    while ((n = read(fd_num, &c, 1)) > 0) {
        char temp[2] = {c, '\0'}; 
        print(temp);
        buffer[pos++] = c;

        if (c == '\n' || pos >= (int)sizeof(buffer) - 1 || c == EOF) {
            print("\n");
            buffer[pos] = '\0';  
            print(buffer);
            print("\n");
            return;
        }
    } 
    if (pos > 0) {
        print(buffer);
    }
}

void filter_command(char *fd) {
    int fd_num = (int)satoi(fd);
    char buffer[128];
    char c;
    int n;
    int pos = 0;
    

      while ((n = read(fd_num, &c, 1)) > 0) {
        char temp[2] = {c, '\0'}; 
        print(temp);
        buffer[pos++] = c;
            if (c == '\n' || pos >= sizeof(buffer) - 1 || c == EOF) 
            break;
      }
      for(int i = 0; i < pos; i++) {
            if (isVowel(buffer[i])) {
            char temp[2] = {buffer[i], '\0'}; 
            print(temp);
            }
             print("\n");  
        }
}

void loop_command() {
     int pid = syscall(16);
    int len = 124;
    char buffer[len];
    intToString(pid,buffer,len);
    while(1){
        print(buffer);
        print("\n");
        print("These Aren't the Droids You're Looking For");
        print("\n");
        for(int i = 0; i <100000000; i++);
    }
}
void mem_command() {
    print_usr_mem_info();
}