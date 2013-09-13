#include <stdio.h>

#include "../string/string.h"
#include "../types/types.h"
#include "../utils/util.h"

//Enforece invariants for the string structure
#define CH_ASSERT_STR_INVARIANTS(str) \
    CH_ASSERT(str.is_const == -1 || str.is_const == 0 || str.is_const == 1); \
    CH_ASSERT(  (str.is_const > 0 && (str.slen == str.mlen)) || str.slen == -1 || str.slen < str.mlen ); \
    CH_ASSERT(  (str.is_const > 0 && (str.slen == str.mlen)) || str.mlen == -1 || str.mlen > str.slen ); \
    CH_ASSERT( str.cstr == NULL || \
             ( str.cstr && str.mlen == -1 && str.slen == -1 && str.is_const == -1 ) || \
             ( str.is_const > 0 && str.slen == (ch_word)strlen(str.cstr) && str.mlen == str.slen ) || \
             ( str.slen == (ch_word)strlen(str.cstr) && str.mlen > str.slen ) )



//Make sure NULL assignment works
ch_word test1()
{
    ch_word result = 1;
    ch_str str = CH_STR_NULL;

    CH_ASSERT(str.cstr == NULL);
    CH_ASSERT(str.is_const == 1);
    CH_ASSERT(str.mlen == 0);
    CH_ASSERT(str.slen == 0);
    CH_ASSERT_STR_INVARIANTS(str);

    return result;
}

//Test the constant literal with an empty literal
ch_word test2()
{
    ch_word result = 1;
    ch_str str = CH_STR_CONST_LIT("");

    CH_ASSERT(*str.cstr == '\0' );
    CH_ASSERT(str.is_const == 1);
    CH_ASSERT(str.mlen == 0);
    CH_ASSERT(str.slen == 0);
    CH_ASSERT_STR_INVARIANTS(str);

    return result;
}

//Test the constant literal with a value
ch_word test3()
{
    ch_word result = 1;
    ch_str str = CH_STR_CONST_LIT("TESTstring\n");

    CH_ASSERT(*str.cstr == 'T' );
    CH_ASSERT(str.is_const == 1);
    CH_ASSERT(str.mlen == 11);
    CH_ASSERT(str.slen == 11);
    CH_ASSERT_STR_INVARIANTS(str);

    return result;
}

//Test the new string constant helper
ch_word test4()
{
    ch_word result = 1;
    CH_STR_CONST_NEW(str,"TESTstring\n");

    CH_ASSERT(*str.cstr == 'T' );
    CH_ASSERT(str.is_const == 1);
    CH_ASSERT(str.mlen == 11);
    CH_ASSERT(str.slen == 11);
    CH_ASSERT_STR_INVARIANTS(str);

    return result;
}


char* test5_helper()
{
    return "TESTstring\n";
}

//Test the from_funtion allocator
ch_word test5()
{
    ch_word result = 1;
    ch_str str = CH_STR_FUNC( test5_helper() );

    CH_ASSERT(*str.cstr == 'T' );
    CH_ASSERT(str.is_const == -1);
    CH_ASSERT(str.mlen == -1);
    CH_ASSERT(str.slen == -1);
    CH_ASSERT_STR_INVARIANTS(str);

    return result;
}



//Test malloc wrapper with null string and non zero backing size
ch_word test6()
{
    ch_word result = 1;
    ch_str str = CH_STR("", 512);

    CH_ASSERT(*str.cstr == '\0' );
    CH_ASSERT(str.is_const == 0);
    CH_ASSERT(str.mlen == 1024);
    CH_ASSERT(str.slen == 0);
    CH_ASSERT_STR_INVARIANTS(str);

    ch_str_free(&str);

    return result;
}

