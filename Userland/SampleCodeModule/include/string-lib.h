#ifndef STRING_LIB
#define STRING_LIB

#include <stdint.h>
#include <stdlib.h>

int strSize(char *word);
int strCompare(char *s1, char *s2);
void size_t_to_str(size_t value, char* buffer);
int strToInt(const char *str);
void strCpy(char* dest, const char* src);
void deleteSpaces(char * str);
void intToString(int value, char *buffer, size_t bufferSize);
char *strpbrk(const char *str1, const char *str2);
char *strchr(const char *str, int c);
char* strdup(const char* str);

#endif