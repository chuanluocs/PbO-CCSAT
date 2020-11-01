#ifndef _PARSE_ARGUMENTS_CPP
#define _PARSE_ARGUMENTS_CPP

#include "parse_arguments.h"

bool parse_arguments(int argc, char **argv)
{
	int i;
	bool flag_inst = false;
	bool flag_seed = false;
	bool flag_maxi_tries = false;
	bool flag_score2 = false;
	bool flag_age = false;
	bool flag_cscc = false;
	bool flag_break_score = false;
	
	default_algorithm_settings();
	
	for(i=1; i<argc; i++)
	{
		if(strcmp(argv[i], "-inst") == 0)
		{
			i++;
			if(i>=argc) return false;
			inst = argv[i];
			flag_inst = true;
			continue;
		}
		else if(strcmp(argv[i], "-seed") == 0)
		{
			i++;
			if(i>=argc) return false;
			sscanf(argv[i], "%d", &seed);
			flag_seed = true;
			continue;
		}
		else if(strcmp(argv[i], "-maxi_tries") == 0)
		{
			i++;
			if(i>=argc) return false;
			sscanf(argv[i], "%lld", &maxi_tries);
			flag_maxi_tries = true;
			continue;
		}
		else if(strcmp(argv[i], "-maxi_flips") == 0)
		{
			i++;
			if(i>=argc) return false;
			sscanf(argv[i], "%lld", &maxi_flips);
			continue;
		}
		else if(strcmp(argv[i], "-init_solution") == 0)
		{
			i++;
			if(i>=argc) return false;
			if(strcmp(argv[i], "1") == 0)
			{
				init_solution_ptr = init_solution_random;
				continue;
			}
			else if(strcmp(argv[i], "2") == 0)
			{
				init_solution_ptr = init_solution_all_true_or_false;
				continue;
			}
			else if(strcmp(argv[i], "3") == 0)
			{
				init_solution_ptr = init_solution_each_lit_true_or_false;
				continue;
			}
			else return false;
		}
		else if(strcmp(argv[i], "-perform_pac") == 0)
		{
			i++;
			if(i>=argc) return false;
			if(strcmp(argv[i], "0") == 0)
			{
				decision_perform_pac_heuristic = false;
				select_var_pac_heuristic_ptr = empty_function_ret_int;
				para_prob_pac = 0;
				continue;
			}
			else if(strcmp(argv[i], "1") == 0)
			{
				decision_perform_pac_heuristic = true;
				select_var_pac_heuristic_ptr = select_var_pac_heuristic;
				continue;
			}
			else return false;
		}
		else if(strcmp(argv[i], "-prob_pac") == 0)
		{
			i++;
			if(i>=argc) return false;
			sscanf(argv[i], "%lf", &para_prob_pac);
			continue;
		}
		else if(strcmp(argv[i], "-perform_first_div") == 0)
		{
			i++;
			if(i>=argc) return false;
			if(strcmp(argv[i], "0") == 0)
			{
				decision_perform_first_div = false;
				select_unsat_clause_heuristic_first_div_ptr = empty_function_ret_int;
				select_var_from_clause_heuristic_first_div_ptr = empty_function_ret_int;
				para_prob_first_div = 0;
				continue;
			}
			else if(strcmp(argv[i], "1") == 0)
			{
				decision_perform_first_div = true;
				continue;
			}
			else return false;
		}
		else if(strcmp(argv[i], "-prob_first_div") == 0)
		{
			i++;
			if(i>=argc) return false;
			sscanf(argv[i], "%lf", &para_prob_first_div);
			continue;
		}
		/*
		else if(strcmp(argv[i], "-sel_clause_first_div") == 0)
		{
			i++;
			if(i>=argc) return false;
			if(strcmp(argv[i], "1") == 0)
			{
				select_unsat_clause_heuristic_first_div_ptr = select_unsat_clause_random_heuristic;
				continue;
			}
			else if(strcmp(argv[i], "2") == 0)
			{
				select_unsat_clause_heuristic_first_div_ptr = select_unsat_clause_weight_distribution_heuristic;
				continue;
			}
			else return false;
		}
		else if(strcmp(argv[i], "-sel_var_first_div") == 0)
		{
			i++;
			if(i>=argc) return false;
			if(strcmp(argv[i], "1") == 0)
			{
				select_var_from_clause_heuristic_first_div_ptr = select_var_from_clause_random_heuristic;
				continue;
			}
			else if(strcmp(argv[i], "2") == 0)
			{
				flag_age = true;
				select_var_from_clause_heuristic_first_div_ptr = select_var_from_clause_greatest_age_heuristic;
				continue;
			}
			else if(strcmp(argv[i], "3") == 0)
			{
				select_var_from_clause_heuristic_first_div_ptr = select_var_from_clause_greatest_score_heuristic;
				continue;
			}
			else if(strcmp(argv[i], "4") == 0)
			{
				flag_score2 = true;
				select_var_from_clause_heuristic_first_div_ptr = select_var_from_clause_greatest_hscore_heuristic;
				continue;
			}
			else if(strcmp(argv[i], "5") == 0)
			{
				flag_score2 = true;
				select_var_from_clause_heuristic_first_div_ptr = select_var_from_clause_greatest_hscore2_heuristic;
				continue;
			}
			else if(strcmp(argv[i], "6") == 0)
			{
				flag_age = true;
				select_var_from_clause_heuristic_first_div_ptr = select_var_from_clause_novelty_heuristic;
				continue;
			}
			else if(strcmp(argv[i], "7") == 0)
			{
				flag_age = true;
				select_var_from_clause_heuristic_first_div_ptr = select_var_from_clause_sparrow_heuristic;
				continue;
			}
			else if(strcmp(argv[i], "8") == 0)
			{
				flag_break_score = true;
				select_var_from_clause_heuristic_first_div_ptr = select_var_from_clause_probsat_heuristic;
				continue;
			}
			else return false;
		}
		*/
		else if(strcmp(argv[i], "-perform_double_cc") == 0)
		{
			i++;
			if(i>=argc) return false;
			if(strcmp(argv[i], "0") == 0)
			{
				decision_perform_double_cc = false;
				select_var_cscc_heuristic_ptr = empty_function_ret_int;
				continue;
			}
			else if(strcmp(argv[i], "1") == 0)
			{
				decision_perform_double_cc = true;
				flag_cscc = true;
				select_var_cscc_heuristic_ptr = select_var_cscc_heuristic_from_nvccdvar_stack;
				continue;
			}
			else return false;
		}
		else if(strcmp(argv[i], "-perform_aspiration") == 0)
		{
			i++;
			if(i>=argc) return false;
			if(strcmp(argv[i], "0") == 0)
			{
				decision_perform_aspiration = false;
				select_var_aspiration_heuristic_ptr = empty_function_ret_int;
				continue;
			}
			else if(strcmp(argv[i], "1") == 0)
			{
				decision_perform_aspiration = true;
				select_var_aspiration_heuristic_ptr = select_var_aspiration_heuristic;
				continue;
			}
			else return false;
		}
		else if(strcmp(argv[i], "-sel_var_break_tie_greedy") == 0)
		{
			i++;
			if(i>=argc) return false;
			if(strcmp(argv[i], "1") == 0)
			{
				select_var_by_break_tie_in_set_ptr = select_var_by_random_break_tie_in_set;
				continue;
			}
			else if(strcmp(argv[i], "2") == 0)
			{
				flag_age = true;
				select_var_by_break_tie_in_set_ptr = select_var_by_greatest_age_break_tie_in_set;
				continue;
			}
			else if(strcmp(argv[i], "3") == 0)
			{
				flag_score2 = true;
				select_var_by_break_tie_in_set_ptr = select_var_by_greatest_hscore_break_tie_in_set;
				continue;
			}
			else if(strcmp(argv[i], "4") == 0)
			{
				flag_score2 = true;
				select_var_by_break_tie_in_set_ptr = select_var_by_greatest_hscore2_break_tie_in_set;
				continue;
			}
			else return false;
		}
		else if(strcmp(argv[i], "-d_hscore") == 0)
		{
			i++;
			if(i>=argc) return false;
			sscanf(argv[i], "%d", &para_d_hscore);
			continue;
		}
		else if(strcmp(argv[i], "-beta_hscore") == 0)
		{
			i++;
			if(i>=argc) return false;
			sscanf(argv[i], "%d", &para_beta_hscore);
			continue;
		}
		else if(strcmp(argv[i], "-gamma_hscore2") == 0)
		{
			i++;
			if(i>=argc) return false;
			sscanf(argv[i], "%d", &para_gamma_hscore2);
			continue;
		}
		else if(strcmp(argv[i], "-perform_clause_weight") == 0)
		{
			i++;
			if(i>=argc) return false;
			if(strcmp(argv[i], "0") == 0)
			{
				decision_perform_clause_weight_scheme = false;
				init_clause_weight_scheme_ptr = empty_function_ret_void;
				clause_weight_scheme_ptr = empty_function_ret_void;
				para_clause_weight_threshold_swt = 0;
				para_p_scale_swt = 0;
				para_q_scale_swt = 0;
				para_smooth_probability_paws = 0;
				continue;
			}
			else if(strcmp(argv[i], "1") == 0)
			{
				decision_perform_clause_weight_scheme = true;
				continue;
			}
			else return false;
		}
		else if(strcmp(argv[i], "-sel_clause_weight_scheme") == 0)
		{
			i++;
			if(i>=argc) return false;
			if(strcmp(argv[i], "1") == 0)
			{
				init_clause_weight_scheme_ptr = init_clause_weight_scheme_swt;
				clause_weight_scheme_ptr = clause_weight_scheme_swt;
				continue;
			}
			else if(strcmp(argv[i], "2") == 0)
			{
				init_clause_weight_scheme_ptr = init_clause_weight_scheme_paws;
				clause_weight_scheme_ptr = clause_weight_scheme_paws;
				continue;
			}
			else return false;
		}
		else if(strcmp(argv[i], "-threshold_swt") == 0)
		{
			i++;
			if(i>=argc) return false;
			sscanf(argv[i], "%lld", &para_clause_weight_threshold_swt);
			continue;
		}
		else if(strcmp(argv[i], "-p_swt") == 0)
		{
			i++;
			if(i>=argc) return false;
			sscanf(argv[i], "%lf", &para_p_scale_swt);
			continue;
		}
		else if(strcmp(argv[i], "-q_swt") == 0)
		{
			i++;
			if(i>=argc) return false;
			sscanf(argv[i], "%lf", &para_q_scale_swt);
			continue;
		}
		else if(strcmp(argv[i], "-sp_paws") == 0)
		{
			i++;
			if(i>=argc) return false;
			sscanf(argv[i], "%lf", &para_smooth_probability_paws);
			continue;
		}
		else if(strcmp(argv[i], "-sel_clause_div") == 0)
		{
			i++;
			if(i>=argc) return false;
			if(strcmp(argv[i], "1") == 0)
			{
				select_unsat_clause_heuristic_div_ptr = select_unsat_clause_random_heuristic;
				select_unsat_clause_heuristic_first_div_ptr = select_unsat_clause_random_heuristic;
				continue;
			}
			else if(strcmp(argv[i], "2") == 0)
			{
				select_unsat_clause_heuristic_div_ptr = select_unsat_clause_weight_distribution_heuristic;
				select_unsat_clause_heuristic_first_div_ptr = select_unsat_clause_weight_distribution_heuristic;
				continue;
			}
			else return false;
		}
		else if(strcmp(argv[i], "-sel_var_div") == 0)
		{
			i++;
			if(i>=argc) return false;
			if(strcmp(argv[i], "1") == 0)
			{
				select_var_from_clause_heuristic_div_ptr = select_var_from_clause_random_heuristic;
				select_var_from_clause_heuristic_first_div_ptr = select_var_from_clause_random_heuristic;
				continue;
			}
			else if(strcmp(argv[i], "2") == 0)
			{
				flag_age = true;
				select_var_from_clause_heuristic_div_ptr = select_var_from_clause_greatest_age_heuristic;
				select_var_from_clause_heuristic_first_div_ptr = select_var_from_clause_greatest_age_heuristic;
				continue;
			}
			else if(strcmp(argv[i], "3") == 0)
			{
				select_var_from_clause_heuristic_div_ptr = select_var_from_clause_greatest_score_heuristic;
				select_var_from_clause_heuristic_first_div_ptr = select_var_from_clause_greatest_score_heuristic;
				continue;
			}
			else if(strcmp(argv[i], "4") == 0)
			{
				flag_score2 = true;
				select_var_from_clause_heuristic_div_ptr = select_var_from_clause_greatest_hscore_heuristic;
				select_var_from_clause_heuristic_first_div_ptr = select_var_from_clause_greatest_hscore_heuristic;
				continue;
			}
			else if(strcmp(argv[i], "5") == 0)
			{
				flag_score2 = true;
				select_var_from_clause_heuristic_div_ptr = select_var_from_clause_greatest_hscore2_heuristic;
				select_var_from_clause_heuristic_first_div_ptr = select_var_from_clause_greatest_hscore2_heuristic;
				continue;
			}
			else if(strcmp(argv[i], "6") == 0)
			{
				flag_age = true;
				select_var_from_clause_heuristic_div_ptr = select_var_from_clause_novelty_heuristic;
				select_var_from_clause_heuristic_first_div_ptr = select_var_from_clause_novelty_heuristic;
				continue;
			}
			else if(strcmp(argv[i], "7") == 0)
			{
				flag_age = true;
				select_var_from_clause_heuristic_div_ptr = select_var_from_clause_sparrow_heuristic;
				select_var_from_clause_heuristic_first_div_ptr = select_var_from_clause_sparrow_heuristic;
				continue;
			}
			else if(strcmp(argv[i], "8") == 0)
			{
				flag_break_score = true;
				select_var_from_clause_heuristic_div_ptr = select_var_from_clause_probsat_heuristic;
				select_var_from_clause_heuristic_first_div_ptr = select_var_from_clause_probsat_heuristic;
				continue;
			}
			else return false;
		}
		else if(strcmp(argv[i], "-prob_novelty") == 0)
		{
			i++;
			if(i>=argc) return false;
			sscanf(argv[i], "%lf", &para_prob_novelty);
			continue;
		}
		else if(strcmp(argv[i], "-sparrow_c1") == 0)
		{
			i++;
			if(i>=argc) return false;
			sscanf(argv[i], "%lf", &para_sparrow_c1);
			continue;
		}
		else if(strcmp(argv[i], "-sparrow_c2") == 0)
		{
			i++;
			if(i>=argc) return false;
			sscanf(argv[i], "%d", &para_sparrow_c2);
			continue;
		}
		else if(strcmp(argv[i], "-sparrow_c3") == 0)
		{
			i++;
			if(i>=argc) return false;
			sscanf(argv[i], "%d", &para_sparrow_c3);
			continue;
		}
		else if(strcmp(argv[i], "-probsat_cb") == 0)
		{
			i++;
			if(i>=argc) return false;
			sscanf(argv[i], "%lf", &para_probsat_cb);
			continue;
		}
		else return false;
	}
	
	if(flag_score2)
	{
		init_sat_var2_ptr = init_sat_var2;
		update_sat_var2_ptr = update_sat_var2;
		init_score2_ptr = init_score2;
		update_score2_ptr = update_score2;
	}
	
	if(flag_age)
	{
		init_time_stamp_ptr = init_time_stamp;
		update_time_stamp_ptr = update_time_stamp;
	}
	
	if(flag_cscc)
	{
		init_cscc_conf_change_ptr = init_cscc_conf_change;
		update_cscc_conf_change_ptr = update_cscc_conf_change;
	}
	
	init_unsatvar_related_ptr = init_unsatvar_related;
	update_unsatvar_related_in_unsat_function_ptr = update_unsatvar_related_in_unsat_function;
	update_unsatvar_related_in_sat_function_ptr = update_unsatvar_related_in_sat_function;	
	
	init_nvccdvar_related_ptr = init_nvccdvar_related;
	update_nvccdvar_related_in_update_function_ptr = update_nvccdvar_related_in_update_function;
	update_nvccdvar_related_in_swt_scheme_ptr = update_nvccdvar_related_in_swt_scheme;
	update_nvccdvar_related_in_dec_paws_scheme_ptr = update_nvccdvar_related_in_dec_paws_scheme;
	update_nvccdvar_related_in_inc_paws_scheme_ptr = update_nvccdvar_related_in_inc_paws_scheme;
	
	init_score_ptr = init_score;
	update_score_ptr = update_score;
	
	if(flag_break_score)
	{
		init_break_score_ptr = init_break_score;
		update_break_score_ptr = update_break_score;
	}
	else
	{
		init_break_score_ptr = empty_function_ret_void;
		update_break_score_ptr = empty_function_ret_void;
	}
	
	init_make_score_ptr = empty_function_ret_void;
	update_make_score_ptr = empty_function_ret_void;
	
	build_neighbor_relation_ptr = build_neighbor_relation;
	init_nvcc_conf_change_ptr = init_nvcc_conf_change;
	update_nvcc_conf_change_ptr = update_nvcc_conf_change;
	
	select_var_nvcc_heuristic_ptr = select_var_nvcc_heuristic_from_nvccdvar_stack;
	
	if(!flag_maxi_tries) maxi_tries = default_maxi_tries_flips_num;
	
	if(flag_inst && flag_seed) return true;
	else return false;
}

#endif
