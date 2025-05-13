#include <stdint.h>
#include <lib.h>

#define BUFFER_SIZE 12


char toLetter(uint8_t hexa);
// uint64_t buffer_has_next();
// uint64_t current();
void keyboardHandler();
void updateKeyboardStatus(uint8_t key, uint8_t isp);
void loadBuffer(uint8_t key); //carga la tecla presionada en el buffer que contiene las letras a ser impresas (y recibe la tecla)
int isBufferEmpty();
char getBuffer();
int getCurr();
uint16_t getFromBuffer(int idx);