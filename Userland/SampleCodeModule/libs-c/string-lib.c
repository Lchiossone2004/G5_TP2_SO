#include <string-lib.h>
#include <stddef.h>
int strSize(char *word) {
    int toRet = 0;
    while (word[toRet] != 0)
    {
        toRet++;
    }
    return toRet;
}

int strCompare(char *s1, char *s2) {
    int flag = 1;
    if (strSize(s1) != strSize(s2))
    {
        flag = 0;
    }
    for (int i = 0; i < strSize(s1) && flag; i++) {
        if (s1[i] != s2[i]) {
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



size_t strlen(const char *s) {
    size_t len = 0;
    while (s[len] != '\0') {
        len++;
    }
    return len;
}
int isVowel(char c) {
    return (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u' ||
            c == 'A' || c == 'E' || c == 'I' || c == 'O' || c == 'U');
}

int removeRepeated(char *str) {
    if (str == NULL || str[0] == '\0') {
        return 0;
    }

    int len = strlen(str);
    if (len < 2) {
        return len;  
    }
    int j = 0;
    for (int i = 0; i < len; i++) {
        int is_repeated = 0;
    
        for (int k = 0; k < j; k++) {
            if (str[i] == str[k]) {
                is_repeated = 1;
                break;
            }
        }
        if (!is_repeated) {
            str[j] = str[i];
            j++;
        }
    }

    str[j] = '\0';  
    return j;  
}