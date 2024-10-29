#include<stdint.h>
#include <videoDriver.h>
#include <font.h>
#include <naiveConsole.h>
#include <lib.h>

#define MOV_X 8  //Lo que ocupa en x de un char 
#define MOV_Y 16 //Lo que ocupa en y de un char
#define BORDER_X 1024 //maximo ancho (en pixeles) de la pantalla
#define BORDER_Y 768 //maximo largo (en pixeles) de la pantalla
#define HEIGHT 8
#define WIDTH 128
#define MAX_ZOOM 10
#define BLANCO 0xFFFFFF
#define REC_ANCHO 32
#define REC_LARGO 32
#define REC_X_FIL BORDER_X / REC_ANCHO
#define REC_X_COL BORDER_Y / REC_LARGO
#define MAX_COLLS_IN_SHELL 127
#define MAX_FIL_IN_SHELL 48
#define COLOR_1 0x00b6e2f0
#define COLOR_2 0x009de0f5


struct vbe_mode_info_structure {
	uint16_t attributes;		// deprecated, only bit 7 should be of interest to you, and it indicates the mode supports a linear frame buffer.
	uint8_t window_a;			// deprecated
	uint8_t window_b;			// deprecated
	uint16_t granularity;		// deprecated; used while calculating bank numbers
	uint16_t window_size;
	uint16_t segment_a;
	uint16_t segment_b;
	uint32_t win_func_ptr;		// deprecated; used to switch banks from protected mode without returning to real mode
	uint16_t pitch;			// number of bytes per horizontal line
	uint16_t width;			// width in pixels
	uint16_t height;			// height in pixels
	uint8_t w_char;			// unused...
	uint8_t y_char;			// ...
	uint8_t planes;
	uint8_t bpp;			// bits per pixel in this mode
	uint8_t banks;			// deprecated; total number of banks in this mode
	uint8_t memory_model;
	uint8_t bank_size;		// deprecated; size of a bank, almost always 64 KB but may be 16 KB...
	uint8_t image_pages;
	uint8_t reserved0;
 
	uint8_t red_mask;
	uint8_t red_position;
	uint8_t green_mask;
	uint8_t green_position;
	uint8_t blue_mask;
	uint8_t blue_position;
	uint8_t reserved_mask;
	uint8_t reserved_position;
	uint8_t direct_color_attributes;
 
	uint32_t framebuffer;		// physical address of the linear frame buffer; write here to draw to the screen
	uint32_t off_screen_mem_off;
	uint16_t off_screen_mem_size;	// size of memory in the framebuffer but not being displayed on the screen
	uint8_t reserved1[206];
} __attribute__ ((packed));

typedef struct vbe_mode_info_structure * VBEInfoPtr;

VBEInfoPtr VBE_mode_info = (VBEInfoPtr) 0x0000000000005C00;

uint8_t new_font[MAX_ZOOM + 1][WIDTH * (MAX_ZOOM + 2)][HEIGHT * (MAX_ZOOM + 2)];
static int zoom = 1; //zoom inicial
static int x = 0;
static int y = 0;
static int aux = 0;
static int pos_circle_x;
static int pos_circle_y;
static word matrix[BORDER_Y/MOV_Y][BORDER_X/MOV_X] = {0};

void putPixel(uint32_t hexColor, uint64_t x, uint64_t y) {
    uint8_t * framebuffer = (uint8_t *) VBE_mode_info->framebuffer;
    uint64_t offset = (x * ((VBE_mode_info->bpp)/8)) + (y * VBE_mode_info->pitch);
    framebuffer[offset]     =  (hexColor) & 0xFF;
    framebuffer[offset+1]   =  (hexColor >> 8) & 0xFF; 
    framebuffer[offset+2]   =  (hexColor >> 16) & 0xFF;
}

void imprimirVideo(char * palabra, int size, uint32_t color){
		for(int i = 0; i< size; i++){
			y = aux;
			word vol;
			vol.num = palabra[i];
			vol.color = color;
			matrix[(y/zoom)/MOV_Y][(x/zoom)/MOV_X] = vol;
			charVideo(palabra[i],1,color); //Che ojo que el 1 ese es importante para cunado se termina la oracion, no lo cambien por el parametro zoom
		}
}
void charVideo(char num, char isEndLine, uint32_t color){
	if(x<=BORDER_X && y < BORDER_Y){
	int set;
	char *bitmap;
	bitmap = font[num];//aca habria que cambiarlo por new_font y adaptar todo porque ahora cada letra ocupa 2 *zoom
	for(int i = 0; i <8; i++){
		for(int j = 0; j<8; j++){
			set = bitmap[i] & 1 << j;
			if(set){
				for(int dy=0;dy<zoom;dy++){
					for (int dx = 0; dx < zoom; dx++) {
                            putPixel(color, x + j * zoom + dx, y + i * zoom + dy);
                        }
				}
			}
			else{
				for (int dy = 0; dy < zoom; dy++) {
                    for (int dx = 0; dx < zoom; dx++) {
                        putPixel(0x0, x + j * zoom + dx, y + i * zoom + dy);
                    }
                }
			}
		}
		y += zoom;
	}
	x += MOV_X*zoom;
	if(x >= BORDER_X && isEndLine){ //Estaria bueno sacar este flag, que es para que borre correctamente 
		aux += MOV_Y*zoom;
		x = 0;
	}
	y = aux;
	}
	if(x >= BORDER_X - 8 && y >= BORDER_Y -16){
		videoClear();
	}
}

