#include<stdint.h>
void putPixel(uint32_t hexColor, uint64_t x, uint64_t y);
void imprimirVideo(char * palabra, int size);
void charVideo(int num,char isEndLine);
void nlVideo();
void deleteVideo();
void movVideo(int direc);
void printHexaVideo(uint64_t value);
//uint8_t ** new_font;