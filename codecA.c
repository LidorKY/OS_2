#include <string.h>
#include "stdio.h"
#include "errno.h"
#include "stdlib.h"
#include "unistd.h"
#include <ctype.h>

char* encode(char* str, int len)
{
    char* messege = (char*)calloc(len,sizeof(char)) ;
    strcpy(messege, str);
    for (int i = 0; i < len; i++)
    {
        if (messege[i] <= 'z' && messege[i] >= 'a')
        {
            messege[i] = toupper(messege[i]);
        }
        else if (messege[i] <= 'Z' && messege[i] >= 'A')
        {
            messege[i] = tolower(messege[i]);
        }
    }
    return messege;
}

char* decode(char* str, int len)
{

    char* messege = (char*)calloc(len,sizeof(char)) ;
    strcpy(messege, str);

    for (int i = 0; i < len; i++)
    {
        if (messege[i] <= 'z' && messege[i] >= 'a')
        {
            messege[i] = toupper(messege[i]);
        }
        else if (messege[i] <= 'Z' && messege[i] >= 'A')
        {
            messege[i] = tolower(messege[i]);
        }
    }
    return messege;
}