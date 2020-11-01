#ifndef _SETTINGS_H
#define _SETTINGS_H

#include "basis.h"

extern void (*init_solution_ptr) ();

extern void (*init_unsatvar_related_ptr) ();
extern void (*update_unsatvar_related_in_unsat_function_ptr) (int);
extern void (*update_unsatvar_related_in_sat_function_ptr) (int);

extern void (*init_nvccdvar_related_ptr) ();
extern void (*update_nvccdvar_related_in_update_function_ptr) (int flipvar);
extern void (*update_nvccdvar_related_in_swt_scheme_ptr) ();
extern void (*update_nvccdvar_related_in_dec_paws_scheme_ptr) ();
extern void (*update_nvccdvar_related_in_inc_paws_scheme_ptr) ();


extern void (*init_sat_var2_ptr) ();
extern void (*update_sat_var2_ptr) (int);

extern void (*init_score2_ptr) ();
extern void (*update_score2_ptr) (int);

extern void (*init_time_stamp_ptr) ();
extern void (*update_time_stamp_ptr) (int, long long);

extern void (*init_score_ptr) ();
extern void (*update_score_ptr) (int);

extern void (*init_break_score_ptr) ();
extern void (*update_break_score_ptr) (int);

extern void (*init_make_score_ptr) ();
extern void (*update_make_score_ptr) (int);

extern void (*build_neighbor_relation_ptr) ();
extern void (*init_nvcc_conf_change_ptr) ();
extern void (*update_nvcc_conf_change_ptr) (int);

extern void (*init_cscc_conf_change_ptr) ();
extern void (*update_cscc_conf_change_ptr) (int);


extern int (*select_var_pac_heuristic_ptr) ();

extern int (*select_unsat_clause_heuristic_first_div_ptr) ();
extern int (*select_var_from_clause_heuristic_first_div_ptr) (int);

extern int (*select_var_cscc_heuristic_ptr) ();
extern int (*select_var_nvcc_heuristic_ptr) ();
extern int (*select_var_aspiration_heuristic_ptr) ();

extern int (*select_var_by_break_tie_in_set_ptr) (int*, int);

extern void (*init_clause_weight_scheme_ptr) ();
extern void (*clause_weight_scheme_ptr) ();

extern int (*select_unsat_clause_heuristic_div_ptr) ();
extern int (*select_var_from_clause_heuristic_div_ptr) (int);

void default_algorithm_settings();
void algorithm_settings_random_7_sat();
void algorithm_settings_random_6_sat();
void algorithm_settings_random_5_sat();
void algorithm_settings_random_4_sat();
void algorithm_settings_random_3_sat();
void algorithm_settings_structured_instances();

void select_algorithm_settings();

#endif
