#include<stdint.h>
#include <videoDriver.h>
#include <font.h>

#define MOV_X 8  //Lo que ocupa en x de un char 
#define MOV_Y 16 //Lo que ocupa en y de un char
#define BORDER_X 1016 //maximo ancho (en pixeles) de la pantalla
#define BORDER_Y 768 //maximo largo (en pixeles) de la pantalla
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

static int x = 0;
static int y = 0;
static int aux = 0;
static int maxX = 0;
static int maxY = 0;
static int num_columns = BORDER_X / MOV_X; 
static int num_rows = BORDER_Y / MOV_Y; 
static char letters[BORDER_Y / MOV_Y][BORDER_X / MOV_X] = {0};

void putPixel(uint32_t hexColor, uint64_t x, uint64_t y) {
    uint8_t * framebuffer = (uint8_t *) VBE_mode_info->framebuffer;
    uint64_t offset = (x * ((VBE_mode_info->bpp)/8)) + (y * VBE_mode_info->pitch);
    framebuffer[offset]     =  (hexColor) & 0xFF;
    framebuffer[offset+1]   =  (hexColor >> 8) & 0xFF; 
    framebuffer[offset+2]   =  (hexColor >> 16) & 0xFF;
}

void imprimirVideo(char * palabra){
		for(int i = 0; palabra[i] != 0; i++){
			y = aux;	
			charVideo(palabra[i],1);
		}
}

void charVideo(int num, int flag){
	if(x<=BORDER_X && y < BORDER_Y){
	int set;
	char *bitmap;
	bitmap = font[num];
	if(flag){
		letters[y/MOV_Y][x/MOV_X] = num;
	}
	else{
		letters[y/MOV_Y][x/MOV_X] = 0;
	}
	for(int i = 0; i <8; i++){
		for(int j = 0; j<8; j++){
			set = bitmap[i] & 1 << j;
			if(set){
				putPixel(0x00FF0000,(x)+j,(y)+i);
			}
			else{
				putPixel(0x0,(x)+j,(y)+i);
			}
		}
		y += 1;
	}
		x += MOV_X;
		if(x>maxX){
			maxX = x;
		}
		if(x >= VBE_mode_info->width && flag){ //Estaria bueno sacar este flag, que es para que borre correctamente 
			aux += MOV_Y;
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
	charVideo(letters[0][0],1);


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
/*void printTimeVideo(char hour, char min, char sec) {
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
	imprimirVideo(toPrint);
	}*/
