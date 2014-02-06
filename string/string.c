/** string_utils.c
 *
 *  Created on: Jul 6, 2013
 *      Author: mgrosvenor
 */

#include "string.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "../utils/util.h"


ch_str ch_str_new(const char* s,i64 size, ch_bool is_char)
{
    ch_str result;
    result.mlen = MIN( size + 1, getpagesize()); //Put a sensible bound on the size of these things
    result.mlen = next_pow2(result.mlen); //Grow this up a little bit so we don't have to alloc too much
    result.slen = is_char ? 2 : strnlen(s,size);
    if(unlikely(result.slen > result.mlen - 1)){
        result.slen = result.mlen - 1;
    }

    result.cstr = (char*)malloc(result.mlen);

    if(!result.cstr){
        fprintf(stderr,"Error: Could not allocate memory for ch_string\n");
        const ch_str null = CH_STR_NULL;
        result = null;
        return result;
    }

    result.cstr[result.mlen -1] = '\0'; //Always null terminate the whole thing
    memcpy(result.cstr,s,result.slen);//Also null terminate the actual string.
    result.cstr[result.slen] = '\0';

    result.is_const = 0;

    return result;

}
//We can't always set the string length, so sometimes it is unset, this sanitizes it before use
//In general, ch_str should be passed by value, this is one of the few exceptions, but it is a static local
static void ch_str_sanitize(ch_str* rhs)
{

    if(rhs){
        if(rhs->cstr == NULL){
            rhs->slen = 0;
            rhs->mlen = rhs->mlen >= 0LL ? rhs->mlen : 0;
        }
        else{
            rhs->slen = rhs->slen >= 0LL ? rhs->slen : (i64)strlen(rhs->cstr);
            rhs->mlen = rhs->mlen >= 0LL ? rhs->mlen : rhs->slen + 1; //Include the null terminator
        }
        //TODO Add some other sanity checks here
    }
    else{
        fprintf(stderr, "Error: Null ch_string passed to %s\n", __FUNCTION__);
    }

}


i64 ch_str_resize(ch_str* rhs,i64 newsize)
{

    ch_str_sanitize(rhs);

    if(rhs->is_const != 0){
        return -1; //Cannot resize a constant string or a string with unknown constantness
    }


    rhs->mlen = MIN(newsize + 1,getpagesize()); //Put a sensible bound on the size of these things
    rhs->mlen *= 2; //Grow this up a little bit so we don't have to alloc too much
    if(unlikely(rhs->slen > rhs->mlen - 1)){
        rhs->slen = rhs->mlen - 1;
    }

    rhs->cstr = realloc(rhs->cstr,rhs->mlen); //Put a little bit of extra memory aside
    if(!rhs->cstr){
        fprintf(stderr, "Error:Could not resize ch_string\n");
        const ch_str null = CH_STR_NULL;
        *rhs = null;
        return -1;
    }

    rhs->cstr[rhs->mlen -1] = '\0'; //Always null terminate the whole thing
    rhs->cstr[rhs->slen] = '\0';

    return 0;
}

//Remove count characters from the end of lhs
ch_str ch_str_trunc(ch_str* lhs, ch_word count)
{
    ch_str_sanitize(lhs);
    if( lhs->is_const == 0){
        lhs->slen = MAX(lhs->slen - count,0);
        lhs->cstr[lhs->slen] = '\n';
        return *lhs;
    }

    return ch_str_new(CH_STR_CSTR(*lhs), lhs->slen - count, 0);
}




//Concatenate rhs onto lhs.
ch_str ch_str_cat(ch_str* lhs, ch_str rhs)
{
    ch_str_sanitize(lhs);
    ch_str_sanitize(&rhs);

    //Quick exits
    if(rhs.slen == 0){
        return *lhs;
    }

    if(lhs->slen == 0 && rhs.slen == 0){
        return *lhs;
    }


    const i64 result_size = lhs->slen + rhs.slen + 1;

    ch_str* result = NULL;

    //See if we can use the lhs
    if(lhs->is_const == 0 && lhs->mlen - lhs->slen >= result_size){ //There is enough memory already
        result = lhs;
    }
    else{
        if(!ch_str_resize(lhs, result_size)){
            //Woot resized!
            result = lhs;
        }
        //lame couldn't resize the LHS
    }


    //Can simply cat, RHS onto the back of LHS at this point.
    if(result){
        memcpy(&lhs->cstr[lhs->slen], rhs.cstr, rhs.slen);
        lhs->cstr[result_size - 1 ] = '\0'; //Make sure we are null terminated
        lhs->cstr[lhs->mlen - 1] = '\0'; //Make sure the whole memory is also null term'd
        lhs->slen = lhs->slen + rhs.slen;
        //Woot! All done.
        return *lhs;
    }


    //If we've got to this point, we need to alloc some new memory for all of this.
    ch_str result_new = ch_str_new(lhs->cstr,result_size, 0);
    //now recurse! Since we have a new LHS allocated enough memory for the rest, this should simply fall into the lhs case above
    return ch_str_cat(&result_new,rhs);

}


ch_str ch_str_cat_cstr(ch_str* lhs, const char* cstr)
{
    ch_str ch_str_rhs = CH_STR_CONST_LIT(cstr);
    return ch_str_cat(lhs, ch_str_rhs);
}


ch_str ch_str_cat_char(ch_str* lhs, const char ch)
{
    ch_str ch_str_rhs = ch_str_new(&ch,1,1);
    return ch_str_cat(lhs, ch_str_rhs);
}


ch_word ch_str_eq(ch_str lhs, ch_str rhs)
{
    ch_str_sanitize(&lhs);
    ch_str_sanitize(&rhs);

    if(lhs.slen == 0 && rhs.slen == 0){
        return 1;
    }


    return !strncmp(lhs.cstr, rhs.cstr, MIN(lhs.slen, rhs.slen));
}


void ch_str_free(ch_str* s){
    if(s->is_const == 0){
        free(s->cstr);

        s->cstr = NULL;
        s->mlen = 0;
        s->slen = -1;
    }
}
