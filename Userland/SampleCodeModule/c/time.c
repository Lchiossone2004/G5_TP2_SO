#include "../include/time.h"
#include "../include/libc.h"
#include <stdint.h>

#define STDIN 0
#define STDOUT 1
#define STDERR 2


void syscall(uint64_t rsi, ...);

static time *Timepo;
void printTime(int area){
    getTime();
    char toPrint[9] = "hh:mm:ss";
	char aux =  Timepo->hours >> 4;
	char aux2 = (Timepo->hours&0xF);
	if(aux2 >= 0x03 && area){
		aux2 =aux2-0x3;
	}
	else if(aux >= 0x01 && area){
		aux = aux -0x01;
		aux2 =  aux2 + 0x07; 
	}
	else if(aux == 0 && area){
		aux = aux + 0x02;
		aux2 = aux2 + 0x01;
	}
	toPrint[0] = aux + '0';
	toPrint[1] = aux2 + '0'; //guardo los 4 bits menos significativos y el resto lo pongo en 0


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

void getTime(){
    syscall(12,STDIN,Timepo);
}