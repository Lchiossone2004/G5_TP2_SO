#include <time.h>
#include <stdint.h>
#include <naiveConsole.h>
#include <keyboard.h>
#include <lib.h>

static void int_20();
static void int_21();

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

void int_20() { //interrupcion del timer, cada tick llama a imprimir la hora, minutos y segundos 
	timer_handler();
	timePrint(getHours(), getMins(), getSec());
}

void int_21(){ //interrupcion del teclado, constantemente se fija si se presiono una tecla o no
	int i = getKey();
	if(i != 0){
	ncPrintChar(toLetter(i));
	}
}
