#include <naiveConsole.h>


static char buffer[64] = { '0' };
static uint8_t * const video = (uint8_t*)0xB8000;
static uint8_t * currentVideo = (uint8_t*)0xB8000;
static uint8_t * maxVideo  = (uint8_t*)0xB8000; //puntero a la maxima posicion del puntero de texto (osea al ultimo caracter)
static uint8_t * timeVideo; //puntero que apunta a la direccion donde se va a imprimir constantemente el reloj 
static const uint32_t width = 80;
static const uint32_t height = 25 ;

void ncPrint(const char * string){
	int i;

	for (i = 0; string[i] != 0; i++)
		ncPrintChar(string[i]);
}

void ncPrintChar(char character){	
	cambioCursor();
	*currentVideo = character;
	currentVideo += 2;
	cursor();
	if(currentVideo > maxVideo){
		maxVideo = currentVideo;
	}
}

void ncNewline(){
	do
	{
		ncPrintChar(' ');
	}
	while((uint64_t)(currentVideo - video) % (width * 2) != 0);
}

void ncPrintDec(uint64_t value){
	ncPrintBase(value, 10);
}

void ncPrintHex(uint64_t value){
	ncPrintBase(value, 16);
}

void ncPrintBin(uint64_t value){
	ncPrintBase(value, 2);
}

void ncPrintBase(uint64_t value, uint32_t base){
    uintToBase(value, buffer, base);
    ncPrint(buffer);
}

void ncClear(){
	int i;

	for (i = 0; i < height * width; i++)
		video[i * 2] = ' ';
	currentVideo = video;
}

uint32_t uintToBase(uint64_t value, char * buffer, uint32_t base){
	char *p = buffer;
	char *p1, *p2;
	uint32_t digits = 0;

	//Calculate characters for each digit
	do
	{
		uint32_t remainder = value % base;
		*p++ = (remainder < 10) ? remainder + '0' : remainder + 'A' - 10;
		digits++;
	}
	while (value /= base);

	// Terminate string in buffer.
	*p = 0;

	//Reverse string in buffer.
	p1 = buffer;
	p2 = p - 1;
	while (p1 < p2)
	{
		char tmp = *p1;
		*p1 = *p2;
		*p2 = tmp;
		p1++;
		p2--;
	}

	return digits;
}

// Funcion que imprime el tiempo 

void timePrint(int hours, int minutes, int seconds){
	timeVideo = (uint8_t*)0xB8088;
	uintToBase(hours,buffer,16);
	for(int i = 0; buffer[i] != 0; i++){
		*timeVideo = buffer[i];
		timeVideo += 2;
	}
	*timeVideo = ':';
	timeVideo += 2; 
	uintToBase(minutes,buffer,16);
	for(int i = 0; buffer[i] != 0; i++){
		*timeVideo = buffer[i];
		timeVideo += 2;
	}
	*timeVideo = ':';
	timeVideo += 2; 
	uintToBase(seconds,buffer,16);
	for(int i = 0; buffer[i] != 0; i++){
		*timeVideo = buffer[i];
		timeVideo += 2;
	}

}

void ncDelete(){ //borra caracteres y mueve toda la cadena uno hacia la izquierda por si borramos algo de la mitad del texto 
	cambioCursor();
	currentVideo -=2;
	*currentVideo = ' ';
	uint8_t * auxVideo;
	auxVideo = currentVideo;
	while(auxVideo < maxVideo){
		*auxVideo = *(auxVideo + 2);
		auxVideo += 2;
	}
	maxVideo -=2;
	cursor();
}
void ncMovDir(){ //mueve el puntero de texto hacia la derecha 
	cambioCursor();
	currentVideo += 2;
	cursor();
}

void ncMovIzq(){ //mueve el puntero de texto hacia la izquierda
	cambioCursor();
	currentVideo -= 2;
	cursor();
}

void cursor(){			//Pone el cursor  
	*(currentVideo + 1)= 0x2f;
}
void cambioCursor(){    //Saca el cursor
	*(currentVideo + 1) = 7; 
}
