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
void putRectangle(int x, int y, uint32_t color);