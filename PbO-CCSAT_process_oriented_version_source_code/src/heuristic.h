#ifndef _HEURISTIC_H
#define _HEURISTIC_H

#include "basis.h"

void empty_function_ret_void();
void empty_function_ret_void(int);
void empty_function_ret_void(int, long long);
int empty_function_ret_int();
int empty_function_ret_int(int);
int empty_function_ret_int(int*, int);

void init_solution_random();
void init_solution_all_true_or_false();
void init_solution_each_lit_true_or_false();

void init_unsatvar_related();
void update_unsatvar_related_in_unsat_function(int clause);
void update_unsatvar_related_in_sat_function(int clause);

void init_nvccdvar_related();
void update_nvccdvar_related_in_update_function(int flipvar);
void update_nvccdvar_related_in_swt_scheme();
void update_nvccdvar_related_in_dec_paws_scheme();
void update_nvccdvar_related_in_inc_paws_scheme();


void init_sat_var2();
void update_sat_var2(int flipvar);

void init_score2();
void update_score2(int flipvar);

void init_time_stamp();
void update_time_stamp(int flipvar, long long step);

void init_score();
void update_score(int flipvar);

void init_break_score();
void update_break_score(int flipvar);

void init_make_score();
void update_make_score(int flipvar);

void build_neighbor_relation();
void init_nvcc_conf_change();
void update_nvcc_conf_change(int flipvar);

void init_cscc_conf_change();
void update_cscc_conf_change(int flipvar);


int select_var_pac_heuristic();

int select_unsat_clause_random_heuristic();
int select_unsat_clause_weight_distribution_heuristic();

int select_var_from_clause_random_heuristic (int clause_num);
int select_var_from_clause_greatest_score_heuristic(int clause_num);
int select_var_from_clause_greatest_age_heuristic(int clause_num);
int select_var_from_clause_novelty_heuristic(int clause_num);
int select_var_from_clause_sparrow_heuristic(int clause_num);
int select_var_from_clause_probsat_heuristic(int clause_num);

int select_var_cscc_heuristic_from_unsatvar_stack();
int select_var_cscc_heuristic_from_nvccdvar_stack();
int select_var_nvcc_heuristic_from_unsatvar_stack();
int select_var_nvcc_heuristic_from_nvccdvar_stack();
int select_var_aspiration_heuristic();


int select_var_by_random_break_tie_in_set(int* the_set, int the_set_length);
int select_var_by_greatest_age_break_tie_in_set(int* the_set, int the_set_length);
int select_var_by_greatest_hscore_break_tie_in_set(int* the_set, int the_set_length);
int select_var_from_clause_greatest_hscore_heuristic(int clause_num);
int select_var_by_greatest_hscore2_break_tie_in_set(int* the_set, int the_set_length);
int select_var_from_clause_greatest_hscore2_heuristic(int clause_num);

void init_clause_weight_scheme_swt();
void clause_weight_scheme_swt();
void init_clause_weight_scheme_paws();
void clause_weight_scheme_paws();





#endif
