#include <lib.h>
#include <stdint.h>
#include <videoDriver.h>
#include "../memory/memory_manager.h"
#define BLANCO  0xFFFFFF
#define VERDE   0x00FF00
#define TAB "     "
static char * regs[17] = {"RAX:", "RBX:", "RCX:", "RDX:", "RSI:","RDI:", "RBP:","RSP:","RIP:","R8:", "R9:","R10:", "R11:","R12:","R13:", "R14:","R15:"};

extern uint64_t * getCPURegisters();

void * memset(void * destination, int32_t c, uint64_t length) {
	uint8_t chr = (uint8_t)c;
	char * dst = (char*)destination;

	while(length--)
		dst[length] = chr;

	return destination;
}


void * memcpy(void * destination, const void * source, uint64_t length) {
	/*
	* memcpy does not support overlapping buffers, so always do it
	* forwards. (Don't change this without adjusting memmove.)
	*
	* For speedy copying, optimize the common case where both pointers
	* and the length are word-aligned, and copy word-at-a-time instead
	* of byte-at-a-time. Otherwise, copy by bytes.
	*
	* The alignment logic below should be portable. We rely on
	* the compiler to be reasonably intelligent about optimizing
	* the divides and modulos out. Fortunately, it is.
	*/
	uint64_t i;

	if ((uint64_t)destination % sizeof(uint32_t) == 0 &&
		(uint64_t)source % sizeof(uint32_t) == 0 &&
		length % sizeof(uint32_t) == 0)
	{
		uint32_t *d = (uint32_t *) destination;
		const uint32_t *s = (const uint32_t *)source;

		for (i = 0; i < length / sizeof(uint32_t); i++)
			d[i] = s[i];
	}
	else
	{
		uint8_t * d = (uint8_t*)destination;
		const uint8_t * s = (const uint8_t*)source;

		for (i = 0; i < length; i++)
			d[i] = s[i];
	}

	return destination;
}

void printRegisters(){
	uint64_t * registers = getCPURegisters();
    for(int i = 0; i<17; i++){
    imprimirVideo(TAB,5,BLANCO);
    imprimirVideo(regs[i],4,VERDE);
	imprimirVideo("0x",2,BLANCO);
    printHexaVideo(registers[i]);
	if(i%2 == 1){
		nlVideo();
	}
	}
	nlVideo();
	return;
}

int strSize(char *word) {
    int toRet = 0;
    while (word[toRet] != 0)
    {
        toRet++;
    }
    return toRet;
}

int find_index(int arr[], int size, int value) {
    for (int i = 0; i < size; i++) {
        if (arr[i] == value) {
            return i; 
        }
    }
    return -1; 
}
