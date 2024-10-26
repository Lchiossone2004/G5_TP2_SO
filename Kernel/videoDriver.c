#include<stdint.h>
#include <videoDriver.h>
#include <font.h>
#include <naiveConsole.h>
#include <lib.h>

#define MOV_X 8  //Lo que ocupa en x de un char 
#define MOV_Y 16 //Lo que ocupa en y de un char
#define BORDER_X 1016 //maximo ancho (en pixeles) de la pantalla
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


static char charsInShell[MAX_FIL_IN_SHELL][MAX_COLLS_IN_SHELL];
static int xMatrizShell=0;
static int yMatrizShell=0;

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
			charsInShell[yMatrizShell][xMatrizShell++]=*palabra;
			if(xMatrizShell==MAX_COLLS_IN_SHELL){
				yMatrizShell++;
				xMatrizShell=0;
			}
			charVideo(palabra[i],1,color); //Che ojo que el 1 ese es importante para cunado se termina la oracion, no lo cambien por el parametro zoom
		}
}

void charVideo(int num, char isEndLine, uint32_t color){
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
                            putPixel(color, x + j * scale + dx, y + i * scale + dy);
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
	yMatrizShell++;
	xMatrizShell=0;
	if(y < BORDER_Y -(16*(zoom+1))){
	aux += MOV_Y*(zoom+1);
	y += MOV_Y*(zoom+1);
	x = 0;
	}
}

void deleteVideo(){ //Borra caracteres
	if(!(y == 0 && x <= 0)){
	if(x >= MOV_X*(zoom+1)){
	x -= MOV_X*(zoom+1);
	charVideo(0,1,BLANCO);
	x -= MOV_X*(zoom+1);
	}
	else{
	x = VBE_mode_info->width -8;
	y -= MOV_Y*(zoom+1);
	aux -= MOV_Y*(zoom+1);
	charVideo(0,0,BLANCO);
	x -= MOV_X*(zoom+1);
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
	imprimirVideo(toPrint, sizeof(toPrint),BLANCO);
}
void printHexaVideo(uint64_t value){
	char* buffer;
	uint32_t digits=uintToBase(value, buffer, 16);
	imprimirVideo(buffer, digits,BLANCO);
}


void  zoomIN() {
	if(zoom <2){
    	zoom++;
	}
}
void zoomOUT() {
	if (zoom>0){
		zoom--;
	}
}

void printMatriz(){
	for(int i=0;i<MAX_FIL_IN_SHELL;i++){
		for (int j = 0; i < MAX_COLLS_IN_SHELL; j++){
			charVideo(charsInShell[i][j],1,BLANCO);
			if(!charsInShell[i][j]){
				break;
			}
		}
		if(!charsInShell[i][0]){
			return;
		}
	}
	return;
}


void putRectangle(int posx, int posy, uint32_t color) {
	for(int i = posx; i < posx+REC_ANCHO; i++) {
		for(int j = posy; j < posy+REC_LARGO; j++) {
			putPixel(color, i, j);
		}
	}
}

void snakeCanvas(uint32_t firstColor, uint32_t secondColor) {
	int flag = 0;
	for(int i = 0; i < BORDER_X + 1; i += REC_ANCHO) {
		flag = !flag;
		for(int j = 0; j < BORDER_Y; j += REC_LARGO) {
			if(flag) {
				putRectangle(i, j, firstColor);
			} else {
				putRectangle(i,j,secondColor);
			}
			flag = !flag;
		}
		
		
	}
}
//imprime un rectangulo en una pos random NO BORRAR!!es para desp hacer las manzanas del snake
/*void getRandomPosition() {
    int minutes = getMins(); 
    int seconds = getSec(); 
    // Generar índices de fila y columna basados en los minutos y segundos
    int totalSquares = REC_X_FIL * REC_X_COL;
     int randomIndex = (minutes * 60 + seconds) % totalSquares;

    // Calcular la fila y columna a partir del índice
    int columnIndex = randomIndex % REC_X_COL;          // Columna
    int rowIndex = randomIndex / REC_X_COL;             // Fila

    int newx = columnIndex * REC_ANCHO; // Multiplicamos el índice de columna por el ancho del cuadrado
    int newy = (randomIndex % REC_X_COL) * REC_LARGO;
	putRectangle(newx, newy, 0x00ffffff);
}*/



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
