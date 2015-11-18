#include <stdlib.h>
#include <stdio.h>

int main(int argc, char** argv)
{
    #ifdef NDEBUG
        printf("release %d\n", argc);
    #else
        printf("debug %d\n", argc);
    #endif
    return 0;
}
