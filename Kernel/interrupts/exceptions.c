#include <videoDriver.h>
#include <lib.h>
#include "scheduler.h"
#include "pipe.h"

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
	char * error = "Error: invalid operation, division by 0";
	p_info  * proc = get_current_process();
	pipe_write(proc->stdin,error,strSize(error));
	nlVideo();
	printRegisters();
	return;
}

static void wrong_op() {
	char * error = "Error: invalid op code";
	p_info  * proc = get_current_process();
	pipe_write(proc->stdin,error,strSize(error));
	nlVideo();
	printRegisters();
	return; 
}