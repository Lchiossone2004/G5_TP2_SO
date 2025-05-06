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
