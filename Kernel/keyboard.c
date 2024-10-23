#include <stdint.h>  //Che no se que tecaron aca pero se rompio el delete
#include <string.h>
#include <naiveConsole.h>
#include <keyboard.h>
#include <videoDriver.h>
#include <lib.h>
static uint64_t buffer_dim = 0;


static uint16_t buffer[BUFFER_SIZE];
static uint64_t dim = 0; //dimension del buffer
static uint64_t curr = 0; //posicion actual del buffer


void printKey(){ //Busca e impirme la letra que se quiere o si borra si se toco la tecla de borrado 
int i = getKey();
	if(i!= 0 && i != 14 && i != 75 && i != 77 && i != 28){
	ncPrintChar(toLetter(i));
    charVideo(toLetter(i),1);

	}
    if(i == 14){
        ncDelete();
        deleteVideo();
    }
    if(i == 75){
        ncMovIzq();
        movVideo(-1);
    }
    if(i== 77){
        ncMovDir();
        movVideo(1);
    }
    if(i==28){
        ncNewline();
        nlVideo();
    }
}


static uint8_t isReleased(uint8_t key){
    return (key & 0x80);
}
static uint8_t isPressed(uint8_t key){
    return !isReleased(key);
}

char toLetter(int i){

    
    switch (i) {
        // Números y símbolos
        case 0x02: return '1';
        case 0x03: return '2';
        case 0x04: return '3';
        case 0x05: return '4';
        case 0x06: return '5';
        case 0x07: return '6';
        case 0x08: return '7';
        case 0x09: return '8';
        case 0x0A: return '9';
        case 0x0B: return '0';
        
        // Letras (minúsculas)
        case 0x1E: return 'a';
        case 0x30: return 'b';
        case 0x2E: return 'c';
        case 0x20: return 'd';
        case 0x12: return 'e';
        case 0x21: return 'f';
        case 0x22: return 'g';
        case 0x23: return 'h';
        case 0x17: return 'i';
        case 0x24: return 'j';
        case 0x25: return 'k';
        case 0x26: return 'l';
        case 0x32: return 'm';
        case 0x31: return 'n';
        case 0x18: return 'o';
        case 0x19: return 'p';
        case 0x10: return 'q';
        case 0x13: return 'r';
        case 0x1F: return 's';
        case 0x14: return 't';
        case 0x16: return 'u';
        case 0x2F: return 'v';
        case 0x11: return 'w';
        case 0x2D: return 'x';
        case 0x15: return 'y';
        case 0x2C: return 'z';
        case 0x39: return ' ';

        default: return '?';       // Tecla no reconocida
    }

}

uint64_t buffer_has_next() {
    return dim > 0 && curr < dim;
}

uint64_t current() {
    if(!buffer_has_next) {
        return 0;
    }
    return buffer[curr++];
}

static uint8_t releasedKeyToPressedMask(uint8_t key){
    return key&0x7F;
}

void keyboardHandler(){
    uint8_t key = getKey();
    uint8_t key_is_pressed = isPressed(key) ? 1:0;
    if( !key_is_pressed){
        key = releasedKeyToPressedMask(key); //la tabla es para PRESSED !
    }
    if(!key_is_pressed){
        return;
    }
    uint16_t code = toLetter(key);
    
    buffer[buffer_dim] = code;
    if(buffer_dim < BUFFER_SIZE){
        buffer_dim++;
    }else{
        buffer_dim = 1;
    }
}