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
        messege[i] = (messege[i] + 3)%255;
    }
    return messege;

}

char* decode(char* str, int len)
{

     char* messege = (char*)calloc(len,sizeof(char)) ;
    strcpy(messege, str);

    for (int i = 0; i < len; i++)
    {
        messege[i] = (messege[i] - 3)%255;
    }
    return messege;

}

