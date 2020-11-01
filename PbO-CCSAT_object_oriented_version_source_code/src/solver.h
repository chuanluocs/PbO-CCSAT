#ifndef _SOLVER_H
#define _SOLVER_H

#include <cmath>
#include <ctime>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

#include "mersenne.h"

using namespace std;

struct lit
{
    int clause_num; //clause num, begin with 0
    int var_num;    //variable num, begin with 1
    int sense;      //is 1 for true literals, 0 for false literals.
};

class Solver
{
private:
    // solver version:
    string version_str;

    // randomness:
    Mersenne rng;
    int seed;

    // cnf instance:
    string inst;
    int ori_num_vars;
    int ori_num_clauses;
    int num_vars;
    int num_clauses;
    int maxi_clause_len;
    int mini_clause_len;
    int all_pos_lit_count;
    int all_neg_lit_count;
    int *var_pos_lit_count;
    int *var_neg_lit_count;
    lit **var_lit;
    int *var_lit_count;
    lit **clause_lit;
    int *clause_lit_count;

    // features:
    int **var_neighbor;
    int *var_neighbor_count;
    long long *score;
    long long *break_score;
    long long *make_score;
    long long *time_stamp;
    int *nvcc_conf_change;
    int *cscc_conf_change;
    long long *score2;
    int *candidate_var_stack;
    int candidate_var_stack_fill_pointer;
    int *nvccdvar_stack;
    int nvccdvar_stack_fill_pointer;
    bool *already_in_nvccdvar_stack;
    double *var_prob_value;
    int *unsatvar_stack;
    int unsatvar_stack_fill_pointer;
    int *index_in_unsatvar_stack;
    int *unsatvar_app_count;
    long long *clause_weight;
    int *sat_count;
    int *sat_var;
    int *sat_var2;
    int *unsat_stack;
    int unsat_stack_fill_pointer;
    int *index_in_unsat_stack;
    long long total_unsat_clause_weight;

    // solution:
    int *cur_soln;
    long long best_unsat_stack_fill_pointer;

    // algorithm parameters:
    bool decision_perform_pac_heuristic;
    bool decision_perform_first_div;
    bool decision_perform_double_cc;
    bool decision_perform_aspiration;
    bool decision_perform_clause_weight_scheme;
    double para_prob_pac;
    double para_prob_first_div;
    double para_prob_novelty;
    int para_d_hscore;
    int para_beta_hscore;
    int para_gamma_hscore2;
    long long avg_clause_weight;
    long long delta_total_clause_weight;
    long long scale_avg;
    long long para_clause_weight_threshold_swt;
    double para_p_scale_swt;
    double para_q_scale_swt;
    double para_smooth_probability_paws;
    int large_clause_weight_count_threshold;
    int *large_weight_clauses;
    int large_weight_clauses_count;
    double para_sparrow_c1;
    int para_sparrow_c2;
    int para_sparrow_c3;
    const long long default_maxi_tries_flips_num = 4500000000000000000ll;
    long long maxi_tries = default_maxi_tries_flips_num;
    long long maxi_flips = default_maxi_tries_flips_num;
    long long tries;
    long long step;
    const long long maximum_clause_weight_limit = 2147483646ll;
    double para_probsat_cb;

