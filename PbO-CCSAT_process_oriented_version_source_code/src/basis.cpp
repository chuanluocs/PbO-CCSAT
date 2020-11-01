#ifndef _BASIS_CPP
#define _BASIS_CPP

#include "basis.h"

Mersenne		rng;
int 			seed;
string 			inst;

int				ori_num_vars;
int				ori_num_clauses;

int 			num_vars;
int 			num_clauses;
int 			maxi_clause_len;
int 			mini_clause_len;

int				all_pos_lit_count;
int				all_neg_lit_count;
int*			var_pos_lit_count;
int*			var_neg_lit_count;

lit**			var_lit;
int*			var_lit_count;
lit**			clause_lit;
int*			clause_lit_count;

int** 			var_neighbor;
int* 			var_neighbor_count;

long long*		score;
long long*		break_score;
long long*		make_score;
long long*		time_stamp;
int*			nvcc_conf_change;
int*			cscc_conf_change;
long long*		score2;
int*			candidate_var_stack;
int				candidate_var_stack_fill_pointer;
int*			nvccdvar_stack;
int				nvccdvar_stack_fill_pointer;
bool*			already_in_nvccdvar_stack;
double*			var_prob_value;

int*			unsatvar_stack;		
int				unsatvar_stack_fill_pointer;
int*			index_in_unsatvar_stack;
int*			unsatvar_app_count;

long long* 		clause_weight;
int*			sat_count;
int*			sat_var;
int*			sat_var2;

int* 			unsat_stack;
int 			unsat_stack_fill_pointer;
int*			index_in_unsat_stack;

long long		total_unsat_clause_weight;

int*			cur_soln;
long long		best_unsat_stack_fill_pointer;

const long long default_maxi_tries_flips_num = 4500000000000000000ll;
long long		maxi_tries = default_maxi_tries_flips_num;
long long   	maxi_flips = default_maxi_tries_flips_num;
long long 		tries;
long long 		step;

const long long maximum_clause_weight_limit = 2147483646ll;


bool			decision_perform_pac_heuristic;
bool			decision_perform_first_div;
bool			decision_perform_double_cc;
bool			decision_perform_aspiration;
bool			decision_perform_clause_weight_scheme;

double			para_prob_pac;
double			para_prob_first_div;
double 			para_prob_novelty;

int 			para_d_hscore;
int 			para_beta_hscore;
int 			para_gamma_hscore2;

long long		avg_clause_weight;
long long		delta_total_clause_weight;
long long		scale_avg;
long long		para_clause_weight_threshold_swt;
double			para_p_scale_swt;
double			para_q_scale_swt;

double			para_smooth_probability_paws;
int				large_clause_weight_count_threshold;
int*			large_weight_clauses;
int				large_weight_clauses_count;

double			para_sparrow_c1;
int				para_sparrow_c2;
int				para_sparrow_c3;

double			para_probsat_cb;


void reassign_istringstream(istringstream &iss, string line)
{
	iss.clear();
	iss.str(line);
	iss.seekg(0, ios::beg);
	return;
}

void allocate_memory(int num_vars, int num_clauses)
{
	int malloc_adding_length = 10;
	int malloc_var_length = num_vars+malloc_adding_length;
	int malloc_clause_length = num_clauses+malloc_adding_length;
	
	//allocate memory for those data structures with regards to num_vars
	var_lit = new lit* [malloc_var_length];
	var_lit_count = new int [malloc_var_length];
	var_pos_lit_count = new int [malloc_var_length];
	var_neg_lit_count = new int [malloc_var_length];
	var_neighbor = new int* [malloc_var_length];
	var_neighbor_count = new int [malloc_var_length];
	cur_soln = new int [malloc_var_length];
	score = new long long [malloc_var_length];
	break_score = new long long [malloc_var_length];
	make_score = new long long [malloc_var_length];
	time_stamp = new long long [malloc_var_length];
	nvcc_conf_change = new int [malloc_var_length];
	cscc_conf_change = new int [malloc_var_length];
	score2 = new long long [malloc_var_length];
	unsatvar_stack = new int [malloc_var_length];
	index_in_unsatvar_stack = new int [malloc_var_length];
	unsatvar_app_count = new int [malloc_var_length];
	candidate_var_stack = new int [malloc_var_length];
	nvccdvar_stack = new int [malloc_var_length];
	already_in_nvccdvar_stack = new bool [malloc_var_length];
	var_prob_value = new double [malloc_var_length];
	
	//allocate memory for those data structures with regards to num_clauses
	clause_lit = new lit* [malloc_clause_length];
	clause_lit_count = new int [malloc_clause_length];
	clause_weight = new long long [malloc_clause_length];
	sat_count = new int [malloc_clause_length];
	sat_var = new int [malloc_clause_length];
	unsat_stack = new int [malloc_clause_length];
	index_in_unsat_stack = new int [malloc_clause_length];
	sat_var2 = new int [malloc_clause_length];
	large_weight_clauses = new int [malloc_clause_length];
	
	return;
}

