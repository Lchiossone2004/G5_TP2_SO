#ifndef STRING_LIB
#define STRING_LIB

#include <stdint.h>
#include <stdlib.h>

int strSize(char *word);
int strCompare(char *s1, char *s2);
void size_t_to_str(size_t value, char* buffer);
int strToInt(const char *str);
void intToString(int value, char *buffer, size_t bufferSize);
size_t strlen(const char *s); 
int isVowel(char c);
int removeRepeated(char *str);
#endif