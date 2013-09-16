/*
 * test_num_parser.c
 *
 *  Created on: Dec 11, 2012
 *      Author: mgrosvenor
 */

#include "../parsing/numeric_parser.h"
#include <stdio.h>

#include "log/log.h"

ch_word test_num_parser() {

    //Test cases
    char* tests[] = {
    "999",                      //0
    "+100",                     //1
    "-100",                     //2
    "000",                      //3
    "010",                      //4
    "0x00",                     //5
    "0xFF            ",         //6
    "0x100",                    //7
    "0b00",                     //8
    "0b01",                     //9
    "     0b100",               //10
    "0.1",                      //11
    ".1",                       //12
    "-.100",                    //13
    "-0.01",                    //14
    "+100.0\0",                 //15
    "-100.0",                   //16
    " \n\n\t\r\n\t100",         //17
    //Should fail in in case where a number could be interpreted, but it's probably not what the user meant
    "100X",                     //18
    "--100",                    //19
    "++100",                    //20
    "-X100",                    //21
    "0b012",                    //22
    "-0x100",                   //23
    "A100",                     //24
    "0A00",                     //25
    //Should fail in clearly broken cases
    "OOO",                      //26
    "XXX",                      //27
    //Prefixes
    "0",                        //28
    "1",                        //29
    "10",                       //30
    "11",                       //31
    "0M",                       //32
    "0ki",                      //33
    "1K",                       //34
    "1Ki",                      //35
    "1.024M",                   //36
    "1mi",                      //37
    "3.6G",                     //38
    "2Gi",                      //39
    //Scientific notation
//    "0E0",                      //40
//    "1E0",                      //41
//    "1E1",                      //42
//    "1E10",                     //43
//    "1E100",                    //44
//    "1E1000",                   //45
//    "0.1E0",                    //46
//    "0.1E1",                    //47
//    "0.1E10",                   //48
//    "-1.1E2"                    //49
    //Finished
    ""
    };


    num_result_t num;

    ch_word test_pass = 1;

    int i = 0;
    for(; tests[i][0] != '\0'; i++ ){
        num = parse_number(tests[i],0);

        printf("Numeric Parser Test %02i: ", i);
        switch(i){
            case 0: num = parse_number(tests[i],0); printf("%s\n",( test_pass = (num.type == CH_UINT64 && num.val_uint == 999)    ) ? "Pass" : "Fail" ); break;
            case 1: num = parse_number(tests[i],0); printf("%s\n",( test_pass = (num.type == CH_UINT64 && num.val_uint == 100)    ) ? "Pass" : "Fail" ); break;
            case 2: num = parse_number(tests[i],0); printf("%s\n",( test_pass = (num.type == CH_INT64  && num.val_int  == -100)   ) ? "Pass" : "Fail" ); break;
            case 3: num = parse_number(tests[i],0); printf("%s\n",( test_pass = (num.type == CH_UINT64 && num.val_int  == 0)      ) ? "Pass" : "Fail" ); break;
            case 4: num = parse_number(tests[i],0); printf("%s\n",( test_pass = ( num.type == CH_UINT64 && num.val_int  == 8)     ) ? "Pass" : "Fail" ); break;
            case 5: num = parse_number(tests[i],0); printf("%s\n",( test_pass = ( num.type == CH_UINT64 && num.val_int  == 0)     ) ? "Pass" : "Fail" ); break;
            case 6: num = parse_number(tests[i],0); printf("%s\n",( test_pass = ( num.type == CH_UINT64 && num.val_int  == 0xFF)  ) ? "Pass" : "Fail" ); break;
            case 7: num = parse_number(tests[i],0); printf("%s\n",( test_pass = ( num.type == CH_UINT64 && num.val_int  == 0x100) ) ? "Pass" : "Fail" ); break;
            case 8: num = parse_number(tests[i],0); printf("%s\n",( test_pass = ( num.type == CH_UINT64 && num.val_int  == 0)     ) ? "Pass" : "Fail" ); break;
            case 9: num = parse_number(tests[i],0); printf("%s\n",( test_pass = ( num.type == CH_UINT64 && num.val_int  == 1)     ) ? "Pass" : "Fail" ); break;
            case 10:num = parse_number(tests[i],0); printf("%s\n",( test_pass = ( num.type == CH_UINT64 && num.val_int  == 4)     ) ? "Pass" : "Fail" ); break;
            case 11:num = parse_number(tests[i],0); printf("%s\n",( test_pass = ( num.type == CH_DOUBLE && num.val_dble == 0.1)   ) ? "Pass" : "Fail" ); break;
            case 12:num = parse_number(tests[i],0); printf("%s\n",( test_pass = ( num.type == CH_DOUBLE && num.val_dble == 0.1)   ) ? "Pass" : "Fail" ); break;
            case 13:num = parse_number(tests[i],0); printf("%s\n",( test_pass = ( num.type == CH_DOUBLE && num.val_dble == -0.1 ) ) ? "Pass" : "Fail" ); break;
            case 14:num = parse_number(tests[i],0); printf("%s\n",( test_pass = ( num.type == CH_DOUBLE && num.val_dble == -0.01) ) ? "Pass" : "Fail" ); break;
            case 15:num = parse_number(tests[i],0); printf("%s\n",( test_pass = ( num.type == CH_DOUBLE && num.val_dble == 100.0) ) ? "Pass" : "Fail" ); break;
            case 16:num = parse_number(tests[i],0); printf("%s\n",( test_pass = ( num.type == CH_DOUBLE && num.val_dble == -100.0)) ? "Pass" : "Fail" ); break;
            case 17:num = parse_number(tests[i],0); printf("%s\n",( test_pass = ( num.type == CH_UINT64 && num.val_uint == 100)   ) ? "Pass" : "Fail" ); break;
            case 18:num = parse_number(tests[i],0); printf("%s\n",( test_pass = ( num.type == CH_NO_TYPE)                         ) ? "Pass" : "Fail" ); break;
            case 19:num = parse_number(tests[i],0); printf("%s\n",( test_pass = ( num.type == CH_NO_TYPE)                         ) ? "Pass" : "Fail" ); break;
            case 20:num = parse_number(tests[i],0); printf("%s\n",( test_pass = ( num.type == CH_NO_TYPE)                         ) ? "Pass" : "Fail" ); break;
            case 21:num = parse_number(tests[i],0); printf("%s\n",( test_pass = ( num.type == CH_NO_TYPE)                         ) ? "Pass" : "Fail" ); break;
            case 22:num = parse_number(tests[i],0); printf("%s\n",( test_pass = ( num.type == CH_NO_TYPE)                         ) ? "Pass" : "Fail" ); break;
            case 23:num = parse_number(tests[i],0); printf("%s\n",( test_pass = ( num.type == CH_NO_TYPE)                         ) ? "Pass" : "Fail" ); break;
            case 24:num = parse_number(tests[i],0); printf("%s\n",( test_pass = ( num.type == CH_NO_TYPE)                         ) ? "Pass" : "Fail" ); break;
            case 25:num = parse_number(tests[i],0); printf("%s\n",( test_pass = ( num.type == CH_NO_TYPE)                         ) ? "Pass" : "Fail" ); break;
            case 26:num = parse_number(tests[i],0); printf("%s\n",( test_pass = ( num.type == CH_NO_TYPE)                         ) ? "Pass" : "Fail" ); break;
            case 27:num = parse_number(tests[i],0); printf("%s\n",( test_pass = ( num.type == CH_NO_TYPE)                         ) ? "Pass" : "Fail" ); break;
            case 28:num = parse_number(tests[i],0); printf("%s\n",( test_pass = ( num.type == CH_UINT64 && num.val_uint == 0)     ) ? "Pass" : "Fail" ); break;
            case 29:num = parse_number(tests[i],0); printf("%s\n",( test_pass = ( num.type == CH_UINT64 && num.val_uint == 1)     ) ? "Pass" : "Fail" ); break;
            case 30:num = parse_number(tests[i],0); printf("%s\n",( test_pass = ( num.type == CH_UINT64 && num.val_uint == 10)    ) ? "Pass" : "Fail" ); break;
            case 31:num = parse_number(tests[i],0); printf("%s\n",( test_pass = ( num.type == CH_UINT64 && num.val_uint == 11)    ) ? "Pass" : "Fail" ); break;
            case 32:num = parse_number(tests[i],0); printf("%s\n",( test_pass = ( num.type == CH_UINT64 && num.val_uint == 0)     ) ? "Pass" : "Fail" ); break;
            case 33:num = parse_number(tests[i],0); printf("%s\n",( test_pass = ( num.type == CH_UINT64 && num.val_uint == 0)     ) ? "Pass" : "Fail" ); break;
            case 34:num = parse_number(tests[i],0); printf("%s\n",( test_pass = ( num.type == CH_UINT64 && num.val_uint == 1000)  ) ? "Pass" : "Fail" ); break;
            case 35:num = parse_number(tests[i],0); printf("%s\n",( test_pass = ( num.type == CH_UINT64 && num.val_uint == 1024)  ) ? "Pass" : "Fail" ); break;
            case 36:num = parse_number(tests[i],0); printf("%s\n",( test_pass = ( num.type == CH_DOUBLE && num.val_dble == 1.024 * 1000 * 1000)         ) ? "Pass" : "Fail" ); break;
            case 37:num = parse_number(tests[i],0); printf("%s\n",( test_pass = ( num.type == CH_UINT64 && num.val_uint == 1024 * 1024)                 ) ? "Pass" : "Fail" ); break;
            case 38:num = parse_number(tests[i],0); printf("%s\n",( test_pass = ( num.type == CH_DOUBLE && num.val_dble == 3.6 * 1000 * 1000 * 1000)    ) ? "Pass" : "Fail" ); break;
            case 39:num = parse_number(tests[i],0); printf("%s\n",( test_pass = ( num.type == CH_UINT64 && num.val_uint == 2 * 1024 * 1024 * 1024ULL)   ) ? "Pass" : "Fail" ); break;
            default: printf("Fail\n"); break;
        }

        if(!test_pass) return !test_pass;

    }

    return !test_pass;

}

USE_CH_LOGGER_DEFAULT;

int main(int argc, char** argv){

    (void) argc;
    (void) argv;

    return test_num_parser();
}