void free_memory(int num_vars, int num_clauses)
{
	int v, c;

	//free memory for those data structures with regards to num_vars
	for(v=1; v<=num_vars; v++)
	{
		delete [] var_lit[v];
		delete [] var_neighbor[v];
	}
	delete [] var_lit;
	delete [] var_lit_count;
	delete [] var_pos_lit_count;
	delete [] var_neg_lit_count;
	delete [] var_neighbor;
	delete [] var_neighbor_count;
	delete [] cur_soln;
	delete [] score;
	delete [] break_score;
	delete [] make_score;
	delete [] time_stamp;
	delete [] nvcc_conf_change;
	delete [] cscc_conf_change;
	delete [] score2;
	delete [] unsatvar_stack;
	delete [] index_in_unsatvar_stack;
	delete [] unsatvar_app_count;
	delete [] candidate_var_stack;
	delete [] nvccdvar_stack;
	delete [] already_in_nvccdvar_stack;
	delete [] var_prob_value;
	
	//free memory for those data structures with regards to num_clauses
	for(c=0; c<num_clauses; c++)
	{
		delete [] clause_lit[c];
	}
	delete [] clause_lit;
	delete [] clause_lit_count;
	delete [] clause_weight;
	delete [] sat_count;
	delete [] sat_var;
	delete [] unsat_stack;
	delete [] index_in_unsat_stack;
	delete [] sat_var2;
	delete [] large_weight_clauses;
	
	return;
}

bool build_instance(string inst)
{
	string line;
	istringstream iss;
	int c, v, i;
	int cur_lit;
	bool lit_redundent, clause_redundent;
	string tempstr1, tempstr2;
	vector<int> temp_clause;
	
	ifstream fin(inst.c_str());
	if(!fin.is_open()) return false;
	
	num_vars = 0;
	num_clauses = 0;
	
	while(getline(fin, line))
	{
		if(line.substr(0, 1) == "c") continue;
		else if(line.substr(0, 1) == "p")
		{
			reassign_istringstream(iss, line);
			iss >> tempstr1 >> tempstr2 >> num_vars >> num_clauses;
			ori_num_vars = num_vars;
			ori_num_clauses = num_clauses;
			break;
		}
	}
	
	allocate_memory(ori_num_vars, ori_num_clauses); //allocate memory for data structures
	
	for (v=1; v<=num_vars; v++)
	{
		var_lit_count[v] = 0;
		var_pos_lit_count[v] = 0;
		var_neg_lit_count[v] = 0;
	}
	for (c=0; c<num_clauses; c++) clause_lit_count[c] = 0;
	
	maxi_clause_len = -1;
	mini_clause_len = -1;
	
	all_pos_lit_count = 0;
	all_neg_lit_count = 0;
	
	for(c=0; c<num_clauses; )
	{
		clause_redundent = false;
		temp_clause.clear();
		vector<int>().swap(temp_clause);
		
		fin>>cur_lit;
		while(cur_lit!=0) 
		{
			if(!clause_redundent)
			{
				lit_redundent = false;
				for(i=0; i<temp_clause.size(); i++)
				{
					if(cur_lit == temp_clause[i])
					{
						lit_redundent = true;
						break;
					}
					else if(cur_lit == -temp_clause[i])
					{
						clause_redundent = true;
						break;
					}
				}
			
				if(!lit_redundent)
				{
					temp_clause.push_back(cur_lit);
				}
			}
			fin>>cur_lit;
		}
		
		if(!clause_redundent)
		{
			clause_lit_count[c] = temp_clause.size();
			clause_lit[c] = new lit [clause_lit_count[c]+1];
			
			for(i=0; i<temp_clause.size(); i++)
			{
				clause_lit[c][i].clause_num = c;
				clause_lit[c][i].var_num = abs(temp_clause[i]);
				if(temp_clause[i]>0)
				{
					clause_lit[c][i].sense = 1;
					var_pos_lit_count[clause_lit[c][i].var_num]++;
					all_pos_lit_count++;
				}
				else
				{
					clause_lit[c][i].sense = 0;
					var_neg_lit_count[clause_lit[c][i].var_num]++;
					all_neg_lit_count++;
				}
				
				var_lit_count[clause_lit[c][i].var_num]++;
			}
			
			clause_lit[c][clause_lit_count[c]].var_num=0;
			clause_lit[c][clause_lit_count[c]].clause_num=-1;
			
			if(maxi_clause_len == -1) maxi_clause_len = clause_lit_count[c];
			else if(maxi_clause_len<clause_lit_count[c]) maxi_clause_len = clause_lit_count[c];
			
			if(mini_clause_len == -1) mini_clause_len = clause_lit_count[c];
			else if(mini_clause_len>clause_lit_count[c]) mini_clause_len = clause_lit_count[c];
			
			c++;
		}
		else
		{
			num_clauses--;
			clause_lit_count[c] = 0;
			temp_clause.clear();
			vector<int>().swap(temp_clause);
		}
		
	}
	
	fin.close();
	
	for (v=1; v<=num_vars; v++)
	{
		var_lit[v] = new lit[var_lit_count[v]+1];
		var_lit[v][var_lit_count[v]].var_num = 0;
		var_lit[v][var_lit_count[v]].clause_num=-1;
		var_lit_count[v] = 0;	//reset to 0, for building up the array
	}
	
	for (c=0; c<num_clauses; c++) 
	{
		for(i=0; i<clause_lit_count[c]; ++i)
		{
			v = clause_lit[c][i].var_num;
			var_lit[v][var_lit_count[v]] = clause_lit[c][i];
			var_lit_count[v]++;
		}
	}
	
	build_neighbor_relation_ptr();
	
	return true;
}


