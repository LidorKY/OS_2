#include <string.h>
#include "stdio.h"
#include "errno.h"
#include "stdlib.h"
#include <ctype.h>

int main(int argc, char *argv[])
{
    int v_flag =0;
    int i_flag =0;
    if(argv[3] && strcmp(argv[3], "-i") == 0){
        i_flag =1;
    }
    else if(argv[3] && strcmp(argv[3], "-v") == 0){
        v_flag =1;
    }
    if(argv[4] && strcmp(argv[4], "-i") == 0){
        i_flag =1;
    }
    else if(argv[4] && strcmp(argv[4], "-v") == 0){
        v_flag =1;
    }

    FILE *fp1 = fopen(argv[1], "r");
    FILE *fp2 = fopen(argv[2], "r");

    if (fp1 == NULL || fp2 == NULL)
    {
        printf("usage: cmp <file1> <file2> (opt:flag1 opt:flag2)\n \
        -v : insted of returning 1 or 0 will print 'distinct' or 'equal'\n \
        -i : comparing the 2 files without considering the case of the letters.");
        return -1;
    }

    fseek(fp1, 0L, SEEK_END);
    long int fp1_length = ftell(fp1);
    fseek(fp2, 0L, SEEK_END);
    long int fp2_length = ftell(fp2);

    if (fp1_length != fp2_length)
    {                // comparing the length of both files.
        fclose(fp1); // closing the files.
        fclose(fp2);
        if (v_flag)
        {
            printf("distinct\n");
        }
        return 1;
    }

    fseek(fp1, 0, SEEK_SET); // returning the file pointer to the start.
    fseek(fp2, 0, SEEK_SET);

    char c1 = fgetc(fp1);
    char c2 = fgetc(fp2);
    while (c1 != EOF && c2 != EOF)
    { // comparing every char betweens the two files.
        if (i_flag)
        {
            if (tolower(c1) != tolower(c2))
            {
                fclose(fp1); // closing the files.
                fclose(fp2);
                if (v_flag)
                {
                    printf("distinct\n");
                }
                return 1;
            }
        }
        else
        {
            if (c1 != c2)
            {
                fclose(fp1); // closing the files.
                fclose(fp2);
                if (v_flag)
                {
                    printf("distinct\n");
                }
                return 1;
            }
        }
        c1 = fgetc(fp1);
        c2 = fgetc(fp2);
    }

    fclose(fp1); // closing the files.
    fclose(fp2);
    if (v_flag)
    {
        printf("equal\n");
    }
    return 0;
}