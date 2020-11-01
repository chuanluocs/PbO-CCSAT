#ifndef _INFO_CPP
#define _INFO_CPP

#include "info.h"

string version_str = "1.2.3";

string get_last_level_name(string file_path)
{
	long long file_path_size = file_path.size();
	if(file_path[file_path_size-1] == '/')
		file_path = file_path.substr(0, file_path_size-1);
	long long pos = file_path.find_last_of('/');
	if(pos<0 || pos>=file_path.size())
		return file_path;
	else return file_path.substr(pos+1, file_path.size()-pos-1);	
}

void print_general_information(char *executable_name)
{
	string executable_name_str = executable_name;
	executable_name_str = get_last_level_name(executable_name_str);
	cout << "c" << endl;
	cout << "c This is " << executable_name_str << " (a local_search SAT solver)" << endl;
	cout << "c Version: " << version_str << endl;
	cout << "c" << endl;
	return;
}

int get_default_init_solution()
{
	if(init_solution_ptr == init_solution_random)
		return 1;
	else if(init_solution_ptr == init_solution_all_true_or_false)
		return 2;
	else if(init_solution_ptr == init_solution_each_lit_true_or_false)
		return 3;
	else return 1;
}

int get_default_sel_clause_first_div()
{
	if(!decision_perform_first_div)
		return 1;
	else
	{
		if(select_unsat_clause_heuristic_first_div_ptr == select_unsat_clause_random_heuristic)
			return 1;
		else if(select_unsat_clause_heuristic_first_div_ptr == select_unsat_clause_weight_distribution_heuristic)
			return 2;
		else return 1;
	}
}

int get_default_sel_var_first_div()
{
	if(!decision_perform_first_div)
		return 1;
	else
	{
		if(select_var_from_clause_heuristic_first_div_ptr == select_var_from_clause_random_heuristic)
			return 1;
		else if(select_var_from_clause_heuristic_first_div_ptr == select_var_from_clause_greatest_age_heuristic)
			return 2;
		else if(select_var_from_clause_heuristic_first_div_ptr == select_var_from_clause_greatest_score_heuristic)
			return 3;
		else if(select_var_from_clause_heuristic_first_div_ptr == select_var_from_clause_greatest_hscore_heuristic)
			return 4;
		else if(select_var_from_clause_heuristic_first_div_ptr == select_var_from_clause_greatest_hscore2_heuristic)
			return 5;
		else if(select_var_from_clause_heuristic_first_div_ptr == select_var_from_clause_novelty_heuristic)
			return 6;
		else if(select_var_from_clause_heuristic_first_div_ptr == select_var_from_clause_sparrow_heuristic)
			return 7;
		else if(select_var_from_clause_heuristic_first_div_ptr == select_var_from_clause_probsat_heuristic)
			return 8;
		else return 1;
	}
}

int get_default_sel_var_break_tie_greedy()
{
	if(select_var_by_break_tie_in_set_ptr == select_var_by_random_break_tie_in_set)
		return 1;
	else if(select_var_by_break_tie_in_set_ptr == select_var_by_greatest_age_break_tie_in_set)
		return 2;
	else if(select_var_by_break_tie_in_set_ptr == select_var_by_greatest_hscore_break_tie_in_set)
		return 3;
	else if(select_var_by_break_tie_in_set_ptr == select_var_by_greatest_hscore2_break_tie_in_set)
		return 4;
	else return 1;
}

int get_default_sel_clause_weight_scheme()
{
	if(!decision_perform_clause_weight_scheme)
		return 1;
	else
	{
		if(init_clause_weight_scheme_ptr == init_clause_weight_scheme_swt)
			return 1;
		else if(init_clause_weight_scheme_ptr == init_clause_weight_scheme_paws)
			return 2;
		else return 1;
	}
}

int get_default_sel_clause_div()
{
	if(select_unsat_clause_heuristic_div_ptr == select_unsat_clause_random_heuristic)
		return 1;
	else if(select_unsat_clause_heuristic_div_ptr == select_unsat_clause_weight_distribution_heuristic)
		return 2;
	else return 1;
}

int get_default_sel_var_div()
{
	if(select_var_from_clause_heuristic_div_ptr == select_var_from_clause_random_heuristic)
		return 1;
	else if(select_var_from_clause_heuristic_div_ptr == select_var_from_clause_greatest_age_heuristic)
		return 2;
	else if(select_var_from_clause_heuristic_div_ptr == select_var_from_clause_greatest_score_heuristic)
		return 3;
	else if(select_var_from_clause_heuristic_div_ptr == select_var_from_clause_greatest_hscore_heuristic)
		return 4;
	else if(select_var_from_clause_heuristic_div_ptr == select_var_from_clause_greatest_hscore2_heuristic)
		return 5;
	else if(select_var_from_clause_heuristic_div_ptr == select_var_from_clause_novelty_heuristic)
		return 6;
	else if(select_var_from_clause_heuristic_div_ptr == select_var_from_clause_sparrow_heuristic)
		return 7;
	else if(select_var_from_clause_heuristic_div_ptr == select_var_from_clause_probsat_heuristic)
		return 8;
	else return 1;
}

