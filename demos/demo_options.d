//#include "../options/ch_options.h"
#include <stdio.h>

#include "../deps/libcgen/vector.h"
#include "../options/options.h"
#include "../log/log.h"

USE_CH_LOGGER_DEFAULT;
USE_CH_OPTIONS;

struct {
    u64     opt1;
    i64     opt2;
    ch_bool opt3;
    char*   opt4;
    double  opt5;
    Vector  opt6;
    Vector  opt7;
} options;


int main(int argc, char** argv)
{
    ch_opt_adduu(CH_OPTION_REQUIRED,'a',"first","This is the 1st option", &options.opt1);
    ch_opt_addii(CH_OPTION_OPTIONAL,'b',"second","This is the 2nd option", &options.opt2, -42);
    ch_opt_addbi(CH_OPTION_FLAG,    'c',"third","This is the 3rd option", &options.opt3, false);
    ch_opt_addsi(CH_OPTION_OPTIONAL,'d',"fourth","This is the 4th option", &options.opt4, "init string");
    ch_opt_addfi(CH_OPTION_OPTIONAL,'e',"fifth","This is the 5th option", &options.opt5, 42.0);
    ch_opt_addSI(CH_OPTION_OPTIONAL,'f',"sixth","This is the 6th option", &options.opt6, "init strig vector");
    ch_opt_addFU(CH_OPTION_UNLIMTED,'g',"seventh","This is the 7th option", &options.opt7);
    ch_opt_parse(argc,argv);

    printf("a=%lu, b=%li, c=%i, d=%s, e=%lf ", options.opt1, options.opt2, options.opt3, options.opt4, options.opt5);
    unsigned int i = 0;
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
