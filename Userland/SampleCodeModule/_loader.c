/* _loader.c */
#include <stdint.h>
#include <memory-lib.h>

extern char bss;
extern char endOfBinary;

int main();


int _start() {
	
	usr_memset(&bss, 0, &endOfBinary - &bss);

	return main();

}



