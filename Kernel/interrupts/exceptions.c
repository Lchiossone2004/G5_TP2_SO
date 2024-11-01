#include <videoDriver.h>
#include <lib.h>

#define ZERO_EXCEPTION_ID 0
#define WRONG_OP_EXCEPTION_ID 1
#define BLANCO  0xFFFFFF
#define ROJO 	0xFF0000
#define TAB "     "

static void zero_division();
static void wrong_op();

void exceptionDispatcher(int exception) {

	if (exception == ZERO_EXCEPTION_ID)
		zero_division();
	if(exception == WRONG_OP_EXCEPTION_ID)
		wrong_op();
	return;
}

static void zero_division() {
	imprimirVideo(TAB, 5,ROJO);
	imprimirVideo("Error: invalid operation, division by 0",40,ROJO);
	nlVideo();
	printRegisters();
	return;
}

static void wrong_op() {
	imprimirVideo(TAB, 5,ROJO);
	imprimirVideo("Error: invalid op code",23,ROJO);
	nlVideo();
	printRegisters();
	return; 
}