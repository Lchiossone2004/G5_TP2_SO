#include "../include/time.h"
#include "../include/libc.h"
#include <stdint.h>

#define STDIN 0
#define STDOUT 1
#define STDERR 2


syscall(uint64_t rsi, ...);

static time *Timepo;
void printTime(int area){
    getTime(area);
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
	if(area == 0){
		print("(UTC): ",7);
	}
	else{
		print("(ARG): ",7);
	}
    print(toPrint,8);
}

void getTime(int area){
    syscall(12,STDIN,Timepo,area);
}