#ifndef _SETTINGS_CPP
#define _SETTINGS_CPP

#include "settings.h"

void (*init_solution_ptr) ();

void (*init_unsatvar_related_ptr) ();
void (*update_unsatvar_related_in_unsat_function_ptr) (int);
void (*update_unsatvar_related_in_sat_function_ptr) (int);

void (*init_nvccdvar_related_ptr) ();
void (*update_nvccdvar_related_in_update_function_ptr) (int flipvar);
void (*update_nvccdvar_related_in_swt_scheme_ptr) ();
void (*update_nvccdvar_related_in_dec_paws_scheme_ptr) ();
void (*update_nvccdvar_related_in_inc_paws_scheme_ptr) ();

void (*init_sat_var2_ptr) ();
void (*update_sat_var2_ptr) (int);

void (*init_score2_ptr) ();
void (*update_score2_ptr) (int);

void (*init_time_stamp_ptr) ();
void (*update_time_stamp_ptr) (int, long long);

void (*init_score_ptr) ();
void (*update_score_ptr) (int);

void (*init_break_score_ptr) ();
void (*update_break_score_ptr) (int);

void (*init_make_score_ptr) ();
void (*update_make_score_ptr) (int);

void (*build_neighbor_relation_ptr) ();
void (*init_nvcc_conf_change_ptr) ();
void (*update_nvcc_conf_change_ptr) (int);

void (*init_cscc_conf_change_ptr) ();
void (*update_cscc_conf_change_ptr) (int);


int (*select_var_pac_heuristic_ptr) ();

int (*select_unsat_clause_heuristic_first_div_ptr) ();
int (*select_var_from_clause_heuristic_first_div_ptr) (int);

int (*select_var_cscc_heuristic_ptr) ();
int (*select_var_nvcc_heuristic_ptr) ();
int (*select_var_aspiration_heuristic_ptr) ();

int (*select_var_by_break_tie_in_set_ptr) (int*, int);

void (*init_clause_weight_scheme_ptr) ();
void (*clause_weight_scheme_ptr) ();

int (*select_unsat_clause_heuristic_div_ptr) ();
int (*select_var_from_clause_heuristic_div_ptr) (int);


void select_algorithm_settings()
{
	default_algorithm_settings();
	/*
	init_nvccdvar_related_ptr = empty_function_ret_void;
	update_nvccdvar_related_in_update_function_ptr = empty_function_ret_void;
	update_nvccdvar_related_in_swt_scheme_ptr = empty_function_ret_void;
	update_nvccdvar_related_in_dec_paws_scheme_ptr = empty_function_ret_void;
	
	select_var_cscc_heuristic_ptr = select_var_cscc_heuristic_from_unsatvar_stack;
	select_var_nvcc_heuristic_ptr = select_var_nvcc_heuristic_from_unsatvar_stack;
	*/
}


void default_algorithm_settings() //Default settings: DCCASat for structured instances
{
	algorithm_settings_structured_instances();
	//algorithm_settings_random_3_sat();
	//algorithm_settings_random_4_sat();
	//algorithm_settings_random_5_sat();
	//algorithm_settings_random_6_sat();
	//algorithm_settings_random_7_sat();
}


void algorithm_settings_random_7_sat() //DCCASat for random 7-SAT instances at phase transition
{
	algorithm_settings_random_5_sat();
	para_d_hscore = 6;
	para_smooth_probability_paws = 0.92;
}

void algorithm_settings_random_6_sat() //DCCASat for random 6-SAT instances at phase transition
{
	algorithm_settings_random_5_sat();
	para_d_hscore = 7;
	para_smooth_probability_paws = 0.9;
}

