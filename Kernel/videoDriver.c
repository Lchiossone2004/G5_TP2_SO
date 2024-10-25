#include<stdint.h>
#include <videoDriver.h>
#include <font.h>
#include <naiveConsole.h>

#define MOV_X 8  //Lo que ocupa en x de un char 
#define MOV_Y 16 //Lo que ocupa en y de un char
#define BORDER_X 1016 //maximo ancho (en pixeles) de la pantalla
#define BORDER_Y 768 //maximo largo (en pixeles) de la pantalla
#define HEIGHT 8
#define WIDTH 128
#define MAX_ZOOM 10
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
static int zoom = 0; //zoom inicial
static int x = 0;
static int y = 0;
static int aux = 0;
static int maxX = 0;
static int maxY = 0;
static int num_columns = BORDER_X / MOV_X; 
static int num_rows = BORDER_Y / MOV_Y; 


void putPixel(uint32_t hexColor, uint64_t x, uint64_t y) {
    uint8_t * framebuffer = (uint8_t *) VBE_mode_info->framebuffer;
    uint64_t offset = (x * ((VBE_mode_info->bpp)/8)) + (y * VBE_mode_info->pitch);
    framebuffer[offset]     =  (hexColor) & 0xFF;
    framebuffer[offset+1]   =  (hexColor >> 8) & 0xFF; 
    framebuffer[offset+2]   =  (hexColor >> 16) & 0xFF;
}

void imprimirVideo(char * palabra, int size){
		for(int i = 0; i< size; i++){
			y = aux;	
			charVideo(palabra[i],zoom);
		}
}

void charVideo(int num, char isEndLine){
	if(x<=BORDER_X && y < BORDER_Y){
	int set;
	char *bitmap;
	char scale=zoom+1;
	bitmap = font[num];//aca habria que cambiarlo por new_font y adaptar todo porque ahora cada letra ocupa 2 *zoom
	for(int i = 0; i <8; i++){
		for(int j = 0; j<8; j++){
			set = bitmap[i] & 1 << j;
			if(set){
				for(int dy=0;dy<scale;dy++){
					for (int dx = 0; dx < scale; dx++) {
                            putPixel(0xFFFFFF, x + j * scale + dx, y + i * scale + dy);
                        }
				}
			}
			else{
				for (int dy = 0; dy < scale; dy++) {
                    for (int dx = 0; dx < scale; dx++) {
                        putPixel(0x0, x + j * scale + dx, y + i * scale + dy);
                    }
                }
			}
		}
		y += scale;
	}
	x += MOV_X*scale;
	if(x>maxX){
		maxX = x;
	}
	if(x >= VBE_mode_info->width && isEndLine){ //Estaria bueno sacar este flag, que es para que borre correctamente 
		aux += MOV_Y*scale;
		x = 0;
		maxX = 0;
	}
	y = aux;
	}
}

void nlVideo(){ //Hace un salto de linea
	if(y < BORDER_Y -16){
	aux += MOV_Y;
	y += MOV_Y;
	x = 0;
	}
}

void deleteVideo(){ //Borra caracteres
	if(!(y == 0 && x <= 0)){
	if(x >= MOV_X){
	x -= MOV_X;
	charVideo(0,1);
	x -= MOV_X;
	}
	else{
	x = VBE_mode_info->width -8;
	y -= MOV_Y;
	aux -= MOV_Y;
	charVideo(0,0);
	x -= MOV_X;
	}
	}
}

void movVideo(int direction){
	x += (direction*MOV_X);
	if(x < 10){
	x = VBE_mode_info->width -8;
	y -= MOV_Y;
	aux -= MOV_Y;
	}
	if(x > VBE_mode_info->width - 10){
		aux += MOV_Y;
		x = 0;
		y = aux;
	}
}

void printTimeVideo(char hour, char min, char sec) {
	char * toPrint = "hh:mm:ss";
	char aux = hour >> 4;
	toPrint[0] = aux + '0';
	toPrint[1] = (hour&0xF) + '0'; //guardo los 4 bits menos significativos y el resto lo pongo en 0
	aux = min >> 4;
	toPrint[3] = aux + '0';
	toPrint[4] = (min&0xF) + '0';
	aux = sec >> 4;
	toPrint[6] = aux + '0';
	toPrint[7] = (sec&0xF) + '0';
	imprimirVideo(toPrint, sizeof(toPrint));
}
void printHexaVideo(uint64_t value){
	char* buffer;
	uint32_t digits=uintToBase(value, buffer, 16);
	imprimirVideo(buffer, digits);
}


void  zoomIN() {
    zoom=1;
}
void zoomOUT() {
	zoom=0;
}
/*
void expand(uint8_t ** newBitMap) {
        zoom++; //siempre quiero que se agrande hasta zoom+1
        for(int i = 0; i < 128 * (zoom+1); i+zoom) { //para cada fila en newBitMap
        for(int j = 0; j < 128; j++) { //para cada fila en font
            expandirFila(font[j], newBitMap[i], zoom); //expando la fila de font en la fila de newBitMap
            for(int k = 1; k < zoom*2; k++) { 
            newBitMap[i + k] = newBitMap[i]; //duplico la columna n veces
            }
        }
    }
}

 void reduce(uint8_t ** newBitMap) {
   if (zoom <= 1) {
    zoom = 0;
    copy(newBitMap); 
    return;
   }
   zoom-=2; //disminuyo la zoom en 2 para que el expand trabaje con zoomOrg -1
   expand(newBitMap);
   //en el mismo expand se agranda zoom en  una unidad entonces queda en -1
}


 void expandirFila(uint8_t * original, uint16_t *expandido, int n) {
    int idx = 0;
    // recorro cada byte en la fila original
    for (int i = 0; i < 8; i++) {
        uint8_t byte = original[i];
        // recorro los bits del byte
        for (int bitPos = 7; bitPos >= 0; bitPos--) {
            uint16_t bit = (byte >> bitPos) & 1; // extraigo el bit
            for (int j = 0; j < n; j++) {
                expandido[idx++] = bit; // lo duplico n veces y lo guardo en idx
            }
        }
    }
}
int outOfBoundsZoom() {
    if(zoom < 0 || zoom > MAX_ZOOM ) {
        return 1;
    }
    return 0;
}




void copyBitMap(uint8_t newBitMap[WIDTH*(zoom+2)][HEIGHT*(zoom+2)]) {
    for (int i = 0; i < WIDTH * (zoom + 2); i++) {
        for (int j = 0; j < HEIGHT * (zoom + 2); j++) {
            new_font[zoom][i][j] = newBitMap[i][j];
        }
    }
}
 void copy() {
    copyBitMap(font);
  }

*/
//newBitMap tiene que tener 8 * (flag+2) x 128*(flag+2)