//Test the malloc string wrapper with null input
ch_word test7()
{
    ch_word result = 1;
    ch_str str = CH_STR("", 0);

    CH_ASSERT(*str.cstr == '\0' );
    CH_ASSERT(str.is_const == 0);
    CH_ASSERT(str.mlen == 1);
    CH_ASSERT(str.slen == 0);
    CH_ASSERT_STR_INVARIANTS(str);

    ch_str_free(&str);

    return result;
}

char* test8_helper_1()
{
    return NULL;
}

char* test8_helper_2()
{
    return "";
}

char* test8_helper_3()
{
    return "TESTstring\n";
}


//Test the eqaulity funcion
ch_word test8()
{
    ch_word result = 1;
    ch_str str1 = CH_STR("", 0);
    ch_str str2 = CH_STR_NULL;
    ch_str str3 = CH_STR_FUNC(test8_helper_1());
    ch_str str4 = CH_STR_FUNC(test8_helper_2());

    CH_ASSERT_STR_INVARIANTS(str1);
    CH_ASSERT_STR_INVARIANTS(str2);
    CH_ASSERT_STR_INVARIANTS(str3);
    CH_ASSERT_STR_INVARIANTS(str4);

    CH_ASSERT(ch_str_eq(str1, str1));
    CH_ASSERT(ch_str_eq(str1, str2));
    CH_ASSERT(ch_str_eq(str2, str2));
    CH_ASSERT(ch_str_eq(str2, str3));
    CH_ASSERT(ch_str_eq(str3, str3));
    CH_ASSERT(ch_str_eq(str3, str4));
    CH_ASSERT(ch_str_eq(str4, str4));

    CH_ASSERT_STR_INVARIANTS(str1);
    CH_ASSERT_STR_INVARIANTS(str2);
    CH_ASSERT_STR_INVARIANTS(str3);
    CH_ASSERT_STR_INVARIANTS(str4);


    ch_str str5 = CH_STR("TESTstring\n", 0);
    ch_str str6 = CH_STR_FUNC(test8_helper_3());

    CH_ASSERT(ch_str_eq(str5, str5));
    CH_ASSERT(ch_str_eq(str5, str6));
    CH_ASSERT(ch_str_eq(str6, str6));

    CH_ASSERT_STR_INVARIANTS(str5);
    CH_ASSERT_STR_INVARIANTS(str6);

    ch_str_free(&str1);

    return result;
}


//Test adding nulls together.
ch_word test9()
{
    ch_word result = 1;
    ch_str str1 = CH_STR("", 0);
    ch_str str2 = CH_STR_NULL;

    CH_ASSERT_STR_INVARIANTS(str1);
    CH_ASSERT_STR_INVARIANTS(str2);


    CH_ASSERT( ch_str_eq( ch_str_cat(&str1,str2), str1) );
    CH_ASSERT( ch_str_eq( ch_str_cat(&str1,str2), str2) );

    CH_ASSERT_STR_INVARIANTS(str1);
    CH_ASSERT_STR_INVARIANTS(str2);

    CH_ASSERT( ch_str_eq( ch_str_cat(&str2,str1), str1) );
    CH_ASSERT( ch_str_eq( ch_str_cat(&str2,str1), str2) );

    CH_ASSERT_STR_INVARIANTS(str1);
    CH_ASSERT_STR_INVARIANTS(str2);

    CH_ASSERT( ch_str_eq( ch_str_cat(&str1,str1), str1) );
    CH_ASSERT( ch_str_eq( ch_str_cat(&str2,str2), str2) );

    CH_ASSERT_STR_INVARIANTS(str1);
    CH_ASSERT_STR_INVARIANTS(str2);

    return result;
}


