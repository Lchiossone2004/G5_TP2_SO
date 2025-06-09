#include <time-lib.h>
#include <c-lib.h>
#include <stdint.h>

#define STDIN 0
#define STDOUT 1
#define STDERR 2

void syscall(uint64_t rsi, ...);

static char Timepo[3];
void printTime(int area)
{
	print("		");
	getTime();
	char toPrint[9] = "hh:mm:ss";
	char aux = Timepo[0] >> 4;
	char aux2 = (Timepo[0] & 0xF);
	if (aux2 >= 0x03 && area)
	{
		aux2 = aux2 - 0x3;
	}
	else if (aux >= 0x01 && area)
	{
		aux = aux - 0x01;
		aux2 = aux2 + 0x07;
	}
	else if (aux == 0 && area)
	{
		aux = aux + 0x02;
		aux2 = aux2 + 0x01;
	}
	toPrint[0] = aux + '0';
	toPrint[1] = aux2 + '0'; // guardo los 4 bits menos significativos y el resto lo pongo en 0

	aux = Timepo[1] >> 4;
	toPrint[3] = aux + '0';
	toPrint[4] = (Timepo[1] & 0xF) + '0';
	aux = Timepo[2] >> 4;
	toPrint[6] = aux + '0';
	toPrint[7] = (Timepo[2] & 0xF) + '0';
	if (area == 0)
	{
		print("(UTC): ");
	}
	else
	{
		print("(ARG): ");
	}
	print(toPrint);
	print("\n");
}

void getTime()
{
	syscall(8, STDIN, Timepo);
}