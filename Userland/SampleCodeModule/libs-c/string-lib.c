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
    char temp[20];  // Suficiente para 64 bits
    int i = 0;

    // Manejo de 0 explícitamente
    if (value == 0) {
        buffer[0] = '0';
        buffer[1] = '\0';
        return;
    }

    // Convertir número a string al revés
    while (value > 0) {
        temp[i++] = '0' + (value % 10);
        value /= 10;
    }

    // Invertir el string en el buffer final
    for (int j = 0; j < i; j++) {
        buffer[j] = temp[i - j - 1];
    }
    buffer[i] = '\0';
}