void print_usage_info(char *executable_name)
{
	default_algorithm_settings();
	cout << "c" << endl;
	cout << "c Usage: " <<executable_name << "   (*Required*)" <<endl
		 << "c -inst <instance_name>   (*Required*)" <<endl
		 << "c -seed <seed>   (*Required*)" <<endl
		 << "c -maxi_flips <maxi_flips>   (Default: " << maxi_flips << ")" <<endl
		 << "c -init_solution {1, 2, 3}   (Default: " << get_default_init_solution() << ")" << endl
		 << "c -perform_pac {0, 1}   (Default: " << (int) decision_perform_pac_heuristic << ")"<< endl
		 << "c -prob_pac [0, 1]   (Default: " << para_prob_pac << ")" << endl
		 << "c -perform_first_div {0, 1}   (Default: " << (int) decision_perform_first_div << ")" << endl
		 << "c -prob_first_div [0, 1]   (Default: " << para_prob_first_div << ")" << endl
		 //<< "c -sel_clause_first_div {1, 2}   (Default: " << get_default_sel_clause_first_div() << ")" << endl
		 //<< "c -sel_var_first_div {1, 2, 3, 4, 5, 6, 7, 8}   (Default: " << get_default_sel_var_first_div() << ")" << endl
		 << "c -perform_double_cc {0, 1}   (Default: " << (int) decision_perform_double_cc << ")" << endl
		 << "c -perform_aspiration {0, 1}   (Default: " << (int) decision_perform_aspiration << ")" << endl
		 << "c -sel_var_break_tie_greedy {1, 2, 3, 4}   (Default: " << get_default_sel_var_break_tie_greedy() << ")" << endl
		 << "c -d_hscore {int, from 1 to 15}   (Default: " << para_d_hscore << ")" << endl
		 << "c -beta_hscore {int, from 100 to 1000000}   (Default: " << para_beta_hscore << ")" << endl
		 << "c -gamma_hscore2 {int, from 100 to 1000000}   (Default: " << para_gamma_hscore2 << ")" << endl
		 << "c -perform_clause_weight {0, 1}   (Default: " << (int) decision_perform_clause_weight_scheme << ")" << endl
		 << "c -sel_clause_weight_scheme {1, 2}   (Default: " << get_default_sel_clause_weight_scheme() << ")" << endl
		 << "c -threshold_swt {int, from 10 to 1000}   (Default: " << para_clause_weight_threshold_swt << ")" << endl
		 << "c -p_swt [0, 1]   (Default: " << para_p_scale_swt << ")" << endl
		 << "c -q_swt [0, 1]   (Default: " << para_q_scale_swt << ")" << endl
		 << "c -sp_paws [0, 1]   (Default: " << para_smooth_probability_paws << ")" << endl
		 << "c -sel_clause_div {1, 2}   (Default: " << get_default_sel_clause_div() << ")" << endl
		 << "c -sel_var_div {1, 2, 3, 4, 5, 6, 7, 8}   (Default: " << get_default_sel_var_div() << ")" << endl
		 << "c -prob_novelty [0, 1]   (Default: " << para_prob_novelty << ")" << endl
		 << "c -sparrow_c1 [2, 10]   (Default: " << para_sparrow_c1 << ")" << endl
		 << "c -sparrow_c2 {int, from 1 to 5}   (Default: " << para_sparrow_c2 << ")" << endl
		 << "c -sparrow_c3 {int, from 20000 to 100000}   (Default: " << para_sparrow_c3 << ")" << endl
		 << "c -probsat_cb [2, 10]   (Default: " << para_probsat_cb << ")" << endl;
	cout << "c" << endl;
	return;
}

