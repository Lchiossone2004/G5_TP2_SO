#include <time.h>
#include <stdint.h>
#include <naiveConsole.h>
#include <keyboard.h>
#include <lib.h>
#include <videoDriver.h>

static void int_20();
static void int_21();

extern uint64_t * saveCPURegisters();

void irqDispatcher(uint64_t irq) {
	switch (irq) {
		case 0:
			int_20();
			break;
		case 1:
			int_21(); 
	}
	return;
}

void int_20() { 
	timer_handler();
}

void int_21(){ 
	uint16_t key = getKey();
	if(key == 0x01){
		saveCPURegisters();
	}
    uint8_t scancode = key & 0xFF;
    uint8_t isPressed = (key >> 8) & 0x01; 
	updateKeyboardStatus(scancode, isPressed);
	if(isPressed) {
	loadBuffer(scancode);
	}
}