void algorithm_settings_random_5_sat() //DCCASat for random 5-SAT instances at phase transition
{
	init_solution_ptr = init_solution_random;

	init_unsatvar_related_ptr = init_unsatvar_related;
	update_unsatvar_related_in_unsat_function_ptr = update_unsatvar_related_in_unsat_function;
	update_unsatvar_related_in_sat_function_ptr = update_unsatvar_related_in_sat_function;	

	
	init_nvccdvar_related_ptr = init_nvccdvar_related;
	update_nvccdvar_related_in_update_function_ptr = update_nvccdvar_related_in_update_function;
	update_nvccdvar_related_in_swt_scheme_ptr = update_nvccdvar_related_in_swt_scheme;
	update_nvccdvar_related_in_dec_paws_scheme_ptr = update_nvccdvar_related_in_dec_paws_scheme;
	update_nvccdvar_related_in_inc_paws_scheme_ptr = update_nvccdvar_related_in_inc_paws_scheme;
	
	init_sat_var2_ptr = init_sat_var2;
	update_sat_var2_ptr = update_sat_var2;
	
	init_score2_ptr = init_score2;
	update_score2_ptr = update_score2;
	
	init_time_stamp_ptr = init_time_stamp;
	update_time_stamp_ptr = update_time_stamp;
	
	init_score_ptr = init_score;
	update_score_ptr = update_score;
	
	init_break_score_ptr = empty_function_ret_void;
	update_break_score_ptr = empty_function_ret_void;
	
	init_make_score_ptr = empty_function_ret_void;
	update_make_score_ptr = empty_function_ret_void;
	
	build_neighbor_relation_ptr = build_neighbor_relation;
	init_nvcc_conf_change_ptr = init_nvcc_conf_change;
	update_nvcc_conf_change_ptr = update_nvcc_conf_change;
	
	init_cscc_conf_change_ptr = init_cscc_conf_change;
	update_cscc_conf_change_ptr = update_cscc_conf_change;
	
	
	
	select_var_pac_heuristic_ptr = empty_function_ret_int;
	
	select_unsat_clause_heuristic_first_div_ptr = empty_function_ret_int;
	select_var_from_clause_heuristic_first_div_ptr = empty_function_ret_int;
	
	select_var_cscc_heuristic_ptr = select_var_cscc_heuristic_from_nvccdvar_stack;
	select_var_nvcc_heuristic_ptr = select_var_nvcc_heuristic_from_nvccdvar_stack;
	select_var_aspiration_heuristic_ptr = select_var_aspiration_heuristic;
	
	select_var_by_break_tie_in_set_ptr = select_var_by_greatest_hscore2_break_tie_in_set;
	
	init_clause_weight_scheme_ptr = init_clause_weight_scheme_paws;
	clause_weight_scheme_ptr = clause_weight_scheme_paws;
	
	select_unsat_clause_heuristic_div_ptr = select_unsat_clause_random_heuristic;
	select_var_from_clause_heuristic_div_ptr = select_var_from_clause_greatest_hscore_heuristic;
	
	maxi_tries = default_maxi_tries_flips_num;
	maxi_flips = 2000000000ll;
	
	decision_perform_pac_heuristic = false;
	decision_perform_first_div = false;
	decision_perform_double_cc = true;
	decision_perform_aspiration = true;
	decision_perform_clause_weight_scheme = true;
	
	para_prob_pac = 0.01;
	para_prob_first_div = 0.05;
	
	para_d_hscore = 8;
	para_beta_hscore = 1000;
	para_gamma_hscore2 = 1000;
	
	para_clause_weight_threshold_swt = 200+(num_vars+250)/500;
	para_p_scale_swt = 0.3;
	para_q_scale_swt = 0.7;

	para_smooth_probability_paws = 0.8;
}

void algorithm_settings_random_4_sat() //DCCASat for random 4-SAT instances at phase transition
{
	algorithm_settings_random_5_sat();
	para_d_hscore = 9;
	para_smooth_probability_paws = 0.75;
}

