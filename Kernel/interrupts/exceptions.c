
#define ZERO_EXCEPTION_ID 0
#define WRONG_OP_EXCEPTION_ID 1

static void zero_division();

void exceptionDispatcher(int exception) {
	if (exception == ZERO_EXCEPTION_ID)
		zero_division();
	if(exception == WRONG_OP_EXCEPTION_ID)
		wrong_op();
}

static void zero_division() {
	// Handler para manejar excepcíon
}

static void wrong_op() {
	//Handeler para manejar excepcion
}