    // function prts:
    void (Solver::*init_solution_ptr)();
    void (Solver::*init_unsatvar_related_ptr)();
    void (Solver::*update_unsatvar_related_in_unsat_function_ptr)(int);
    void (Solver::*update_unsatvar_related_in_sat_function_ptr)(int);
    void (Solver::*init_nvccdvar_related_ptr)();
    void (Solver::*update_nvccdvar_related_in_update_function_ptr)(int flipvar);
    void (Solver::*update_nvccdvar_related_in_swt_scheme_ptr)();
    void (Solver::*update_nvccdvar_related_in_dec_paws_scheme_ptr)();
    void (Solver::*update_nvccdvar_related_in_inc_paws_scheme_ptr)();
    void (Solver::*init_sat_var2_ptr)();
    void (Solver::*update_sat_var2_ptr)(int);
    void (Solver::*init_score2_ptr)();
    void (Solver::*update_score2_ptr)(int);
    void (Solver::*init_time_stamp_ptr)();
    void (Solver::*update_time_stamp_ptr)(int, long long);
    void (Solver::*init_score_ptr)();
    void (Solver::*update_score_ptr)(int);
    void (Solver::*init_break_score_ptr)();
    void (Solver::*update_break_score_ptr)(int);
    void (Solver::*init_make_score_ptr)();
    void (Solver::*update_make_score_ptr)(int);
    void (Solver::*build_neighbor_relation_ptr)();
    void (Solver::*init_nvcc_conf_change_ptr)();
    void (Solver::*update_nvcc_conf_change_ptr)(int);
    void (Solver::*init_cscc_conf_change_ptr)();
    void (Solver::*update_cscc_conf_change_ptr)(int);
    int (Solver::*select_var_pac_heuristic_ptr)();
    int (Solver::*select_unsat_clause_heuristic_first_div_ptr)();
    int (Solver::*select_var_from_clause_heuristic_first_div_ptr)(int);
    int (Solver::*select_var_cscc_heuristic_ptr)();
    int (Solver::*select_var_nvcc_heuristic_ptr)();
    int (Solver::*select_var_aspiration_heuristic_ptr)();
    int (Solver::*select_var_by_break_tie_in_set_ptr)(int *, int);
    void (Solver::*init_clause_weight_scheme_ptr)();
    void (Solver::*clause_weight_scheme_ptr)();
    int (Solver::*select_unsat_clause_heuristic_div_ptr)();
    int (Solver::*select_var_from_clause_heuristic_div_ptr)(int);

public:
    Solver(int argc, char **argv);
    Solver(string inst_dir, int sd);
    Solver(string inst_dir);

    // arguments settings:
    void default_algorithm_settings();
    bool parse_arguments(int argc, char **argv);

    // stack operation:
    int pop(int *stack, int &stack_fill_pointer);
    int push(int item, int *stack, int &stack_fill_pointer);

    // from local search:
    bool solve();
    int pick_var_pbocc();
    bool local_search_each_try();
    void flip(int flipvar);
    void update(int flipvar, long long step);

    // from basis:
    void reassign_istringstream(istringstream &iss, string line);
    bool build_instance(string inst);
    void allocate_memory(int num_vars, int num_clauses);
    void free_memory();
    void init();
    void unsat(int clause);
    void sat(int clause);

    long long calc_current_total_unsat_clause_weight(int *cur_soln);
    bool verify_soln(int *cur_soln);

    // heuristics:
    inline void empty_function_ret_void() {}
    inline void empty_function_ret_void(int a) {}
    inline void empty_function_ret_void(int a, long long b) {}
    inline int empty_function_ret_int() { return -1; }
    inline int empty_function_ret_int(int a) { return -1; }
    inline int empty_function_ret_int(int *a, int b) { return -1; }
    inline long long calc_hscore2(int v) { return score2[v] + (step - time_stamp[v]) / para_gamma_hscore2; }
    inline long long calc_hscore(int v) { return score[v] + score2[v] / para_d_hscore + (step - time_stamp[v]) / para_beta_hscore; }
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
    int select_var_from_clause_random_heuristic(int clause_num);
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
    int select_var_by_random_break_tie_in_set(int *the_set, int the_set_length);
    int select_var_by_greatest_age_break_tie_in_set(int *the_set, int the_set_length);
    int select_var_by_greatest_hscore_break_tie_in_set(int *the_set, int the_set_length);
    int select_var_from_clause_greatest_hscore_heuristic(int clause_num);
    int select_var_by_greatest_hscore2_break_tie_in_set(int *the_set, int the_set_length);
    int select_var_from_clause_greatest_hscore2_heuristic(int clause_num);
    void init_clause_weight_scheme_swt();
    void smooth_clause_weight_swt();
    void clause_weight_scheme_swt();
    void init_clause_weight_scheme_paws();
    void dec_clause_weight_paws();
    void inc_clause_weight_paws();
    void clause_weight_scheme_paws();

    // show results and arguments:
    inline int *get_soln() { return cur_soln; }
    inline long long get_best_unsat_stack_fill_pointer() { return best_unsat_stack_fill_pointer; }
    int get_default_init_solution();
    int get_default_sel_clause_first_div();
    int get_default_sel_var_first_div();
    int get_default_sel_var_break_tie_greedy();
    int get_default_sel_clause_weight_scheme();
    int get_default_sel_clause_div();
    int get_default_sel_var_div();
    string get_last_level_name(string file_path);
    void print_general_information(char *executable_name);
    void print_usage_info(char *executable_name);
    void print_algorithm_settings_information();
    void print_solution();
    void print_SAT_result();
    void print_UNKNOWN_result();
    void print_run_info(double run_time);
};

#endif