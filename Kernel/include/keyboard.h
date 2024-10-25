#include <stdint.h>
#include <lib.h>

#define BUFFER_SIZE 1024

static uint8_t isReleased(uint8_t key);
static uint8_t isPressed(uint8_t key);
char toLetter(int hexa);
uint64_t buffer_has_next();
uint64_t current();
static uint8_t releasedKeyToPressedMask(uint8_t key);
void keyboardHandler();
void loadBuffer(int key); //carga la tecla presionada en el buffer que contiene las letras a ser impresas (y recibe la tecla)
int isBufferEmpty();
char getBuffer();