void print_algorithm_settings_information()
{
	cout << "c" << endl;
	cout << "c instance = " << inst << endl;
	cout << "c ori_num_vars = " << ori_num_vars << endl;
	cout << "c ori_num_clauses = " << ori_num_clauses << endl;
	cout << "c maxi_clause_len = " << maxi_clause_len << endl;
	cout << "c mini_clause_len = " << mini_clause_len << endl;
	cout << "c seed = " << seed << endl;
	cout << "c maxi_flips = " << maxi_flips << endl;
	
	if(init_solution_ptr == init_solution_random)
		cout << "c init_solution_ptr = init_solution_random" << endl;
	else if(init_solution_ptr == init_solution_all_true_or_false)
		cout << "c init_solution_ptr = init_solution_all_true_or_false" << endl;
	else if(init_solution_ptr == init_solution_each_lit_true_or_false)
		cout << "c init_solution_ptr = init_solution_each_lit_true_or_false" << endl;
	
	if(decision_perform_pac_heuristic)
	{
		cout << "c decision_perform_pac_heuristic = true" << endl;
		if(select_var_pac_heuristic_ptr == select_var_pac_heuristic)
			cout<< "c select_var_pac_heuristic_ptr = select_var_pac_heuristic" << endl;
		cout << "c prob_pac = " << para_prob_pac << endl;
	}
	
	if(decision_perform_first_div)
	{
		cout << "c decision_perform_first_div = true" << endl;
		/*
		if(select_unsat_clause_heuristic_first_div_ptr == select_unsat_clause_random_heuristic)
			cout << "c select_unsat_clause_heuristic_first_div_ptr = select_unsat_clause_random_heuristic" << endl;
		else if(select_unsat_clause_heuristic_first_div_ptr == select_unsat_clause_weight_distribution_heuristic)
			cout << "c select_unsat_clause_heuristic_first_div_ptr = select_unsat_clause_weight_distribution_heuristic" << endl;
		
		if(select_var_from_clause_heuristic_first_div_ptr == select_var_from_clause_random_heuristic)
			cout << "c select_var_from_clause_heuristic_first_div_ptr = select_var_from_clause_random_heuristic" << endl;
		else if(select_var_from_clause_heuristic_first_div_ptr == select_var_from_clause_greatest_age_heuristic)
			cout << "c select_var_from_clause_heuristic_first_div_ptr = select_var_from_clause_greatest_age_heuristic" << endl;
		else if(select_var_from_clause_heuristic_first_div_ptr == select_var_from_clause_greatest_score_heuristic)
			cout << "c select_var_from_clause_heuristic_first_div_ptr = select_var_from_clause_greatest_score_heuristic" << endl;
		else if(select_var_from_clause_heuristic_first_div_ptr == select_var_from_clause_novelty_heuristic)
		{
			cout << "c select_var_from_clause_heuristic_first_div_ptr = select_var_from_clause_novelty_heuristic" << endl;
			cout << "c prob_novelty = " << para_prob_novelty << endl;
		}
		else if(select_var_from_clause_heuristic_first_div_ptr == select_var_from_clause_sparrow_heuristic)
		{
			cout << "c select_var_from_clause_heuristic_first_div_ptr = select_var_from_clause_sparrow_heuristic" << endl;
			cout << "c sparrow_c1 = " << para_sparrow_c1 << endl;
			cout << "c sparrow_c2 = " << para_sparrow_c2 << endl;
			cout << "c sparrow_c3 = " << para_sparrow_c3 << endl;
		}
		else if(select_var_from_clause_heuristic_first_div_ptr == select_var_from_clause_probsat_heuristic)
		{
			cout << "c select_var_from_clause_heuristic_first_div_ptr = select_var_from_clause_probsat_heuristic" << endl;
			cout << "c probsat_cb = " << para_probsat_cb << endl;
		}
		*/
		cout << "c prob_first_div = " << para_prob_first_div << endl;
	}
	
	if(decision_perform_double_cc)
	{
		cout << "c decision_perform_double_cc = true" << endl;
	}
	
	if(decision_perform_aspiration)
	{
		cout << "c decision_perform_aspiration = true" << endl;
	}
	
	if(select_var_by_break_tie_in_set_ptr == select_var_by_random_break_tie_in_set)
		cout << "c select_var_by_break_tie_in_set_ptr = select_var_by_random_break_tie_in_set" << endl;
	else if(select_var_by_break_tie_in_set_ptr == select_var_by_greatest_age_break_tie_in_set)
		cout << "c select_var_by_break_tie_in_set_ptr = select_var_by_greatest_age_break_tie_in_set" << endl;
	else if(select_var_by_break_tie_in_set_ptr == select_var_by_greatest_hscore_break_tie_in_set)
	{
		cout << "c select_var_by_break_tie_in_set_ptr = select_var_by_greatest_hscore_break_tie_in_set" << endl;
		cout << "c d_hscore = " << para_d_hscore << endl;
		cout << "c beta_hscore = " << para_beta_hscore << endl;
	}
	else if(select_var_by_break_tie_in_set_ptr == select_var_by_greatest_hscore2_break_tie_in_set)
	{
		cout << "c select_var_by_break_tie_in_set_ptr = select_var_by_greatest_hscore2_break_tie_in_set" << endl; 
		cout << "c gamma_hscore2 = " << para_gamma_hscore2 << endl;
	}
	
	if(decision_perform_clause_weight_scheme)
	{
		cout << "c decision_perform_clause_weight_scheme = true" << endl;
		if(init_clause_weight_scheme_ptr == init_clause_weight_scheme_swt)
		{
			cout << "c init_clause_weight_scheme_ptr = init_clause_weight_scheme_swt" << endl;
			cout << "c clause_weight_scheme_ptr = clause_weight_scheme_swt" << endl;
			cout << "c threshold_swt = " << para_clause_weight_threshold_swt << endl;
			cout << "c p_swt = " << para_p_scale_swt << endl;
			cout << "c q_swt = " << para_q_scale_swt << endl;
		}
		else if(init_clause_weight_scheme_ptr == init_clause_weight_scheme_paws)
		{
			cout << "c init_clause_weight_scheme_ptr = init_clause_weight_scheme_paws" << endl;
			cout << "c clause_weight_scheme_ptr = clause_weight_scheme_paws" << endl;
			cout << "c sp_paws = " << para_smooth_probability_paws << endl;
		}
	}
	
	if(select_unsat_clause_heuristic_div_ptr == select_unsat_clause_random_heuristic)
		cout << "c select_unsat_clause_heuristic_div_ptr = select_unsat_clause_random_heuristic" << endl;
	else if(select_unsat_clause_heuristic_div_ptr == select_unsat_clause_weight_distribution_heuristic)
		cout << "c select_unsat_clause_heuristic_div_ptr = select_unsat_clause_weight_distribution_heuristic" << endl;
	
	if(select_var_from_clause_heuristic_div_ptr == select_var_from_clause_random_heuristic)
		cout << "c select_var_from_clause_heuristic_div_ptr = select_var_from_clause_random_heuristic" << endl;
	else if(select_var_from_clause_heuristic_div_ptr == select_var_from_clause_greatest_age_heuristic)
		cout << "c select_var_from_clause_heuristic_div_ptr = select_var_from_clause_greatest_age_heuristic" << endl;
	else if(select_var_from_clause_heuristic_div_ptr == select_var_from_clause_greatest_score_heuristic)
		cout << "c select_var_from_clause_heuristic_div_ptr = select_var_from_clause_greatest_score_heuristic" << endl;
	else if(select_var_from_clause_heuristic_div_ptr == select_var_from_clause_greatest_hscore_heuristic)
	{
		cout << "c select_var_from_clause_heuristic_div_ptr = select_var_from_clause_greatest_hscore_heuristic" << endl;
		cout << "c d_hscore = " << para_d_hscore << endl;
		cout << "c beta_hscore = " << para_beta_hscore << endl;
	}
	else if(select_var_from_clause_heuristic_div_ptr == select_var_from_clause_greatest_hscore2_heuristic)
	{
		cout << "c select_var_from_clause_heuristic_div_ptr = select_var_from_clause_greatest_hscore2_heuristic" << endl;
		cout << "c gamma_hscore2 = " << para_gamma_hscore2 << endl;
	}
	else if(select_var_from_clause_heuristic_div_ptr == select_var_from_clause_novelty_heuristic)
	{
		cout << "c select_var_from_clause_heuristic_div_ptr = select_var_from_clause_novelty_heuristic" << endl;
		cout << "c prob_novelty = " << para_prob_novelty << endl;
	}
	else if(select_var_from_clause_heuristic_div_ptr == select_var_from_clause_sparrow_heuristic)
	{
		cout << "c select_var_from_clause_heuristic_div_ptr = select_var_from_clause_sparrow_heuristic" << endl;
		cout << "c sparrow_c1 = " << para_sparrow_c1 << endl;
		cout << "c sparrow_c2 = " << para_sparrow_c2 << endl;
		cout << "c sparrow_c3 = " << para_sparrow_c3 << endl;
	}
	else if(select_var_from_clause_heuristic_div_ptr == select_var_from_clause_probsat_heuristic)
	{
		cout << "c select_var_from_clause_heuristic_div_ptr = select_var_from_clause_probsat_heuristic" << endl;
		cout << "c probsat_cb = " << para_probsat_cb << endl;
	}
	
	cout << "c" << endl;
	return;
}

void print_solution(int *cur_soln, int num_vars)
{
	int v;
	
	cout<<"v";
	for(v=1; v<=num_vars; v++)
	{
		cout<<" ";
		if(cur_soln[v]==0) cout<<"-";
		cout<<v;
	}
	cout<<endl;
	return;
}

void print_SAT_result(int *cur_soln, int num_vars)
{
	cout<<"s SATISFIABLE"<<endl;
	print_solution(cur_soln, num_vars);
	return;
}

void print_UNKNOWN_result()
{
	cout<<"s UNKNOWN"<<endl;
	return;
}

void print_run_info(double run_time, long long tries, long long step)
{
	cout<<"c run time = "<<run_time<<" second(s)"<<endl;
	cout<<"c total steps = " << step << " + (" << tries <<" * " << maxi_flips << ")" << endl;
	return;
}

#endif
