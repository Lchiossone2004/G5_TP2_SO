#include <videoDriver.h>

#define ZERO_EXCEPTION_ID 0
#define WRONG_OP_EXCEPTION_ID 1
#define BLANCO  0xFFFFFF

static void zero_division();
static void wrong_op();

void exceptionDispatcher(int exception) {
	if (exception == ZERO_EXCEPTION_ID)
		zero_division();
	if(exception == WRONG_OP_EXCEPTION_ID)
		wrong_op();
}

static void zero_division() {
	// Handler para manejar excepcíon
	imprimirVideo("hola",5,BLANCO);
	return;
}

static void wrong_op() {
	//Handeler para manejar excepcion
}