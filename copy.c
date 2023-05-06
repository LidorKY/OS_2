#include <string.h>
#include "stdio.h"
#include "errno.h"
#include "stdlib.h"
#include "unistd.h"
int main(int argc, char *argv[])
{

    int v_flag = 0;
    int f_flag = 0;
    if (argv[3] && strcmp(argv[3], "-f") == 0)
    {
        f_flag = 1;
    }
    else if (argv[3] && strcmp(argv[3], "-v") == 0)
    {
        v_flag = 1;
    }
    if (argv[4] && strcmp(argv[4], "-f") == 0)
    {
        f_flag = 1;
    }
    else if (argv[4] && strcmp(argv[4], "-v") == 0)
    {
        v_flag = 1;
    }

    FILE *fp1 = fopen(argv[1], "rb");

    if (fp1 == NULL)
    {
        if (v_flag)
            printf("general failure\n");

        return 1;
    }

    if ((argv[0] && strcmp(argv[0], "-v") == 0) || (argv[0] && strcmp(argv[0], "-f") == 0) || (argv[1] && strcmp(argv[1], "-v") == 0) || (argv[1] && strcmp(argv[1], "-f") == 0) || (argv[2] && strcmp(argv[2], "-v") == 0) || (argv[2] && strcmp(argv[2], "-f") == 0))
    {
        printf("Usage : ./copy <file1> <file2> <(flags)> \n");
        return 1;
    }

    if(argc <= 3){
        if((argv[2] && strcmp(argv[2], "-v") == 0) || (argv[2] && strcmp(argv[2], "-f") == 0))
        {
            printf("Usage : ./copy <file1> <file2> <(flags)> \n");
            return 1;
        }
        if(argc <= 2){
            printf("Usage : ./copy <file1> <file2> <(flags)> \n");
            return 1;
        }
    }

    if (access((argv[2]), F_OK) == 0)
    {
        // if we enter here the file exist
        if (f_flag)
        {
            remove(argv[2]);
            FILE *fp2 = fopen(argv[2], "wb");
            if (fp2 == NULL)
            {
                // we didn't manage to create a file
                if (v_flag)
                    printf("general failure\n");

                return 1;
            }

            char buff[1000] = {0};
            while (!feof(fp1))
            {
                fgets(buff, 1000, fp1);
                fputs(buff, fp2);
                bzero(buff,sizeof(buff));
            }

            fclose(fp1);
            fclose(fp2);
            if (v_flag)
                printf("success\n");
            return 0;
        }
        if (v_flag)
            printf("target file exist\n");

        fclose(fp1);
        return 1;
    }

    FILE *fp2 = fopen(argv[2], "wb");
    if (fp2 == NULL)
    {
        // we didn't manage to create a file
        if (v_flag)
            printf("general failure\n");

        return 1;
    }

    char buff[1000] = {0};
    while (!feof(fp1))
    {
        fgets(buff, 1000, fp1);
        fputs(buff, fp2);
        bzero(buff,sizeof(buff));
    }

    fclose(fp1);
    fclose(fp2);
    if (v_flag)
        printf("success\n");
    return 0;
}