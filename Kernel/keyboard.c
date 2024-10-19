#include <stdint.h>
#include <string.h>
#include <naiveConsole.h>
#include <keyboard.h>

void printKey(){ //Busca e impirme la letra que se quiere o si borra si se toco la tecla de borrado 
int i = getKey();
	if(i != 0 ){
          ncPrintHex(i);
	ncPrintChar(toLetter(i));
	}

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

        // Espacio, Enter, y otros
        case 0x39: return ' ';     // Espacio
        case 0x1C: return '\n';    // Enter
        case 0x0F: return '\t';    // Tab

        // Flechas (devolviendo un valor de ejemplo, ya que son códigos extendidos)
        case 0xE048: return '^';   // Flecha arriba
        case 0xE050: return 'v';   // Flecha abajo
        case 0xE04B: return '<';   // Flecha izquierda
        case 0xE04D: return '>';   // Flecha derecha

        default: return '?';       // Tecla no reconocida
    }
    

/*
    switch(i){

        case 130:
        return '1';
        case 131:
        return '2';
        case 132:
        return '3';
        case 133:
        return '4';
        case 134:
        return '5';
         case 135:
        return '6';
        case 136:
        return '7';
        case 137:
        return '8';
        case 138:
        return '9';
        case 139:
        return '0';


        case 144:
        return 'q';
        case 145:
        return 'w';
        case 146:
        return 'e';
        case 147:
        return 'r';
        case 148:
        return 't';
         case 149:
        return 'y';
        case 150:
        return 'u';
        case 151:
        return 'i';
        case 152:
        return 'o';
        case 153:
        return 'p';

        case 158:
        return 'a';
        case 159:
        return 's';
        case 160:
        return 'd';
        case 161:
        return 'f';
        case 162:
        return 'g';
         case 163:
        return 'h';
        case 164:
        return 'j';
        case 165:
        return 'k';
        case 166:
        return 'l';

        case 172:
        return 'z';
        case 173:
        return 'x';
        case 174:
        return 'c';
        case 175:
        return 'v';
        case 176:
        return 'b';
         case 177:
        return 'n';
        case 178:
        return 'm';
        case 179:
        return ',';
        case 180:
        return '.';

        case 185:
        return ' ';

        case 156:
        ncNewline();
        return;

        default:
        return '-';
    }
*/
}