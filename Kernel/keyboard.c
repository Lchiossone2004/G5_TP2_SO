#include <stdint.h>  // Bueno hay que arreglar el shift nomas 
#include <string.h>
#include <naiveConsole.h>
#include <keyboard.h>
#include <videoDriver.h>
#include <lib.h>
static uint64_t buffer_dim = 0;
static uint64_t shift_pressed = 0;
static uint64_t caps_pressed = 0;

static uint16_t buffer[BUFFER_SIZE];
static uint64_t dim = 0; //dimension del buffer
static uint64_t curr = 0; //posicion actual del buffer

void updateKeyboardStatus(int scancode) {
    switch (scancode) {
        case 0x2A:  //shift izq 
        case 0x36:  //shift der
            shift_pressed = 1;
            break;
        case 0xAA:  // shift izq
        case 0xB6:  // shift der
            shift_pressed = 0;
            break;
        case 0x3A:  //mayuscula presionado
           caps_pressed = !caps_pressed;
            break;
    }
}
void printKey(){ //Busca e impirme la letra que se quiere o si borra si se toco la tecla de borrado 
int i = getKey();
updateKeyboardStatus(i);
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
//lo mismo que print key solo que en lugar de imprimirlo lo guarda en buffer
void keyInBuffer(char buffer) {
    int i = getKey();
    updateKeyboardStatus(i);
    if(i!= 0 && i != 14 && i != 75 && i != 77 && i != 28) { // si no es delete, las flechitas o enter
        buffer = toLetter(i);
    }
}

// static uint8_t isReleased(uint8_t key){
//     return (key & 0x80);
// }
// static uint8_t isPressed(uint8_t key){
//     return !isReleased(key);
// }
char shiftNum(char num) {
     switch (num) {
            case '1': return '!';
            case '2': return '@';
            case '3': return '#';
            case '4': return '$';
            case '5': return '%';
            case '6': return '^';
            case '7': return '&';
            case '8': return '*';
            case '9': return '(';
            case '0': return ')';
        }
}
char toLetter(int i){
    char aux;
    switch (i) {
        // Números y símbolos
        case 0x02: aux =  '1'; break;
        case 0x03: aux =  '2'; break;
        case 0x04: aux =  '3'; break;
        case 0x05: aux =  '4'; break;
        case 0x06: aux =  '5'; break;
        case 0x07: aux =  '6'; break;
        case 0x08: aux =  '7'; break;
        case 0x09: aux =  '8'; break;
        case 0x0A: aux =  '9'; break;
        case 0x0B: aux =  '0'; break;
        
        // Letras (minúsculas)
        case 0x1E: aux =  'a'; break;
        case 0x30: aux =  'b'; break;
        case 0x2E: aux =  'c'; break;
        case 0x20: aux =  'd'; break;
        case 0x12: aux =  'e'; break;
        case 0x21: aux =  'f'; break;
        case 0x22: aux =  'g'; break;
        case 0x23: aux =  'h'; break;
        case 0x17: aux =  'i'; break;
        case 0x24: aux =  'j'; break;
        case 0x25: aux =  'k'; break;
        case 0x26: aux =  'l'; break;
        case 0x32: aux =  'm'; break;
        case 0x31: aux =  'n'; break;
        case 0x18: aux =  'o'; break;
        case 0x19: aux =  'p'; break;
        case 0x10: aux =  'q'; break;
        case 0x13: aux =  'r'; break;
        case 0x1F: aux =  's'; break;
        case 0x14: aux =  't'; break;
        case 0x16: aux =  'u'; break;
        case 0x2F: aux =  'v'; break;
        case 0x11: aux =  'w'; break;
        case 0x2D: aux =  'x'; break;
        case 0x15: aux =  'y'; break;
        case 0x2C: aux =  'z'; break;
        case 0x39: aux =  ' '; break;
        case 0x2A: break; //shift izquierdo
        case 0x36: break; //shift derecho
        case 0x3A: break; //capslock
        //en estos ultimos 3 no quiero que ponga ?
        default: return '?';       // Tecla no reconocida
    }
    if (aux >= 'a' && aux <= 'z' && (caps_pressed || shift_pressed )) {
        aux  -= 32;  // Convierte a mayúscula (restando 32 en ASCII)
    } else 
    if(aux >= '0' && aux <= '9' && shift_pressed) {
        aux = shiftNum(aux);
    }
    return aux; 
}

// uint64_t buffer_has_next() {
//     return dim > 0 && curr < dim;
// }

// uint64_t current() {
//     if(!buffer_has_next) {
//         return 0;
//     }
//     return buffer[curr++];
// }

// static uint8_t releasedKeyToPressedMask(uint8_t key){
//     return key&0x7F;
// }

// void keyboardHandler(){
//     uint8_t key = getKey();
//     uint8_t key_is_pressed = isPressed(key) ? 1:0;
//     if( !key_is_pressed){
//         key = releasedKeyToPressedMask(key); //la tabla es para PRESSED !
//     }
//     if(!key_is_pressed){
//         return;
//     }
//     uint16_t code = toLetter(key);
    
//     buffer[buffer_dim] = code;
//     if(buffer_dim < BUFFER_SIZE){
//         buffer_dim++;
//     }else{
//         buffer_dim = 1;
//     }
// }