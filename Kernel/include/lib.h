#ifndef LIB_H
#define LIB_H

#include <stdint.h>
#include <stdlib.h>

void * memset(void * destination, int32_t character, uint64_t length);
void * memcpy(void * destination, const void * source, uint64_t length);
void printRegisters();
int strSize(char *word);

char *cpuVendor(char *result);
int getHours();
int getMins();
int getSec();
uint16_t getKey();
int find_index(int arr[], int size, int value);
#endif