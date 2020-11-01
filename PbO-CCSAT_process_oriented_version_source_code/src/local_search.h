#ifndef _LOCAL_SEARCH_H
#define _LOCAL_SEARCH_H

#include "basis.h"

int pick_var_pbocc();
int pick_var_walksat_skc();

void flip(int flipvar);
void update(int flipvar, long long step);
bool local_search();
bool local_search_each_try();

#endif
