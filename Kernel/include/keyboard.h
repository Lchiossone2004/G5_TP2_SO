#include <stdint.h>
#include <lib.h>

#define BUFF_SIZE 1024
#define EOF -1

void init_keyboard();
int read_keyboard(char* letter,int count);
char toLetter(uint8_t hexa);
void updateKeyboardStatus(uint8_t key, uint8_t isp);
void loadBuffer(uint8_t key);