//Test adding non nulls together.
ch_word test10()
{
    ch_word result = 1;
    ch_str str1 = CH_STR("TEST", 512);
    ch_str str2 = CH_STR_CONST_LIT("string\n");

    ch_str ans1 = CH_STR_CONST_LIT("TESTstring\n");
    ch_str ans2 = CH_STR_CONST_LIT("string\nTEST");
    ch_str ans3 = CH_STR_CONST_LIT("TESTTEST");
    ch_str ans4 = CH_STR_CONST_LIT("string\nstring\n");

    CH_ASSERT_STR_INVARIANTS(str1);
    CH_ASSERT_STR_INVARIANTS(str2);
    CH_ASSERT_STR_INVARIANTS(ans1);
    CH_ASSERT_STR_INVARIANTS(ans2);
    CH_ASSERT_STR_INVARIANTS(ans3);

    CH_ASSERT( ch_str_eq( ch_str_cat(&str1,str2), ans1) );

    CH_ASSERT_STR_INVARIANTS(str1);
    CH_ASSERT_STR_INVARIANTS(str2);
    CH_ASSERT_STR_INVARIANTS(ans1);
    CH_ASSERT_STR_INVARIANTS(ans2);
    CH_ASSERT_STR_INVARIANTS(ans3);

    CH_ASSERT( ch_str_eq( ch_str_cat(&str2,str1), ans2) );

    CH_ASSERT_STR_INVARIANTS(str1);
    CH_ASSERT_STR_INVARIANTS(str2);
    CH_ASSERT_STR_INVARIANTS(ans1);
    CH_ASSERT_STR_INVARIANTS(ans2);
    CH_ASSERT_STR_INVARIANTS(ans3);

    CH_ASSERT( ch_str_eq( ch_str_cat(&str1,str1), ans3) );

    CH_ASSERT_STR_INVARIANTS(str1);
    CH_ASSERT_STR_INVARIANTS(str2);
    CH_ASSERT_STR_INVARIANTS(ans1);
    CH_ASSERT_STR_INVARIANTS(ans2);
    CH_ASSERT_STR_INVARIANTS(ans3);

    CH_ASSERT( ch_str_eq( ch_str_cat(&str2,str2), ans4) );

    CH_ASSERT_STR_INVARIANTS(str1);
    CH_ASSERT_STR_INVARIANTS(str2);
    CH_ASSERT_STR_INVARIANTS(ans1);
    CH_ASSERT_STR_INVARIANTS(ans2);
    CH_ASSERT_STR_INVARIANTS(ans3);


    return result;
}


int main(int argc, char** argv)
{
    (void)argc;
    (void)argv;

    ch_word test_pass = 0;
    printf("CH String Test 01: ");  printf("%s", (test_pass = test1()) ? "PASS\n" : "FAIL\n"); if(!test_pass) return 1;
    printf("CH String Test 02: ");  printf("%s", (test_pass = test2()) ? "PASS\n" : "FAIL\n"); if(!test_pass) return 1;
    printf("CH String Test 03: ");  printf("%s", (test_pass = test3()) ? "PASS\n" : "FAIL\n"); if(!test_pass) return 1;
    printf("CH String Test 04: ");  printf("%s", (test_pass = test4()) ? "PASS\n" : "FAIL\n"); if(!test_pass) return 1;
    printf("CH String Test 05: ");  printf("%s", (test_pass = test5()) ? "PASS\n" : "FAIL\n"); if(!test_pass) return 1;
    printf("CH String Test 06: ");  printf("%s", (test_pass = test6()) ? "PASS\n" : "FAIL\n"); if(!test_pass) return 1;
    printf("CH String Test 07: ");  printf("%s", (test_pass = test7()) ? "PASS\n" : "FAIL\n"); if(!test_pass) return 1;
    printf("CH String Test 08: ");  printf("%s", (test_pass = test8()) ? "PASS\n" : "FAIL\n"); if(!test_pass) return 1;
    printf("CH String Test 09: ");  printf("%s", (test_pass = test9()) ? "PASS\n" : "FAIL\n"); if(!test_pass) return 1;
    printf("CH String Test 10: ");  printf("%s", (test_pass = test10()) ? "PASS\n" : "FAIL\n"); if(!test_pass) return 1;


    return 0;
}



