#include<stdint.h>
void putPixel(uint32_t hexColor, uint64_t x, uint64_t y);
void imprimirVideo(char * palabra, int size, uint32_t color);
void charVideo(int num,char isEndLine, uint32_t color);
void nlVideo();
void deleteVideo();
void movVideo(int direc);
void printHexaVideo(uint64_t value);
void zoomIN();
void zoomOUT();
void snakeCanvas(uint32_t firstColor, uint32_t secondColor);
void printMatriz();