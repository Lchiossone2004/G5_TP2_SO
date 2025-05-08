#ifndef STRING_LIB
#define STRING_LIB

#include <stdint.h>
#include <stdlib.h>

int strSize(char *word);
int strCompare(char *s1, char *s2);
void size_t_to_str(size_t value, char* buffer);

#endif