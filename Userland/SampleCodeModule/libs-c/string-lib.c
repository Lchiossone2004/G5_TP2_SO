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