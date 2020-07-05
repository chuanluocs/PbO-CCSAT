#ifndef _INFO_H
#define _INFO_H

#include "basis.h"

extern string version_str;

void print_general_information(char *executable_name);
void print_usage_info(char *executable_name);
void print_algorithm_settings_information();
void print_solution(int *cur_soln, int num_vars);
void print_SAT_result(int *cur_soln, int num_vars);
void print_UNKNOWN_result();
void print_run_info(double run_time, long long tries, long long step);

#endif
