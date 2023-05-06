#include "dlfcn.h"
#include <string.h>
#include "stdio.h"
#include "errno.h"
#include "stdlib.h"
#include "unistd.h"

typedef char* (*p_decode_)(char* str, int len);

int main(int argc, char *argv[])
{

    if (argv[1] && strcmp(argv[1], "codecA") == 0)
    {
        void* handle = dlopen("./libcodecA.so",RTLD_LAZY);
        if(!handle){
            printf("general failure");
            return 1;
        }
        p_decode_ decode = (p_decode_)dlsym(handle,"decode");
        if(!decode){
            printf("general failure");
            dlclose(handle);
            return 1;
        }
        char* ans = decode(argv[2], strlen(argv[2]));
        printf("%s\n",ans);
        free(ans);
        dlclose(handle);
    }
    else if (argv[1] && strcmp(argv[1], "codecB") == 0)
    {
        void* handle = dlopen("./libcodecB.so",RTLD_LAZY);
        if(!handle){
            printf("general failure");
            return 1;
        }
        p_decode_ decode = (p_decode_)dlsym(handle,"decode");
        if(!decode){
            printf("general failure");
            dlclose(handle);
            return 1;
        }
        char* ans = decode(argv[2], strlen(argv[2]));
        printf("%s\n",ans);
        free(ans);
        dlclose(handle);
    }
    else
    {
        printf("Usage : encode/decode <codec> <message> \n");
    }

    return 0;
}