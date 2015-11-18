//#include "../options/ch_options.h"
#include <stdio.h>

#include "../data_structs/vector/vector_std.h"
#include "../options/options.h"
#include "../log/log.h"

USE_CH_LOGGER_DEFAULT;
USE_CH_OPTIONS;

struct {
    u64     opt1;
    ch_word   opt2;
    ch_bool opt3;
    char*   opt4;
    double  opt5;
    CH_VECTOR(cstr)*  opt6;
    CH_VECTOR(float)*  opt7;
    u64     opt8;
} options;


int main(int argc, char** argv)
{
    ch_opt_addfi(CH_OPTION_OPTIONAL,'e',"fifth","This is the 5th option", &options.opt5, 42.0);
    ch_opt_addxi(CH_OPTION_OPTIONAL,'i',"eighth","This is the 8th option", &options.opt8, 0xDEADBEEF);
    ch_opt_addbi(CH_OPTION_FLAG,    'c',"third","This is the 3rd option", &options.opt3, false);
    ch_opt_adduu(CH_OPTION_REQUIRED,'a',"first","This is the 1st option", &options.opt1);
    ch_opt_addFU(CH_OPTION_UNLIMTED,'g',"seventh","This is the 7th option", &options.opt7);
    ch_opt_addsi(CH_OPTION_OPTIONAL,'d',"fourth","This is the 4th option", &options.opt4, "init string");
    ch_opt_addii(CH_OPTION_OPTIONAL,'b',"second","This is the 2nd option", &options.opt2, -42);
    ch_opt_addSI(CH_OPTION_OPTIONAL,'f',"sixth","This is the 6th option", &options.opt6, "init strig vector");

    ch_opt_parse(argc,argv);

    printf("a=%llu, b=%lli, c=%i, d=%s, e=%lf ", options.opt1, options.opt2, options.opt3, options.opt4, options.opt5);
    printf("f=[");
    for(ch_cstr* opt = options.opt6->first; opt < options.opt6->end; opt = options.opt6->next(options.opt6, opt)){
        printf("%s,",*opt);
    }
    printf(" ] ");

    printf("g=[");
    for(ch_float* opt = options.opt7->first; opt < options.opt7->end; opt = options.opt7->next(options.opt7, opt)){
        printf("%lf,",*opt);
    }
    printf("]\n");


    return 0;
}