void nlVideo(){ //Hace un salto de linea
	if(y < BORDER_Y -(16*zoom)){
	aux += MOV_Y*zoom;
	y += MOV_Y*zoom;
	x = 0;
	}
	else{
		videoClear();
	}
}

void deleteVideo(){ //Borra caracteres
	word vol;
	vol.num = 0;
	vol.color = BLANCO;
	if(!(y == 0 && x <= 0)){
		if(x >= MOV_X*zoom){
			x -= MOV_X*zoom;
			matrix[(y/zoom)/MOV_Y][(x/zoom)/MOV_X] = vol;
			charVideo(0,1,BLANCO);
			x -= MOV_X*zoom;
	}
	else{
	x = VBE_mode_info->width -MOV_X*zoom;
	y -= MOV_Y*zoom;
	aux -= MOV_Y*zoom;
	matrix[(y/zoom)/MOV_Y][(x/zoom)/MOV_X] = vol;
	charVideo(0,0,BLANCO);
	x -= MOV_X*zoom;
	}
	}
}

void printHexaVideo(uint64_t value){
	char buffer[20];
	uint32_t digits=uintToBase(value, buffer, 16);
	imprimirVideo(buffer, digits,BLANCO);
}

void rePrint(){ // 1 if is zoomIN 0 if zoomOUT
	clearScrenn();
	for(int i = 0; i<(BORDER_Y/MOV_Y)/zoom; i++){
		for(int j= 0; j<(BORDER_X/MOV_X)/zoom; j++){
				charVideo(matrix[i][j].num,1, matrix[i][j].color);
		}
		if(matrix[i+1][0].num == 0){
			break;
		}
	}
	return;
}

void videoClear(){ //funcion que devuelve la pantalla a su estado "incial"
	clearScrenn();
	for(int i = 0; i<(BORDER_Y/MOV_Y); i++){
		for(int j= 0; j<(BORDER_X/MOV_X ); j++){
			matrix[i][j].num = 0;
		}
	}
	x = 0;
	y = 0;
	aux = 0;
}

void clearScrenn(){
	for(int i = 0; i<BORDER_Y; i++){
		for(int j = 0; j<BORDER_X;j++){
			putPixel(0x0, j, i);
		}
	}
	x = 0;
	y = 0;
	aux = 0;
}

void  zoomIN() {
	if(zoom <2){
    	zoom++;
	}else{
		return;
	}
}
void zoomOUT() {
	if (zoom>1){
		zoom--;
	}else{
		return;
	}
}

//Funciones para el snake

void putRectangle(int posx, int posy, uint32_t color) {
	for(int i = posx; i < posx+REC_ANCHO; i++) {
		for(int j = posy; j < posy+REC_LARGO; j++) {
			putPixel(color, i, j);
		}
	}
}

void snakeCanvas() {
	int flag = 0;
	for(int i = 0; i < BORDER_X + 1; i += REC_ANCHO) {
		flag = !flag;
		for(int j = 0; j < BORDER_Y; j += REC_LARGO) {
			if(flag) {
				putRectangle(i, j, COLOR_1);
			} else {
				putRectangle(i,j,COLOR_2);
			}
			flag = !flag;
		}
	}
}
void putCircle(int posx, int posy, uint32_t color) {
    int centerX = posx + REC_ANCHO / 2;  
    int centerY = posy + REC_LARGO / 2;  
    int radius = (REC_ANCHO < REC_LARGO ? REC_ANCHO : REC_LARGO) / 4;  
    for (int i = posx; i < posx + REC_ANCHO; i++) {
        for (int j = posy; j < posy + REC_LARGO; j++) {
            int dx = i - centerX;  
            int dy = j - centerY;  
            if (dx * dx + dy * dy <= radius * radius) {  
                putPixel(color, i, j);
            }
        }
    }
}
//imprime un circulo en una pos random (seria la manzana) y guarda la posición
void putRandomCircle() {
    int min = getMins(); 
    int sec = getSec(); 

    int area = REC_X_FIL * REC_X_COL;
    int idx = (min * 60 + sec) % area;
    int col = idx % REC_X_COL;          
    pos_circle_x = col * REC_ANCHO; 
    pos_circle_y = col * REC_LARGO;
	putCircle(pos_circle_x, pos_circle_y, BLANCO);
}

void deleteCircle() {
	if((isPair(pos_circle_x) && isPair(pos_circle_y))||(!isPair(pos_circle_x) && !isPair(pos_circle_y))) {
		putRectangle(pos_circle_x, pos_circle_y, COLOR_1);
	} else {
		putRectangle(pos_circle_x, pos_circle_y, COLOR_2);
	}
}
int isPair(int pos) {
	return pos % 2 == 0;
}
