#include <stdint.h>  // Bueno hay que arreglar el shift nomas y el ctrl(tema release)
#include <string.h>
#include <naiveConsole.h>
#include <keyboard.h>
#include <videoDriver.h>
#include <lib.h>
#include "scheduler.h"
#include "pipe.h"

static uint64_t shift_pressed = 0;
static uint64_t ctrl_pressed = 0;
static uint64_t caps_pressed = 0;


static uint16_t buffer[BUFFER_SIZE];
static uint64_t curr = 0; //posicion actual del buffer

int specialKey(uint8_t key) {
    return  (key == 0 || key == 0x01 || key == 14 || key == 75 || key == 77 || key == 28 || key == 0x1D || key == 0x3A || key == 0x2A || key == 0x36 || key == 0xAA || key == 0xB6 || key == 0x0F );
}
void loadBuffer(uint8_t key){
    //updateKeyboardStatus(key,);
    if(curr == 12) {
        curr = 0;
    }

    if(ctrl_pressed && key == 0x2E){
        p_info * foreground_proc = get_foreground_process();
        kill_process(foreground_proc->pid);
        return;
    }
    if(ctrl_pressed && key == 0x20){ 
        //Comportamiento del ctrl+d
    }
    if(!specialKey(key)){
        char letter = toLetter(key); 
        pipe_write(STDIN,&letter,1);    
        buffer[curr++] = letter;

    }
    if(key == 14){      //Borrado  
        char * aux = 0;
        pipe_write(STDIN,aux,1);        
        buffer[curr++] = 0;
    }
    if(key == 28){      //Enter
        char * aux = 1;
        pipe_write(STDIN,aux,1);      
        buffer[curr++] = 1;
    }
    if(key == 0x0F){   //TAB
        char * aux = ' ';
        pipe_write(STDIN,aux,1);  
        pipe_write(STDIN,aux,1); 
        pipe_write(STDIN,aux,1); 
        pipe_write(STDIN,aux,1); 
        pipe_write(STDIN,aux,1);     
        buffer[curr++] = ' ';
        buffer[curr++] = ' ';
        buffer[curr++] = ' ';
        buffer[curr++] = ' ';
        buffer[curr++] = ' ';
    }
}

int isBufferEmpty(){
    if(curr == 0){
        return 1;
    }
    return 0;
}

char getBuffer(){
    char aux = buffer[0];
    for(int i = 0; i< curr-1; i++){
        buffer[i] = buffer[i+1];
    }
    curr -= 1;
    return aux;
}
uint16_t getFromBuffer(int idx) {
    if(idx >= BUFFER_SIZE) {
        return '0';
    }
    return buffer[idx];
}
int getCurr() {
    return curr;
}

void updateKeyboardStatus(uint8_t scancode, uint8_t isPressed) {
    if (scancode == 0x2A || scancode == 0x36) {  // Shift izquierdo o derecho
        shift_pressed = isPressed;  // Cambia según esté presionado o no
    }else if(scancode == 0x1D || scancode == 0xE01D){
        ctrl_pressed = isPressed;
    } 
    else if (scancode == 0x3A && isPressed) {  // Caps Lock presionado (toggle)
        caps_pressed = !caps_pressed;
    }
}

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
        return '0'; //nunca va  a llegar acá
}
char toLetter(uint8_t i){
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
        case 0x0C: aux =  '-'; break;
        case 0x0D: aux =  '='; break;
        
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
        case 0x2A:  break; //shift izquierdo
        case 0x36:  break; //shift derecho
        case 0x1D:  break; //ctrl
        case 0x3A:  break; //capslock
        case 0x2B: aux =  '|'; break; 
        
        default: return '?';       // Tecla no reconocida
    }
    if (aux >= 'a' && aux <= 'z' && (caps_pressed || shift_pressed )) {  //si es una letra y esta apretado shift o prendido el capslock
        aux  -= 32;  //la paso a mayúscula
    } else 
    if(aux >= '0' && aux <= '9' && shift_pressed) { //si es un número y esta el shift apretado
        aux = shiftNum(aux); //actua segun el numero que corresponda
    }

    return aux; 
}