void init()
{
	int c, v, i;
	
	best_unsat_stack_fill_pointer = -1;
	unsat_stack_fill_pointer = 0;
	total_unsat_clause_weight = 0;
	
	init_clause_weight_scheme_ptr();
	init_unsatvar_related_ptr();
	
	/*
	for(v=1; v<=num_vars; v++)
	{
		if(rng.next(2)==0) cur_soln[v] = 0;
		else cur_soln[v] = 1;
	}
	*/
	
	init_solution_ptr();
	
	for(c=0; c<num_clauses; c++)
	{
		clause_weight[c] = 1;
		sat_count[c] = 0;
		sat_var[c] = -1;
		for(i=0; i<clause_lit_count[c]; i++)
		{
			if(cur_soln[clause_lit[c][i].var_num] == clause_lit[c][i].sense)
			{
				sat_count[c]++;
				sat_var[c] = clause_lit[c][i].var_num;
			}
		}
		
		if(sat_count[c] == 0)
		{
			unsat(c);
		}
	}
	
	avg_clause_weight = 1;
	delta_total_clause_weight = 0;
	
	candidate_var_stack_fill_pointer = 0;
	
	init_time_stamp_ptr();
	init_score_ptr();
	init_break_score_ptr();
	init_make_score_ptr();
	
	init_sat_var2_ptr();
	init_score2_ptr();
	
	//build_neighbor_relation_ptr();
	init_nvcc_conf_change_ptr();
	
	init_cscc_conf_change_ptr();
	
	init_nvccdvar_related_ptr();
}


void unsat(int clause)
{
	int v;
	index_in_unsat_stack[clause] = unsat_stack_fill_pointer;
	push(clause,unsat_stack);
	
	total_unsat_clause_weight += clause_weight[clause];
	
	update_unsatvar_related_in_unsat_function_ptr(clause);
}


void sat(int clause)
{
	int index,last_unsat_clause,v,last_unsat_var;

	last_unsat_clause = pop(unsat_stack);
	index = index_in_unsat_stack[clause];
	unsat_stack[index] = last_unsat_clause;
	index_in_unsat_stack[last_unsat_clause] = index;
	
	total_unsat_clause_weight -= clause_weight[clause];
	
	update_unsatvar_related_in_sat_function_ptr(clause);
}


long long calc_current_total_unsat_clause_weight(int *cur_soln)
{
	int c, i;
	bool flag;
	long long current_total_unsat_clause_weight = 0;
	
	for (c=0; c<num_clauses; c++) 
	{
		flag = false;
		for(i=0; i<clause_lit_count[c]; i++)
		{
			if (cur_soln[clause_lit[c][i].var_num] == clause_lit[c][i].sense)
			{
				flag = true; 
				break;
			}
		}

		if(!flag) current_total_unsat_clause_weight += clause_weight[c];
	}
	return current_total_unsat_clause_weight;
}

bool verify_soln(int *cur_soln)
{
	int c, i;
	bool flag;
	for (c=0; c<num_clauses; c++) 
	{
		flag = false;
		for(i=0; i<clause_lit_count[c]; i++)
		{
			if (cur_soln[clause_lit[c][i].var_num] == clause_lit[c][i].sense)
			{
				flag = true; 
				break;
			}
		}

		if(!flag) return false;
	}
	return true;
}

#endif
