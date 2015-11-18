// CamIO 2: perf_mod.c
// Copyright (C) 2013: Matthew P. Grosvenor (matthew.grosvenor@cl.cam.ac.uk) 
// Licensed under BSD 3 Clause, please see LICENSE for more details. 

#include "../perf/perf_mon.h"
#include "../log/log.h"


make_perf_module(TSC test1[1]; TSC test2[2]; TSC test3[3];);

struct some_state {
    ch_word state_var_1;
    ch_word state_var_2;
    perf_mod_generic_t* perf;
};


void init(struct some_state* state)
{
    state->state_var_1 = 0;
    state->state_var_2 = 1;
    state->perf = get_perf_mod_ref;
}


void test1(struct some_state* state)
{
    //perf_mod.test1[0].start_count += 1;
    perf_mod_start(test1);
    state->state_var_1++;
    perf_mod_end(test1,0);
}

void test2(struct some_state* state)
{
    perf_mod_start(test2);
    if(state->state_var_1 % 2){
        state->state_var_2++;
        perf_mod_end(test2,0);
        return;
    }

    state->state_var_1++;
    perf_mod_end(test2,1);
}

void test3(struct some_state* state)
{
    perf_mod_start(test3);
    if(state->state_var_1 % 5 == 4){
        state->state_var_2+= 7;
        perf_mod_end(test3,0);
        return;
    }

    if(state->state_var_1 % 7 == 1){
        state->state_var_2++;
        state->state_var_1++;
        perf_mod_end(test3,1);
        return;
    }

    state->state_var_2++;
    perf_mod_end(test3,2);
}


USE_CH_LOGGER_DEFAULT;


int main(int argc, char** argv)
{
    (void)argc;
    (void)argv;

    struct some_state state;
    init(&state);

    for(ch_word i = 0; i < 1000; i++){
        test1(&state);
        test2(&state);
        test3(&state);
    }

    print_perf_stats(state.perf);


}
