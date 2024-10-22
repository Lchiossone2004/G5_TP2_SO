#ifndef LIB_H
#define LIB_H

#include <stdint.h>

void * memset(void * destination, int32_t character, uint64_t length);
void * memcpy(void * destination, const void * source, uint64_t length);
int strcmp(const char* str1, const char* str2);

char *cpuVendor(char *result);
int getHours();
int getMins();
int getSec();
int getKey();

#endif