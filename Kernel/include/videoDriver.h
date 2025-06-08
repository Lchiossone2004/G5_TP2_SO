#include<stdint.h>

typedef struct{
    char num;
    uint32_t color;
}word;
void putPixel(uint32_t hexColor, uint64_t x, uint64_t y);
void imprimirVideo(char * palabra, int size, uint32_t color);
void charVideo(char num,char isEndLine, uint32_t color);
void nlVideo();
void deleteVideo();
void printHexaVideo(uint64_t value);
void zoomIN();
void zoomOUT();
void videoClear();
void rePrint();
uint_fast64_t goMiddle();