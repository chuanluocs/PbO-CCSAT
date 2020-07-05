#ifndef _BASIS_H
#define _BASIS_H

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <cmath>
#include <ctime>
using namespace std;

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

#include "heuristic.h"
#include "info.h"
#include "local_search.h"
#include "mersenne.h"
#include "parse_arguments.h"
#include "settings.h"

#define pop(stack) stack[--stack ## _fill_pointer]
#define push(item, stack) stack[stack ## _fill_pointer++] = item

struct lit
{
    int clause_num;		//clause num, begin with 0
    int var_num;		//variable num, begin with 1
    int sense;			//is 1 for true literals, 0 for false literals.
};

extern Mersenne			rng;
extern int 				seed;
extern string 			inst;

extern int				ori_num_vars;
extern int				ori_num_clauses;

extern int 				num_vars;
extern int 				num_clauses;
extern int 				maxi_clause_len;
extern int 				mini_clause_len;

extern int				all_pos_lit_count;
extern int				all_neg_lit_count;
extern int*				var_pos_lit_count;
extern int*				var_neg_lit_count;

extern lit**			var_lit;
extern int*				var_lit_count;
extern lit**			clause_lit;
extern int*				clause_lit_count;

extern int**	 		var_neighbor;
extern int* 			var_neighbor_count;

extern long long*		score;
extern long long*		break_score;
extern long long*		make_score;
extern long long*		time_stamp;
extern int*				nvcc_conf_change;
extern int*				cscc_conf_change;
extern long long*		score2;
extern int*				candidate_var_stack;
extern int				candidate_var_stack_fill_pointer;
extern int*				nvccdvar_stack;
extern int				nvccdvar_stack_fill_pointer;
extern bool*			already_in_nvccdvar_stack;
extern double*			var_prob_value;

extern int*				unsatvar_stack;		
extern int				unsatvar_stack_fill_pointer;
extern int*				index_in_unsatvar_stack;
extern int*				unsatvar_app_count;

extern long long*	 	clause_weight;
extern int*				sat_count;
extern int*				sat_var;
extern int*				sat_var2;

extern int*		 		unsat_stack;
extern int		 		unsat_stack_fill_pointer;
extern int*				index_in_unsat_stack;

extern long long		total_unsat_clause_weight;

extern int*				cur_soln;
extern long long		best_unsat_stack_fill_pointer;

extern const long long 	default_maxi_tries_flips_num;
extern long long		maxi_tries;
extern long long		maxi_flips;
extern long long		tries;
extern long long		step;

extern const long long 	maximum_clause_weight_limit;

extern bool				decision_perform_pac_heuristic;
extern bool				decision_perform_first_div;
extern bool				decision_perform_double_cc;
extern bool				decision_perform_aspiration;
extern bool				decision_perform_clause_weight_scheme;


extern double			para_prob_pac;
extern double			para_prob_first_div;
extern double 			para_prob_novelty;

extern int 				para_d_hscore;
extern int 				para_beta_hscore;
extern int 				para_gamma_hscore2;

extern long long		avg_clause_weight;
extern long long		delta_total_clause_weight;
extern long long		scale_avg;
extern long long		para_clause_weight_threshold_swt;
extern double			para_p_scale_swt;
extern double			para_q_scale_swt;

extern double			para_smooth_probability_paws;
extern int				large_clause_weight_count_threshold;
extern int*				large_weight_clauses;
extern int				large_weight_clauses_count;

extern double			para_sparrow_c1;
extern int				para_sparrow_c2;
extern int				para_sparrow_c3;

extern double			para_probsat_cb;



void reassign_istringstream(istringstream &iss, string line);
bool build_instance(string inst);
void allocate_memory(int num_vars, int num_clauses);
void free_memory(int num_vars, int num_clauses);
void init();
void unsat(int clause);
void sat(int clause);

long long calc_current_total_unsat_clause_weight(int *cur_soln);
bool verify_soln(int *cur_soln);

#endif