void algorithm_settings_random_3_sat() //DCCASat for random 3-SAT instances at phase transition
{
	init_solution_ptr = init_solution_random;

	init_unsatvar_related_ptr = init_unsatvar_related;
	update_unsatvar_related_in_unsat_function_ptr = update_unsatvar_related_in_unsat_function;
	update_unsatvar_related_in_sat_function_ptr = update_unsatvar_related_in_sat_function;	

	
	init_nvccdvar_related_ptr = init_nvccdvar_related;
	update_nvccdvar_related_in_update_function_ptr = update_nvccdvar_related_in_update_function;
	update_nvccdvar_related_in_swt_scheme_ptr = update_nvccdvar_related_in_swt_scheme;
	update_nvccdvar_related_in_dec_paws_scheme_ptr = update_nvccdvar_related_in_dec_paws_scheme;
	update_nvccdvar_related_in_inc_paws_scheme_ptr = update_nvccdvar_related_in_inc_paws_scheme;
	
	init_sat_var2_ptr = empty_function_ret_void;
	update_sat_var2_ptr = empty_function_ret_void;
	
	init_score2_ptr = empty_function_ret_void;
	update_score2_ptr = empty_function_ret_void;
	
	init_time_stamp_ptr = init_time_stamp;
	update_time_stamp_ptr = update_time_stamp;
	
	init_score_ptr = init_score;
	update_score_ptr = update_score;
	
	init_break_score_ptr = empty_function_ret_void;
	update_break_score_ptr = empty_function_ret_void;
	
	init_make_score_ptr = empty_function_ret_void;
	update_make_score_ptr = empty_function_ret_void;
	
	build_neighbor_relation_ptr = build_neighbor_relation;
	init_nvcc_conf_change_ptr = init_nvcc_conf_change;
	update_nvcc_conf_change_ptr = update_nvcc_conf_change;
	
	init_cscc_conf_change_ptr = init_cscc_conf_change;
	update_cscc_conf_change_ptr = update_cscc_conf_change;
	
	
	
	select_var_pac_heuristic_ptr = empty_function_ret_int;
	
	select_unsat_clause_heuristic_first_div_ptr = empty_function_ret_int;
	select_var_from_clause_heuristic_first_div_ptr = empty_function_ret_int;
	
	select_var_cscc_heuristic_ptr = select_var_cscc_heuristic_from_nvccdvar_stack;
	select_var_nvcc_heuristic_ptr = select_var_nvcc_heuristic_from_nvccdvar_stack;
	select_var_aspiration_heuristic_ptr = select_var_aspiration_heuristic;
	
	select_var_by_break_tie_in_set_ptr = select_var_by_greatest_age_break_tie_in_set;
	
	init_clause_weight_scheme_ptr = init_clause_weight_scheme_swt;
	clause_weight_scheme_ptr = clause_weight_scheme_swt;
	
	select_unsat_clause_heuristic_div_ptr = select_unsat_clause_random_heuristic;
	select_var_from_clause_heuristic_div_ptr = select_var_from_clause_greatest_age_heuristic;
	
	maxi_tries = default_maxi_tries_flips_num;
	maxi_flips = 2000000000ll;
	
	decision_perform_pac_heuristic = false;
	decision_perform_first_div = false;
	decision_perform_double_cc = true;
	decision_perform_aspiration = true;
	decision_perform_clause_weight_scheme = true;
	
	para_prob_pac = 0.01;
	para_prob_first_div = 0.05;
	
	para_d_hscore = 8;
	para_beta_hscore = 1000;
	para_gamma_hscore2 = 1000;
	
	para_clause_weight_threshold_swt = 200+(num_vars+250)/500;
	para_p_scale_swt = 0.3;
	para_q_scale_swt = 0.7;

	para_smooth_probability_paws = 0.8;
}




void algorithm_settings_structured_instances() //DCCASat for structured instances
{
	init_solution_ptr = init_solution_all_true_or_false;
	//init_solution_ptr = init_solution_random;

	init_unsatvar_related_ptr = init_unsatvar_related; //
	update_unsatvar_related_in_unsat_function_ptr = update_unsatvar_related_in_unsat_function; //
	update_unsatvar_related_in_sat_function_ptr = update_unsatvar_related_in_sat_function; //
	
	init_nvccdvar_related_ptr = init_nvccdvar_related; //
	update_nvccdvar_related_in_update_function_ptr = update_nvccdvar_related_in_update_function; //
	update_nvccdvar_related_in_swt_scheme_ptr = update_nvccdvar_related_in_swt_scheme; //
	update_nvccdvar_related_in_dec_paws_scheme_ptr = update_nvccdvar_related_in_dec_paws_scheme; //
	update_nvccdvar_related_in_inc_paws_scheme_ptr = update_nvccdvar_related_in_inc_paws_scheme; //
	
	init_sat_var2_ptr = empty_function_ret_void; //
	update_sat_var2_ptr = empty_function_ret_void; //
	
	init_score2_ptr = empty_function_ret_void; //
	update_score2_ptr = empty_function_ret_void; //
	
	init_time_stamp_ptr = init_time_stamp; //
	update_time_stamp_ptr = update_time_stamp; //
	
	init_score_ptr = init_score; //
	update_score_ptr = update_score; //
	
	init_break_score_ptr = empty_function_ret_void; //
	update_break_score_ptr = empty_function_ret_void; //
	
	init_make_score_ptr = empty_function_ret_void; //
	update_make_score_ptr = empty_function_ret_void; //
	
	build_neighbor_relation_ptr = build_neighbor_relation; //
	init_nvcc_conf_change_ptr = init_nvcc_conf_change; //
	update_nvcc_conf_change_ptr = update_nvcc_conf_change; //
	
	init_cscc_conf_change_ptr = init_cscc_conf_change; //
	update_cscc_conf_change_ptr = update_cscc_conf_change; //
	
	
	
	select_var_pac_heuristic_ptr = select_var_pac_heuristic_ptr; //empty_function_ret_int; //
	
	select_unsat_clause_heuristic_first_div_ptr = select_unsat_clause_random_heuristic; //empty_function_ret_int; //
	select_var_from_clause_heuristic_first_div_ptr = select_var_from_clause_sparrow_heuristic; //
	//select_var_from_clause_heuristic_first_div_ptr = select_var_from_clause_novelty_heuristic; //empty_function_ret_int; //
	
	select_var_cscc_heuristic_ptr = select_var_cscc_heuristic_from_nvccdvar_stack; //
	select_var_nvcc_heuristic_ptr = select_var_nvcc_heuristic_from_nvccdvar_stack; //
	select_var_aspiration_heuristic_ptr = select_var_aspiration_heuristic; //
	
	select_var_by_break_tie_in_set_ptr = select_var_by_greatest_age_break_tie_in_set; //
	
	init_clause_weight_scheme_ptr = init_clause_weight_scheme_swt; //
	clause_weight_scheme_ptr = clause_weight_scheme_swt; //
	
	select_unsat_clause_heuristic_div_ptr = select_unsat_clause_random_heuristic; //
	select_var_from_clause_heuristic_div_ptr = select_var_from_clause_sparrow_heuristic; //
	//select_var_from_clause_heuristic_div_ptr = select_var_from_clause_novelty_heuristic; //
	
	maxi_tries = default_maxi_tries_flips_num; //
	maxi_flips = 2000000000ll; //
	
	decision_perform_pac_heuristic = false; //
	decision_perform_first_div = false; //
	decision_perform_double_cc = true; //
	decision_perform_aspiration = true; //
	decision_perform_clause_weight_scheme = true; //
	
	para_prob_pac = 0.01; //
	para_prob_first_div = 0.05; //
	
	para_d_hscore = 8; //
	para_beta_hscore = 1000; //
	para_gamma_hscore2 = 1000; //
	
	para_clause_weight_threshold_swt = 300; //
	para_p_scale_swt = 0.3; //
	para_q_scale_swt = 0; //

	para_smooth_probability_paws = 0.8; //
	
	para_prob_novelty = 0.2; //
	
	para_sparrow_c1 = 2.15; //
	para_sparrow_c2 = 4; //
	para_sparrow_c3 = 75000; //
	
	para_probsat_cb = 2.85; //
}


