#include "../include/time.h"
#include "../include/libc.h"
#include <stdint.h>

syscall(uint64_t rsi, ...);

static time *Timepo;
void printTime(){
    getTime();
    char toPrint[9] = "hh:mm:ss";
	char aux = Timepo->hours >> 4;
	toPrint[0] = aux + '0';
	toPrint[1] = (Timepo->hours&0xF) + '0'; //guardo los 4 bits menos significativos y el resto lo pongo en 0
	aux = Timepo->mins >> 4;
	toPrint[3] = aux + '0';
	toPrint[4] = (Timepo->mins&0xF) + '0';
	aux = Timepo->seconds >> 4;
	toPrint[6] = aux + '0';
	toPrint[7] = (Timepo->seconds&0xF) + '0';
    print(toPrint,8);
}

void getTime(){
    syscall(13,Timepo);
}