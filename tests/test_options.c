//#include "../options/m6_options.h"
#include <stdio.h>

#include "../deps/libcgen/vector.h"
#include "../options/m6_options.h"
#include "../log/log.h"

USE_M6_LOGGER_DEFAULT;
USE_M6_OPTIONS;

struct {
    u64     opt1;
    i64     opt2;
    m6_bool opt3;
    char*   opt4;
    double  opt5;
    Vector  opt6;
    Vector  opt7;
} options;


int main(int argc, char** argv)
{
    m6_opt_adduu(M6_OPTION_REQUIRED,'a',"first","This is the 1st option", &options.opt1);
    m6_opt_addii(M6_OPTION_OPTIONAL,'b',"second","This is the 2nd option", &options.opt2, -42);
    m6_opt_addbi(M6_OPTION_FLAG,    'c',"third","This is the 3rd option", &options.opt3, false);
    m6_opt_addsi(M6_OPTION_OPTIONAL,'d',"fourth","This is the 4th option", &options.opt4, "init string");
    m6_opt_addfi(M6_OPTION_OPTIONAL,'e',"fifth","This is the 5th option", &options.opt5, 42.0);
    m6_opt_addSI(M6_OPTION_OPTIONAL,'f',"sixth","This is the 6th option", &options.opt6, "init strig vector");
    m6_opt_addFU(M6_OPTION_UNLIMTED,'g',"seventh","This is the 7th option", &options.opt7);
    m6_opt_parse(argc,argv);

    printf("a=%lu, b=%li, c=%i, d=%s, e=%lf ", options.opt1, options.opt2, options.opt3, options.opt4, options.opt5);
    int i = 0;
    printf("f=[");
    for(; i < size(Vector,&options.opt6); i++){
        printf("%s,",Vector_geti(char*,options.opt6,i));
    }
    printf(" ] ");

    printf("g=[");
    for(i=0; i < size(Vector,&options.opt7); i++){
        printf("%lf,",Vector_geti(double,options.opt7,i));
    }
    printf("]\n");


    return 0;
}
