#include <string-lib.h>

int strSize(char *word)
{
    int toRet = 0;
    while (word[toRet] != 0)
    {
        toRet++;
    }
    return toRet;
}

int strCompare(char *s1, char *s2)
{
    int flag = 1;
    if (strSize(s1) != strSize(s2))
    {
        flag = 0;
    }
    for (int i = 0; i < strSize(s1) && flag; i++)
    {
        if (s1[i] != s2[i])
        {
            flag = 0;
        }
    }
    return flag;
}

void size_t_to_str(size_t value, char* buffer) {
    char temp[20];
    int i = 0;

    if (value == 0) {
        buffer[0] = '0';
        buffer[1] = '\0';
        return;
    }

    while (value > 0) {
        temp[i++] = '0' + (value % 10);
        value /= 10;
    }

    for (int j = 0; j < i; j++) {
        buffer[j] = temp[i - j - 1];
    }
    buffer[i] = '\0';
}

int strToInt(const char *str) {
    int res = 0;
    while (*str >= '0' && *str <= '9') {
        res = res * 10 + (*str - '0');
        str++;
    }
    return res;
}

void strCpy(char* dest, const char* src) {
    char* ptr = dest;
    while (*src != '\0') {
        *ptr++ = *src++;
    }
    *ptr = '\0';  
}

void deleteSpaces(char * input){
    int i = 0, j = 0;
    int space_flag = 0;

    while (input[i] == ' '){ 
        i++;
    }
    while (input[i] != '\0') {
        if (input[i] != ' ') {
            input[j++] = input[i++];
            space_flag = 0;
        } else {
            if (!space_flag) {
                input[j++] = ' ';
                space_flag = 1;
            }
            i++;
        }
    }

    if (j > 0 && input[j - 1] == ' ') {
        j--;
    }
    input[j] = '\0'; 
}

#include <stddef.h>

void intToString(int value, char *buffer, size_t bufferSize){
    if (bufferSize < 2) {

        if (bufferSize > 0)
            buffer[0] = '\0';
        return;
    }
    int isNegative = 0;

    if (value < 0) {
        isNegative = 1;
        value = -value;
    }

    char temp[12]; 
    size_t j = 0;

    if (value == 0) {
        temp[j++] = '0';
    } else {
        while (value > 0 && j < sizeof(temp)) {
            temp[j++] = '0' + (value % 10);
            value /= 10;
        }
    }

    if (isNegative && j < sizeof(temp)) {
        temp[j++] = '-';
    }
    if (j >= bufferSize) {
        buffer[0] = '\0';
        return;
    }

    for (size_t k = 0; k < j && k < bufferSize - 1; k++) {
        buffer[k] = temp[j - k - 1];
    }

    buffer[j < bufferSize - 1 ? j : bufferSize - 1] = '\0';
}
char *strpbrk(const char *str1, const char *str2) {
    while (*str1) {
        const char *s2 = str2;
        while (*s2) {
            if (*str1 == *s2) {
                return (char *)str1;
            }
            s2++;
        }
        str1++;
    }
    return NULL; 
}
char *strchr(const char *str, int c) {
    while (*str != '\0') {
        if (*str == (char)c) {
            return (char *)str; 
        }
        str++;
    }
    return NULL;  
}
char* strtok(char* str, const char* delimiters) {
    static char* static_str = NULL; 
    char* token;
    
    if (str) {
        static_str = str;
    }

    if (!static_str) {
        return NULL;  
    }
    while (*static_str && strchr(delimiters, *static_str)) {
        static_str++;
    }

    if (!*static_str) {
        return NULL;  
    }

    token = static_str;


    while (*static_str && !strchr(delimiters, *static_str)) {
        static_str++;
    }

    if (*static_str) {
        *static_str = '\0';  
        static_str++;  
    }

    return token;
}
size_t strlen(const char *s) {
    size_t len = 0;
    while (s[len] != '\0') {
        len++;
    }
    return len;
}
int isBlank(char c) {
    return (c == ' '  ||
            c == '\t' ||
            c == '\n' ||
            c == '\v' ||
            c == '\f' ||
            c == '\r');
}

int isVowel(char c) {
    return (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u' ||
            c == 'A' || c == 'E' || c == 'I' || c == 'O' || c == 'U');
}