/*
void algorithm_settings()
{
	init_unsatvar_related_ptr = empty_function_ret_void;
	update_unsatvar_related_in_unsat_function_ptr = empty_function_ret_void;
	update_unsatvar_related_in_sat_function_ptr = empty_function_ret_void;	
	
	init_sat_var2_ptr = empty_function_ret_void;
	update_sat_var2_ptr = empty_function_ret_void;
	
	init_score2_ptr = empty_function_ret_void;
	update_score2_ptr = empty_function_ret_void;
	
	init_time_stamp_ptr = empty_function_ret_void;
	update_time_stamp_ptr = empty_function_ret_void;
	
	init_score_ptr = empty_function_ret_void;
	update_score_ptr = empty_function_ret_void;
	
	init_break_score_ptr = init_break_score;
	update_break_score_ptr = update_break_score;
	
	init_make_score_ptr = empty_function_ret_void;
	update_make_score_ptr = empty_function_ret_void;
	
	build_neighbor_relation_ptr = empty_function_ret_void;
	init_nvcc_conf_change_ptr = empty_function_ret_void;
	update_nvcc_conf_change_ptr = empty_function_ret_void;
	
	init_cscc_conf_change_ptr = empty_function_ret_void;
	update_cscc_conf_change_ptr = empty_function_ret_void;
	
	
	
	select_var_pac_heuristic_ptr = empty_function_ret_int;
	
	select_unsat_clause_heuristic_first_div_ptr = empty_function_ret_int;
	select_var_from_clause_heuristic_first_div_ptr = empty_function_ret_int;
	
	select_var_cscc_heuristic_ptr = empty_function_ret_int;
	select_var_nvcc_heuristic_ptr = empty_function_ret_int;
	select_var_aspiration_heuristic_ptr = empty_function_ret_int;
	
	select_var_by_break_tie_in_set_ptr = empty_function_ret_int;
	
	init_clause_weight_scheme_ptr = empty_function_ret_void;
	clause_weight_scheme_ptr = empty_function_ret_void;
	
	select_unsat_clause_heuristic_div_ptr = empty_function_ret_int;
	select_var_from_clause_heuristic_div_ptr = empty_function_ret_int;
	
	decision_perform_pac_heuristic = false;
	decision_perform_first_div = false;
	decision_perform_double_cc = true;
	decision_perform_aspiration = true;
	decision_perform_clause_weight_scheme = true;
	
	para_prob_pac = 0.01;
	para_prob_first_div = 0.05;
	
	para_d_hscore = 8;
	para_beta_hscore = 1000;
	para_gamma_hscore2 = 1000;
	
	para_clause_weight_threshold_swt = 300;
	para_p_scale_swt = 0.3;
	para_q_scale_swt = 0;

	para_smooth_probability_paws = 0.8;
}
*/

#endif
