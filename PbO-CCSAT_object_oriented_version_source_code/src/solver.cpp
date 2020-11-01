#ifndef _SOLVER_CPP
#define _SOLVER_CPP

#include "solver.h"

Solver::Solver(int argc, char **argv)
{
    version_str = "1.2.3";
    if (!parse_arguments(argc, argv))
    {
        cout << "c" << endl;
        cout << "c Arguments Error!" << endl;
        cout << "c" << endl;
        print_usage_info(argv[0]);
        exit(-1);
    }

    if (!build_instance(inst))
    {
        cout << "c" << endl;
        cout << "c Instance File Error!" << endl;
        cout << "c" << endl;
        print_usage_info(argv[0]);
        exit(-1);
    }
}

Solver::Solver(string inst_dir, int sd)
{
    version_str = "1.2.3";
    inst = inst_dir;
    seed = sd;
    default_algorithm_settings();
    if (!build_instance(inst))
    {
        cout << "c" << endl;
        cout << "c Instance File Error!" << endl;
        cout << "c" << endl;
    }
}

Solver::Solver(string inst_dir)
{
    version_str = "1.2.3";
    inst = inst_dir;
    seed = 100;
    default_algorithm_settings();
    if (!build_instance(inst))
    {
        cout << "c" << endl;
        cout << "c Instance File Error!" << endl;
        cout << "c" << endl;
    }
}

int Solver::pop(int *stack, int &stack_fill_pointer)
{
    --stack_fill_pointer;
    return stack[stack_fill_pointer];
}

int Solver::push(int item, int *stack, int &stack_fill_pointer)
{
    stack[stack_fill_pointer] = item;
    stack_fill_pointer++;
    return item;
}

void Solver::reassign_istringstream(istringstream &iss, string line)
{
    iss.clear();
    iss.str(line);
    iss.seekg(0, ios::beg);
    return;
}

void Solver::allocate_memory(int num_vars, int num_clauses)
{
    int malloc_adding_length = 10;
    int malloc_var_length = num_vars + malloc_adding_length;
    int malloc_clause_length = num_clauses + malloc_adding_length;

    //allocate memory for those data structures with regards to num_vars
    var_lit = new lit *[malloc_var_length];
    var_lit_count = new int[malloc_var_length];
    var_pos_lit_count = new int[malloc_var_length];
    var_neg_lit_count = new int[malloc_var_length];
    var_neighbor = new int *[malloc_var_length];
    var_neighbor_count = new int[malloc_var_length];
    cur_soln = new int[malloc_var_length];
    score = new long long[malloc_var_length];
    break_score = new long long[malloc_var_length];
    make_score = new long long[malloc_var_length];
    time_stamp = new long long[malloc_var_length];
    nvcc_conf_change = new int[malloc_var_length];
    cscc_conf_change = new int[malloc_var_length];
    score2 = new long long[malloc_var_length];
    unsatvar_stack = new int[malloc_var_length];
    index_in_unsatvar_stack = new int[malloc_var_length];
    unsatvar_app_count = new int[malloc_var_length];
    candidate_var_stack = new int[malloc_var_length];
    nvccdvar_stack = new int[malloc_var_length];
    already_in_nvccdvar_stack = new bool[malloc_var_length];
    var_prob_value = new double[malloc_var_length];

    //allocate memory for those data structures with regards to num_clauses
    clause_lit = new lit *[malloc_clause_length];
    clause_lit_count = new int[malloc_clause_length];
    clause_weight = new long long[malloc_clause_length];
    sat_count = new int[malloc_clause_length];
    sat_var = new int[malloc_clause_length];
    unsat_stack = new int[malloc_clause_length];
    index_in_unsat_stack = new int[malloc_clause_length];
    sat_var2 = new int[malloc_clause_length];
    large_weight_clauses = new int[malloc_clause_length];

    return;
}

void Solver::free_memory()
{
    int v, c;

    //free memory for those data structures with regards to num_vars
    for (v = 1; v <= num_vars; v++)
    {
        delete[] var_lit[v];
        delete[] var_neighbor[v];
    }
    delete[] var_lit;
    delete[] var_lit_count;
    delete[] var_pos_lit_count;
    delete[] var_neg_lit_count;
    delete[] var_neighbor;
    delete[] var_neighbor_count;
    delete[] cur_soln;
    delete[] score;
    delete[] break_score;
    delete[] make_score;
    delete[] time_stamp;
    delete[] nvcc_conf_change;
    delete[] cscc_conf_change;
    delete[] score2;
    delete[] unsatvar_stack;
    delete[] index_in_unsatvar_stack;
    delete[] unsatvar_app_count;
    delete[] candidate_var_stack;
    delete[] nvccdvar_stack;
    delete[] already_in_nvccdvar_stack;
    delete[] var_prob_value;

    //free memory for those data structures with regards to num_clauses
    for (c = 0; c < num_clauses; c++)
    {
        delete[] clause_lit[c];
    }
    delete[] clause_lit;
    delete[] clause_lit_count;
    delete[] clause_weight;
    delete[] sat_count;
    delete[] sat_var;
    delete[] unsat_stack;
    delete[] index_in_unsat_stack;
    delete[] sat_var2;
    delete[] large_weight_clauses;

    return;
}

bool Solver::build_instance(string inst)
{
    string line;
    istringstream iss;
    int c, v, i;
    int cur_lit;
    bool lit_redundent, clause_redundent;
    string tempstr1, tempstr2;
    vector<int> temp_clause;

    ifstream fin(inst.c_str());
    if (!fin.is_open())
        return false;

    num_vars = 0;
    num_clauses = 0;

    while (getline(fin, line))
    {
        if (line.substr(0, 1) == "c")
            continue;
        else if (line.substr(0, 1) == "p")
        {
            reassign_istringstream(iss, line);
            iss >> tempstr1 >> tempstr2 >> num_vars >> num_clauses;
            ori_num_vars = num_vars;
            ori_num_clauses = num_clauses;
            break;
        }
    }

    allocate_memory(ori_num_vars, ori_num_clauses); //allocate memory for data structures

    for (v = 1; v <= num_vars; v++)
    {
        var_lit_count[v] = 0;
        var_pos_lit_count[v] = 0;
        var_neg_lit_count[v] = 0;
    }
    for (c = 0; c < num_clauses; c++)
        clause_lit_count[c] = 0;

    maxi_clause_len = -1;
    mini_clause_len = -1;

    all_pos_lit_count = 0;
    all_neg_lit_count = 0;

    for (c = 0; c < num_clauses;)
    {
        clause_redundent = false;
        temp_clause.clear();
        vector<int>().swap(temp_clause);

        fin >> cur_lit;
        while (cur_lit != 0)
        {
            if (!clause_redundent)
            {
                lit_redundent = false;
                for (i = 0; i < temp_clause.size(); i++)
                {
                    if (cur_lit == temp_clause[i])
                    {
                        lit_redundent = true;
                        break;
                    }
                    else if (cur_lit == -temp_clause[i])
                    {
                        clause_redundent = true;
                        break;
                    }
                }

                if (!lit_redundent)
                {
                    temp_clause.push_back(cur_lit);
                }
            }
            fin >> cur_lit;
        }

        if (!clause_redundent)
        {
            clause_lit_count[c] = temp_clause.size();
            clause_lit[c] = new lit[clause_lit_count[c] + 1];

            for (i = 0; i < temp_clause.size(); i++)
            {
                clause_lit[c][i].clause_num = c;
                clause_lit[c][i].var_num = abs(temp_clause[i]);
                if (temp_clause[i] > 0)
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

            clause_lit[c][clause_lit_count[c]].var_num = 0;
            clause_lit[c][clause_lit_count[c]].clause_num = -1;

            if (maxi_clause_len == -1)
                maxi_clause_len = clause_lit_count[c];
            else if (maxi_clause_len < clause_lit_count[c])
                maxi_clause_len = clause_lit_count[c];

            if (mini_clause_len == -1)
                mini_clause_len = clause_lit_count[c];
            else if (mini_clause_len > clause_lit_count[c])
                mini_clause_len = clause_lit_count[c];

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

    for (v = 1; v <= num_vars; v++)
    {
        var_lit[v] = new lit[var_lit_count[v] + 1];
        var_lit[v][var_lit_count[v]].var_num = 0;
        var_lit[v][var_lit_count[v]].clause_num = -1;
        var_lit_count[v] = 0; //reset to 0, for building up the array
    }

    for (c = 0; c < num_clauses; c++)
    {
        for (i = 0; i < clause_lit_count[c]; ++i)
        {
            v = clause_lit[c][i].var_num;
            var_lit[v][var_lit_count[v]] = clause_lit[c][i];
            var_lit_count[v]++;
        }
    }

    (this->*build_neighbor_relation_ptr)();

    return true;
}

void Solver::init()
{
    int c, v, i;

    best_unsat_stack_fill_pointer = -1;
    unsat_stack_fill_pointer = 0;
    total_unsat_clause_weight = 0;

    (this->*init_clause_weight_scheme_ptr)();
    (this->*init_unsatvar_related_ptr)();

    (this->*init_solution_ptr)();

    for (c = 0; c < num_clauses; c++)
    {
        clause_weight[c] = 1;
        sat_count[c] = 0;
        sat_var[c] = -1;
        for (i = 0; i < clause_lit_count[c]; i++)
        {
            if (cur_soln[clause_lit[c][i].var_num] == clause_lit[c][i].sense)
            {
                sat_count[c]++;
                sat_var[c] = clause_lit[c][i].var_num;
            }
        }

        if (sat_count[c] == 0)
        {
            unsat(c);
        }
    }

    avg_clause_weight = 1;
    delta_total_clause_weight = 0;

    candidate_var_stack_fill_pointer = 0;

    (this->*init_time_stamp_ptr)();
    (this->*init_score_ptr)();
    (this->*init_break_score_ptr)();
    (this->*init_make_score_ptr)();

    (this->*init_sat_var2_ptr)();
    (this->*init_score2_ptr)();

    (this->*init_nvcc_conf_change_ptr)();

    (this->*init_cscc_conf_change_ptr)();

    (this->*init_nvccdvar_related_ptr)();
}

void Solver::unsat(int clause)
{
    int v;
    index_in_unsat_stack[clause] = unsat_stack_fill_pointer;
    push(clause, unsat_stack, unsat_stack_fill_pointer);

    total_unsat_clause_weight += clause_weight[clause];

    (this->*update_unsatvar_related_in_unsat_function_ptr)(clause);
}

void Solver::sat(int clause)
{
    int index, last_unsat_clause, v, last_unsat_var;

    last_unsat_clause = pop(unsat_stack, unsat_stack_fill_pointer);
    index = index_in_unsat_stack[clause];
    unsat_stack[index] = last_unsat_clause;
    index_in_unsat_stack[last_unsat_clause] = index;

    total_unsat_clause_weight -= clause_weight[clause];

    (this->*update_unsatvar_related_in_sat_function_ptr)(clause);
}

long long Solver::calc_current_total_unsat_clause_weight(int *cur_soln)
{
    int c, i;
    bool flag;
    long long current_total_unsat_clause_weight = 0;

    for (c = 0; c < num_clauses; c++)
    {
        flag = false;
        for (i = 0; i < clause_lit_count[c]; i++)
        {
            if (cur_soln[clause_lit[c][i].var_num] == clause_lit[c][i].sense)
            {
                flag = true;
                break;
            }
        }

        if (!flag)
            current_total_unsat_clause_weight += clause_weight[c];
    }
    return current_total_unsat_clause_weight;
}

bool Solver::verify_soln(int *cur_soln)
{
    int c, i;
    bool flag;
    for (c = 0; c < num_clauses; c++)
    {
        flag = false;
        for (i = 0; i < clause_lit_count[c]; i++)
        {
            if (cur_soln[clause_lit[c][i].var_num] == clause_lit[c][i].sense)
            {
                flag = true;
                break;
            }
        }

        if (!flag)
            return false;
    }
    return true;
}

void Solver::default_algorithm_settings() //DCCASat for structured instances
{
    init_solution_ptr = &Solver::init_solution_all_true_or_false;

    init_unsatvar_related_ptr = &Solver::init_unsatvar_related;                                         //
    update_unsatvar_related_in_unsat_function_ptr = &Solver::update_unsatvar_related_in_unsat_function; //
    update_unsatvar_related_in_sat_function_ptr = &Solver::update_unsatvar_related_in_sat_function;     //

    init_nvccdvar_related_ptr = &Solver::init_nvccdvar_related;                                           //
    update_nvccdvar_related_in_update_function_ptr = &Solver::update_nvccdvar_related_in_update_function; //
    update_nvccdvar_related_in_swt_scheme_ptr = &Solver::update_nvccdvar_related_in_swt_scheme;           //
    update_nvccdvar_related_in_dec_paws_scheme_ptr = &Solver::update_nvccdvar_related_in_dec_paws_scheme; //
    update_nvccdvar_related_in_inc_paws_scheme_ptr = &Solver::update_nvccdvar_related_in_inc_paws_scheme; //

    init_sat_var2_ptr = &Solver::empty_function_ret_void;   //
    update_sat_var2_ptr = &Solver::empty_function_ret_void; //

    init_score2_ptr = &Solver::empty_function_ret_void;   //
    update_score2_ptr = &Solver::empty_function_ret_void; //

    init_time_stamp_ptr = &Solver::init_time_stamp;     //
    update_time_stamp_ptr = &Solver::update_time_stamp; //

    init_score_ptr = &Solver::init_score;     //
    update_score_ptr = &Solver::update_score; //

    init_break_score_ptr = &Solver::empty_function_ret_void;   //
    update_break_score_ptr = &Solver::empty_function_ret_void; //

    init_make_score_ptr = &Solver::empty_function_ret_void;   //
    update_make_score_ptr = &Solver::empty_function_ret_void; //

    build_neighbor_relation_ptr = &Solver::build_neighbor_relation; //
    init_nvcc_conf_change_ptr = &Solver::init_nvcc_conf_change;     //
    update_nvcc_conf_change_ptr = &Solver::update_nvcc_conf_change; //

    init_cscc_conf_change_ptr = &Solver::init_cscc_conf_change;     //
    update_cscc_conf_change_ptr = &Solver::update_cscc_conf_change; //

    select_var_pac_heuristic_ptr = select_var_pac_heuristic_ptr; //empty_function_ret_int; //

    select_unsat_clause_heuristic_first_div_ptr = &Solver::select_unsat_clause_random_heuristic;        //empty_function_ret_int; //
    select_var_from_clause_heuristic_first_div_ptr = &Solver::select_var_from_clause_sparrow_heuristic; //

    select_var_cscc_heuristic_ptr = &Solver::select_var_cscc_heuristic_from_nvccdvar_stack; //
    select_var_nvcc_heuristic_ptr = &Solver::select_var_nvcc_heuristic_from_nvccdvar_stack; //
    select_var_aspiration_heuristic_ptr = &Solver::select_var_aspiration_heuristic;         //

    select_var_by_break_tie_in_set_ptr = &Solver::select_var_by_greatest_age_break_tie_in_set; //

    init_clause_weight_scheme_ptr = &Solver::init_clause_weight_scheme_swt; //
    clause_weight_scheme_ptr = &Solver::clause_weight_scheme_swt;           //

    select_unsat_clause_heuristic_div_ptr = &Solver::select_unsat_clause_random_heuristic;        //
    select_var_from_clause_heuristic_div_ptr = &Solver::select_var_from_clause_sparrow_heuristic; //

    maxi_tries = default_maxi_tries_flips_num; //
    maxi_flips = 2000000000ll;                 //

    decision_perform_pac_heuristic = false;       //
    decision_perform_first_div = false;           //
    decision_perform_double_cc = true;            //
    decision_perform_aspiration = true;           //
    decision_perform_clause_weight_scheme = true; //

    para_prob_pac = 0.01;       //
    para_prob_first_div = 0.05; //

    para_d_hscore = 8;         //
    para_beta_hscore = 1000;   //
    para_gamma_hscore2 = 1000; //

    para_clause_weight_threshold_swt = 300; //
    para_p_scale_swt = 0.3;                 //
    para_q_scale_swt = 0;                   //

    para_smooth_probability_paws = 0.8; //

    para_prob_novelty = 0.2; //

    para_sparrow_c1 = 2.15;  //
    para_sparrow_c2 = 4;     //
    para_sparrow_c3 = 75000; //

    para_probsat_cb = 2.85; //
}

bool Solver::parse_arguments(int argc, char **argv)
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

    for (i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "-inst") == 0)
        {
            i++;
            if (i >= argc)
                return false;
            inst = argv[i];
            flag_inst = true;
            continue;
        }
        else if (strcmp(argv[i], "-seed") == 0)
        {
            i++;
            if (i >= argc)
                return false;
            sscanf(argv[i], "%d", &seed);
            flag_seed = true;
            continue;
        }
        else if (strcmp(argv[i], "-maxi_tries") == 0)
        {
            i++;
            if (i >= argc)
                return false;
            sscanf(argv[i], "%lld", &maxi_tries);
            flag_maxi_tries = true;
            continue;
        }
        else if (strcmp(argv[i], "-maxi_flips") == 0)
        {
            i++;
            if (i >= argc)
                return false;
            sscanf(argv[i], "%lld", &maxi_flips);
            continue;
        }
        else if (strcmp(argv[i], "-init_solution") == 0)
        {
            i++;
            if (i >= argc)
                return false;
            if (strcmp(argv[i], "1") == 0)
            {
                init_solution_ptr = &Solver::init_solution_random;
                continue;
            }
            else if (strcmp(argv[i], "2") == 0)
            {
                init_solution_ptr = &Solver::init_solution_all_true_or_false;
                continue;
            }
            else if (strcmp(argv[i], "3") == 0)
            {
                init_solution_ptr = &Solver::init_solution_each_lit_true_or_false;
                continue;
            }
            else
                return false;
        }
        else if (strcmp(argv[i], "-perform_pac") == 0)
        {
            i++;
            if (i >= argc)
                return false;
            if (strcmp(argv[i], "0") == 0)
            {
                decision_perform_pac_heuristic = false;
                select_var_pac_heuristic_ptr = &Solver::empty_function_ret_int;
                para_prob_pac = 0;
                continue;
            }
            else if (strcmp(argv[i], "1") == 0)
            {
                decision_perform_pac_heuristic = true;
                select_var_pac_heuristic_ptr = &Solver::select_var_pac_heuristic;
                continue;
            }
            else
                return false;
        }
        else if (strcmp(argv[i], "-prob_pac") == 0)
        {
            i++;
            if (i >= argc)
                return false;
            sscanf(argv[i], "%lf", &para_prob_pac);
            continue;
        }
        else if (strcmp(argv[i], "-perform_first_div") == 0)
        {
            i++;
            if (i >= argc)
                return false;
            if (strcmp(argv[i], "0") == 0)
            {
                decision_perform_first_div = false;
                select_unsat_clause_heuristic_first_div_ptr = &Solver::empty_function_ret_int;
                select_var_from_clause_heuristic_first_div_ptr = &Solver::empty_function_ret_int;
                para_prob_first_div = 0;
                continue;
            }
            else if (strcmp(argv[i], "1") == 0)
            {
                decision_perform_first_div = true;
                continue;
            }
            else
                return false;
        }
        else if (strcmp(argv[i], "-prob_first_div") == 0)
        {
            i++;
            if (i >= argc)
                return false;
            sscanf(argv[i], "%lf", &para_prob_first_div);
            continue;
        }

        else if (strcmp(argv[i], "-perform_double_cc") == 0)
        {
            i++;
            if (i >= argc)
                return false;
            if (strcmp(argv[i], "0") == 0)
            {
                decision_perform_double_cc = false;
                select_var_cscc_heuristic_ptr = &Solver::empty_function_ret_int;
                continue;
            }
            else if (strcmp(argv[i], "1") == 0)
            {
                decision_perform_double_cc = true;
                flag_cscc = true;
                select_var_cscc_heuristic_ptr = &Solver::select_var_cscc_heuristic_from_nvccdvar_stack;
                continue;
            }
            else
                return false;
        }
        else if (strcmp(argv[i], "-perform_aspiration") == 0)
        {
            i++;
            if (i >= argc)
                return false;
            if (strcmp(argv[i], "0") == 0)
            {
                decision_perform_aspiration = false;
                select_var_aspiration_heuristic_ptr = &Solver::empty_function_ret_int;
                continue;
            }
            else if (strcmp(argv[i], "1") == 0)
            {
                decision_perform_aspiration = true;
                select_var_aspiration_heuristic_ptr = &Solver::select_var_aspiration_heuristic;
                continue;
            }
            else
                return false;
        }
        else if (strcmp(argv[i], "-sel_var_break_tie_greedy") == 0)
        {
            i++;
            if (i >= argc)
                return false;
            if (strcmp(argv[i], "1") == 0)
            {
                select_var_by_break_tie_in_set_ptr = &Solver::select_var_by_random_break_tie_in_set;
                continue;
            }
            else if (strcmp(argv[i], "2") == 0)
            {
                flag_age = true;
                select_var_by_break_tie_in_set_ptr = &Solver::select_var_by_greatest_age_break_tie_in_set;
                continue;
            }
            else if (strcmp(argv[i], "3") == 0)
            {
                flag_score2 = true;
                select_var_by_break_tie_in_set_ptr = &Solver::select_var_by_greatest_hscore_break_tie_in_set;
                continue;
            }
            else if (strcmp(argv[i], "4") == 0)
            {
                flag_score2 = true;
                select_var_by_break_tie_in_set_ptr = &Solver::select_var_by_greatest_hscore2_break_tie_in_set;
                continue;
            }
            else
                return false;
        }
        else if (strcmp(argv[i], "-d_hscore") == 0)
        {
            i++;
            if (i >= argc)
                return false;
            sscanf(argv[i], "%d", &para_d_hscore);
            continue;
        }
        else if (strcmp(argv[i], "-beta_hscore") == 0)
        {
            i++;
            if (i >= argc)
                return false;
            sscanf(argv[i], "%d", &para_beta_hscore);
            continue;
        }
        else if (strcmp(argv[i], "-gamma_hscore2") == 0)
        {
            i++;
            if (i >= argc)
                return false;
            sscanf(argv[i], "%d", &para_gamma_hscore2);
            continue;
        }
        else if (strcmp(argv[i], "-perform_clause_weight") == 0)
        {
            i++;
            if (i >= argc)
                return false;
            if (strcmp(argv[i], "0") == 0)
            {
                decision_perform_clause_weight_scheme = false;
                init_clause_weight_scheme_ptr = &Solver::empty_function_ret_void;
                clause_weight_scheme_ptr = &Solver::empty_function_ret_void;
                para_clause_weight_threshold_swt = 0;
                para_p_scale_swt = 0;
                para_q_scale_swt = 0;
                para_smooth_probability_paws = 0;
                continue;
            }
            else if (strcmp(argv[i], "1") == 0)
            {
                decision_perform_clause_weight_scheme = true;
                continue;
            }
            else
                return false;
        }
        else if (strcmp(argv[i], "-sel_clause_weight_scheme") == 0)
        {
            i++;
            if (i >= argc)
                return false;
            if (strcmp(argv[i], "1") == 0)
            {
                init_clause_weight_scheme_ptr = &Solver::init_clause_weight_scheme_swt;
                clause_weight_scheme_ptr = &Solver::clause_weight_scheme_swt;
                continue;
            }
            else if (strcmp(argv[i], "2") == 0)
            {
                init_clause_weight_scheme_ptr = &Solver::init_clause_weight_scheme_paws;
                clause_weight_scheme_ptr = &Solver::clause_weight_scheme_paws;
                continue;
            }
            else
                return false;
        }
        else if (strcmp(argv[i], "-threshold_swt") == 0)
        {
            i++;
            if (i >= argc)
                return false;
            sscanf(argv[i], "%lld", &para_clause_weight_threshold_swt);
            continue;
        }
        else if (strcmp(argv[i], "-p_swt") == 0)
        {
            i++;
            if (i >= argc)
                return false;
            sscanf(argv[i], "%lf", &para_p_scale_swt);
            continue;
        }
        else if (strcmp(argv[i], "-q_swt") == 0)
        {
            i++;
            if (i >= argc)
                return false;
            sscanf(argv[i], "%lf", &para_q_scale_swt);
            continue;
        }
        else if (strcmp(argv[i], "-sp_paws") == 0)
        {
            i++;
            if (i >= argc)
                return false;
            sscanf(argv[i], "%lf", &para_smooth_probability_paws);
            continue;
        }
        else if (strcmp(argv[i], "-sel_clause_div") == 0)
        {
            i++;
            if (i >= argc)
                return false;
            if (strcmp(argv[i], "1") == 0)
            {
                select_unsat_clause_heuristic_div_ptr = &Solver::select_unsat_clause_random_heuristic;
                select_unsat_clause_heuristic_first_div_ptr = &Solver::select_unsat_clause_random_heuristic;
                continue;
            }
            else if (strcmp(argv[i], "2") == 0)
            {
                select_unsat_clause_heuristic_div_ptr = &Solver::select_unsat_clause_weight_distribution_heuristic;
                select_unsat_clause_heuristic_first_div_ptr = &Solver::select_unsat_clause_weight_distribution_heuristic;
                continue;
            }
            else
                return false;
        }
        else if (strcmp(argv[i], "-sel_var_div") == 0)
        {
            i++;
            if (i >= argc)
                return false;
            if (strcmp(argv[i], "1") == 0)
            {
                select_var_from_clause_heuristic_div_ptr = &Solver::select_var_from_clause_random_heuristic;
                select_var_from_clause_heuristic_first_div_ptr = &Solver::select_var_from_clause_random_heuristic;
                continue;
            }
            else if (strcmp(argv[i], "2") == 0)
            {
                flag_age = true;
                select_var_from_clause_heuristic_div_ptr = &Solver::select_var_from_clause_greatest_age_heuristic;
                select_var_from_clause_heuristic_first_div_ptr = &Solver::select_var_from_clause_greatest_age_heuristic;
                continue;
            }
            else if (strcmp(argv[i], "3") == 0)
            {
                select_var_from_clause_heuristic_div_ptr = &Solver::select_var_from_clause_greatest_score_heuristic;
                select_var_from_clause_heuristic_first_div_ptr = &Solver::select_var_from_clause_greatest_score_heuristic;
                continue;
            }
            else if (strcmp(argv[i], "4") == 0)
            {
                flag_score2 = true;
                select_var_from_clause_heuristic_div_ptr = &Solver::select_var_from_clause_greatest_hscore_heuristic;
                select_var_from_clause_heuristic_first_div_ptr = &Solver::select_var_from_clause_greatest_hscore_heuristic;
                continue;
            }
            else if (strcmp(argv[i], "5") == 0)
            {
                flag_score2 = true;
                select_var_from_clause_heuristic_div_ptr = &Solver::select_var_from_clause_greatest_hscore2_heuristic;
                select_var_from_clause_heuristic_first_div_ptr = &Solver::select_var_from_clause_greatest_hscore2_heuristic;
                continue;
            }
            else if (strcmp(argv[i], "6") == 0)
            {
                flag_age = true;
                select_var_from_clause_heuristic_div_ptr = &Solver::select_var_from_clause_novelty_heuristic;
                select_var_from_clause_heuristic_first_div_ptr = &Solver::select_var_from_clause_novelty_heuristic;
                continue;
            }
            else if (strcmp(argv[i], "7") == 0)
            {
                flag_age = true;
                select_var_from_clause_heuristic_div_ptr = &Solver::select_var_from_clause_sparrow_heuristic;
                select_var_from_clause_heuristic_first_div_ptr = &Solver::select_var_from_clause_sparrow_heuristic;
                continue;
            }
            else if (strcmp(argv[i], "8") == 0)
            {
                flag_break_score = true;
                select_var_from_clause_heuristic_div_ptr = &Solver::select_var_from_clause_probsat_heuristic;
                select_var_from_clause_heuristic_first_div_ptr = &Solver::select_var_from_clause_probsat_heuristic;
                continue;
            }
            else
                return false;
        }
        else if (strcmp(argv[i], "-prob_novelty") == 0)
        {
            i++;
            if (i >= argc)
                return false;
            sscanf(argv[i], "%lf", &para_prob_novelty);
            continue;
        }
        else if (strcmp(argv[i], "-sparrow_c1") == 0)
        {
            i++;
            if (i >= argc)
                return false;
            sscanf(argv[i], "%lf", &para_sparrow_c1);
            continue;
        }
        else if (strcmp(argv[i], "-sparrow_c2") == 0)
        {
            i++;
            if (i >= argc)
                return false;
            sscanf(argv[i], "%d", &para_sparrow_c2);
            continue;
        }
        else if (strcmp(argv[i], "-sparrow_c3") == 0)
        {
            i++;
            if (i >= argc)
                return false;
            sscanf(argv[i], "%d", &para_sparrow_c3);
            continue;
        }
        else if (strcmp(argv[i], "-probsat_cb") == 0)
        {
            i++;
            if (i >= argc)
                return false;
            sscanf(argv[i], "%lf", &para_probsat_cb);
            continue;
        }
        else
            return false;
    }

    if (flag_score2)
    {
        init_sat_var2_ptr = &Solver::init_sat_var2;
        update_sat_var2_ptr = &Solver::update_sat_var2;
        init_score2_ptr = &Solver::init_score2;
        update_score2_ptr = &Solver::update_score2;
    }

    if (flag_age)
    {
        init_time_stamp_ptr = &Solver::init_time_stamp;
        update_time_stamp_ptr = &Solver::update_time_stamp;
    }

    if (flag_cscc)
    {
        init_cscc_conf_change_ptr = &Solver::init_cscc_conf_change;
        update_cscc_conf_change_ptr = &Solver::update_cscc_conf_change;
    }

    init_unsatvar_related_ptr = &Solver::init_unsatvar_related;
    update_unsatvar_related_in_unsat_function_ptr = &Solver::update_unsatvar_related_in_unsat_function;
    update_unsatvar_related_in_sat_function_ptr = &Solver::update_unsatvar_related_in_sat_function;

    init_nvccdvar_related_ptr = &Solver::init_nvccdvar_related;
    update_nvccdvar_related_in_update_function_ptr = &Solver::update_nvccdvar_related_in_update_function;
    update_nvccdvar_related_in_swt_scheme_ptr = &Solver::update_nvccdvar_related_in_swt_scheme;
    update_nvccdvar_related_in_dec_paws_scheme_ptr = &Solver::update_nvccdvar_related_in_dec_paws_scheme;
    update_nvccdvar_related_in_inc_paws_scheme_ptr = &Solver::update_nvccdvar_related_in_inc_paws_scheme;

    init_score_ptr = &Solver::init_score;
    update_score_ptr = &Solver::update_score;

    if (flag_break_score)
    {
        init_break_score_ptr = &Solver::init_break_score;
        update_break_score_ptr = &Solver::update_break_score;
    }
    else
    {
        init_break_score_ptr = &Solver::empty_function_ret_void;
        update_break_score_ptr = &Solver::empty_function_ret_void;
    }

    init_make_score_ptr = &Solver::empty_function_ret_void;
    update_make_score_ptr = &Solver::empty_function_ret_void;

    build_neighbor_relation_ptr = &Solver::build_neighbor_relation;
    init_nvcc_conf_change_ptr = &Solver::init_nvcc_conf_change;
    update_nvcc_conf_change_ptr = &Solver::update_nvcc_conf_change;

    select_var_nvcc_heuristic_ptr = &Solver::select_var_nvcc_heuristic_from_nvccdvar_stack;

    if (!flag_maxi_tries)
        maxi_tries = default_maxi_tries_flips_num;

    if (flag_inst && flag_seed)
        return true;
    else
        return false;
}

bool Solver::solve()
{
    rng.seed(seed);

    bool answer = false;

    for (tries = 0; tries < maxi_tries; tries++)
    {
        init();
        answer = local_search_each_try();
        if (answer)
            return true;
    }

    if (tries > 0 && tries == maxi_tries)
        tries--;

    return false;
}

bool Solver::local_search_each_try()
{
    int flipvar;
    best_unsat_stack_fill_pointer = unsat_stack_fill_pointer;

    for (step = 0; step < maxi_flips; step++)
    {

        if (unsat_stack_fill_pointer < best_unsat_stack_fill_pointer)
        {
            best_unsat_stack_fill_pointer = unsat_stack_fill_pointer;
        }

        if (unsat_stack_fill_pointer == 0)
            return true;

        flipvar = pick_var_pbocc();

        flip(flipvar);
        update(flipvar, step);
    }

    if (unsat_stack_fill_pointer == 0)
        return true;
    return false;
}

int Solver::pick_var_pbocc()
{
    int v, c;
    if (decision_perform_pac_heuristic)
    {
        if (rng.nextClosed() < para_prob_pac)
        {
            v = (this->*select_var_pac_heuristic_ptr)();
            return v;
        }
    }

    if (decision_perform_first_div)
    {
        if (rng.nextClosed() < para_prob_first_div)
        {
            c = (this->*select_unsat_clause_heuristic_first_div_ptr)();
            v = (this->*select_var_from_clause_heuristic_first_div_ptr)(c);
            return v;
        }
    }

    if (decision_perform_double_cc)
    {
        v = (this->*select_var_cscc_heuristic_ptr)();
        if (v >= 1)
        {
            return v;
        }
    }

    v = (this->*select_var_nvcc_heuristic_ptr)();
    if (v >= 1)
    {
        return v;
    }

    if (decision_perform_aspiration)
    {
        v = (this->*select_var_aspiration_heuristic_ptr)();
        if (v >= 1)
        {
            return v;
        }
    }

    if (decision_perform_clause_weight_scheme)
    {
        (this->*clause_weight_scheme_ptr)();
    }

    c = (this->*select_unsat_clause_heuristic_div_ptr)();
    v = (this->*select_var_from_clause_heuristic_div_ptr)(c);

    return v;
}

void Solver::flip(int flipvar)
{
    int c, v;
    lit *clause_c;
    lit *p;
    lit *q;

    cur_soln[flipvar] = 1 - cur_soln[flipvar];

    for (q = var_lit[flipvar]; (c = q->clause_num) != -1; q++)
    {
        clause_c = clause_lit[c];
        if (cur_soln[flipvar] == q->sense)
        {
            sat_count[c]++;

            if (sat_count[c] == 1) // sat_count from 0 to 1
            {
                sat_var[c] = flipvar; //record the only true lit's var
                sat(c);
            }
        }
        else // cur_soln[flipvar] != cur_lit.sense
        {
            sat_count[c]--;
            if (sat_count[c] == 1) //sat_count from 2 to 1
            {
                for (p = clause_c; (v = p->var_num) != 0; p++)
                {
                    if (p->sense == cur_soln[v])
                    {
                        sat_var[c] = v;
                        break;
                    }
                }
            }
            else if (sat_count[c] == 0) //sat_count from 1 to 0
            {
                unsat(c);
            } //end else if

        } //end else
    }
}

void Solver::update(int flipvar, long long step)
{
    (this->*update_time_stamp_ptr)(flipvar, step);
    (this->*update_score_ptr)(flipvar);
    (this->*update_break_score_ptr)(flipvar);
    (this->*update_make_score_ptr)(flipvar);
    (this->*update_sat_var2_ptr)(flipvar);
    (this->*update_score2_ptr)(flipvar);
    (this->*update_nvcc_conf_change_ptr)(flipvar);
    (this->*update_cscc_conf_change_ptr)(flipvar);
    (this->*update_nvccdvar_related_in_update_function_ptr)(flipvar);
}

// from heuristics:
void Solver::init_solution_random()
{
    int v;
    for (v = 1; v <= num_vars; v++)
    {
        if (rng.next(2) == 0)
            cur_soln[v] = 0;
        else
            cur_soln[v] = 1;
    }
    return;
}

void Solver::init_solution_all_true_or_false()
{
    int init_assign_value, v;
    if (all_pos_lit_count > all_neg_lit_count)
        init_assign_value = 1;
    else
        init_assign_value = 0;
    for (v = 1; v <= num_vars; v++)
        cur_soln[v] = init_assign_value;
    return;
}

void Solver::init_solution_each_lit_true_or_false()
{
    int v;
    for (v = 1; v <= num_vars; v++)
    {
        if (var_pos_lit_count[v] > var_neg_lit_count[v])
            cur_soln[v] = 1;
        else
            cur_soln[v] = 0;
    }
    return;
}

void Solver::init_unsatvar_related()
{
    int v;
    unsatvar_stack_fill_pointer = 0;
    for (v = 1; v <= num_vars; v++)
        unsatvar_app_count[v] = 0;
}

void Solver::update_unsatvar_related_in_unsat_function(int clause)
{
    int v;
    lit *p = clause_lit[clause];

    for (; (v = p->var_num) != 0; p++)
    {
        unsatvar_app_count[v]++;
        if (unsatvar_app_count[v] == 1)
        {
            index_in_unsatvar_stack[v] = unsatvar_stack_fill_pointer;
            push(v, unsatvar_stack, unsatvar_stack_fill_pointer);
        }
    }
    return;
}

void Solver::update_unsatvar_related_in_sat_function(int clause)
{
    int v, last_unsatvar, index;
    lit *p = clause_lit[clause];

    for (; (v = p->var_num) != 0; p++)
    {
        unsatvar_app_count[v]--;
        if (unsatvar_app_count[v] == 0)
        {
            last_unsatvar = pop(unsatvar_stack, unsatvar_stack_fill_pointer);
            index = index_in_unsatvar_stack[v];
            unsatvar_stack[index] = last_unsatvar;
            index_in_unsatvar_stack[last_unsatvar] = index;
        }
    }
    return;
}

void Solver::init_nvccdvar_related()
{
    int v;
    nvccdvar_stack_fill_pointer = 0;
    for (v = 1; v <= num_vars; v++)
    {
        if (score[v] > 0 && nvcc_conf_change[v] == 1)
        {
            already_in_nvccdvar_stack[v] = true;
            push(v, nvccdvar_stack, nvccdvar_stack_fill_pointer);
        }
        else
            already_in_nvccdvar_stack[v] = false;
    }
    return;
}

void Solver::update_nvccdvar_related_in_update_function(int flipvar)
{
    int i, v, *p;
    for (i = nvccdvar_stack_fill_pointer - 1; i >= 0; i--)
    {
        v = nvccdvar_stack[i];
        if (score[v] <= 0 || nvcc_conf_change[v] == 0)
        {
            nvccdvar_stack[i] = pop(nvccdvar_stack, nvccdvar_stack_fill_pointer);
            already_in_nvccdvar_stack[v] = false;
        }
    }

    for (p = var_neighbor[flipvar]; (v = *p) != 0; p++)
    {
        if (score[v] > 0 && !already_in_nvccdvar_stack[v])
        {
            push(v, nvccdvar_stack, nvccdvar_stack_fill_pointer);
            already_in_nvccdvar_stack[v] = true;
        }
    }
    return;
}

void Solver::update_nvccdvar_related_in_swt_scheme()
{
    int v, i;
    for (i = 0; i < unsatvar_stack_fill_pointer; i++)
    {
        v = unsatvar_stack[i];
        if (score[v] > 0 && nvcc_conf_change[v] == 1 && !already_in_nvccdvar_stack[v])
        {
            push(v, nvccdvar_stack, nvccdvar_stack_fill_pointer);
            already_in_nvccdvar_stack[v] = true;
        }
    }
    return;
}

void Solver::update_nvccdvar_related_in_dec_paws_scheme()
{
    int c, v, i;
    for (i = 0; i < large_weight_clauses_count; i++)
    {
        c = large_weight_clauses[i];
        if (sat_count[c] == 1)
        {
            v = sat_var[c];
            if (score[v] > 0 && nvcc_conf_change[v] == 1 && !already_in_nvccdvar_stack[v])
            {
                push(v, nvccdvar_stack, nvccdvar_stack_fill_pointer);
                already_in_nvccdvar_stack[v] = true;
            }
        }
    }
    return;
}

void Solver::update_nvccdvar_related_in_inc_paws_scheme()
{
    int v, i;
    for (i = 0; i < unsatvar_stack_fill_pointer; i++)
    {
        v = unsatvar_stack[i];
        if (score[v] > 0 && nvcc_conf_change[v] == 1 && !already_in_nvccdvar_stack[v])
        {
            push(v, nvccdvar_stack, nvccdvar_stack_fill_pointer);
            already_in_nvccdvar_stack[v] = true;
        }
    }
    return;
}

void Solver::init_sat_var2()
{
    int c, v, i;
    bool flag;

    for (c = 0; c < num_clauses; c++)
    {
        if (sat_count[c] == 2)
        {
            for (i = 0; i < clause_lit_count[c]; i++)
            {
                v = clause_lit[c][i].var_num;
                if (clause_lit[c][i].sense == cur_soln[v])
                {
                    sat_var[c] = v;
                    break;
                }
            }
            for (i++; i < clause_lit_count[c]; i++)
            {
                v = clause_lit[c][i].var_num;
                if (clause_lit[c][i].sense == cur_soln[v])
                {
                    sat_var2[c] = v;
                    break;
                }
            }
        }
    }
    return;
}

void Solver::update_sat_var2(int flipvar)
{
    int c, v;
    lit *clause_c;
    lit *p;
    lit *q;

    for (q = var_lit[flipvar]; (c = q->clause_num) != -1; q++)
    {
        clause_c = clause_lit[c];
        if (cur_soln[flipvar] == q->sense)
        {
            if (sat_count[c] == 2)
            {
                sat_var2[c] = flipvar;
            }
        }
        else
        {
            if (sat_count[c] == 2)
            {
                for (p = clause_c; (v = p->var_num) != 0; p++)
                {
                    if (p->sense == cur_soln[v])
                    {
                        sat_var[c] = v;
                        break;
                    }
                }
                for (p++; (v = p->var_num) != 0; p++)
                {
                    if (p->sense == cur_soln[v])
                    {
                        sat_var2[c] = v;
                        break;
                    }
                }
            }
        }
    }
}

void Solver::init_score2()
{
    int c, v, i;

    for (v = 1; v <= num_vars; v++)
        score2[v] = 0;

    for (c = 0; c < num_clauses; c++)
    {
        if (sat_count[c] == 1)
        {
            for (i = 0; i < clause_lit_count[c]; i++)
            {
                v = clause_lit[c][i].var_num;
                if (v != sat_var[c])
                    score2[v]++;
            }
        }
        else if (sat_count[c] == 2)
        {
            for (i = 0; i < clause_lit_count[c]; i++)
            {
                v = clause_lit[c][i].var_num;
                if (clause_lit[c][i].sense == cur_soln[v])
                    score2[v]--;
            }
        }
    }

    score2[0] = 0;
    return;
}

void Solver::update_score2(int flipvar)
{
    int c, v;
    lit *clause_c;
    lit *p;
    lit *q;

    for (q = var_lit[flipvar]; (c = q->clause_num) != -1; q++)
    {
        clause_c = clause_lit[c];
        if (cur_soln[flipvar] == q->sense)
        {
            if (sat_count[c] == 3)
            {
                score2[sat_var[c]]++;
                score2[sat_var2[c]]++;
            }
            else if (sat_count[c] == 2)
            {
                score2[flipvar]--;
                for (p = clause_c; (v = p->var_num) != 0; p++)
                    score2[v]--;
            }
            else if (sat_count[c] == 1)
            {
                score2[flipvar]--;
                for (p = clause_c; (v = p->var_num) != 0; p++)
                    score2[v]++;
            }
        }
        else
        {
            if (sat_count[c] == 2)
            {
                score2[sat_var[c]]--;
                score2[sat_var2[c]]--;
            }
            else if (sat_count[c] == 1)
            {
                score2[flipvar]++;
                for (p = clause_c; (v = p->var_num) != 0; p++)
                    score2[v]++;
            }
            else if (sat_count[c] == 0)
            {
                score2[flipvar]++;
                for (p = clause_c; (v = p->var_num) != 0; p++)
                    score2[v]--;
            }
        }
    }
    return;
}

//for time_stamp
void Solver::init_time_stamp()
{
    int v;
    for (v = 1; v <= num_vars; v++)
        time_stamp[v] = 0;
    time_stamp[0] = 0;
    return;
}

void Solver::update_time_stamp(int flipvar, long long step)
{
    time_stamp[flipvar] = step;
}

//for score
void Solver::init_score()
{
    int v, c, i;
    for (v = 1; v <= num_vars; v++)
    {
        score[v] = 0;
        for (i = 0; i < var_lit_count[v]; i++)
        {
            c = var_lit[v][i].clause_num;
            if (sat_count[c] == 0)
                score[v] += clause_weight[c];
            else if (sat_count[c] == 1 && var_lit[v][i].sense == cur_soln[v])
                score[v] -= clause_weight[c];
        }
    }

    score[0] = 0;
    return;
}

void Solver::update_score(int flipvar)
{
    int c, v;
    lit *clause_c;
    lit *p;
    lit *q;

    //update related clauses and neighbor vars
    for (q = var_lit[flipvar]; (c = q->clause_num) != -1; q++)
    {
        clause_c = clause_lit[c];
        if (cur_soln[flipvar] == q->sense)
        {
            if (sat_count[c] == 2) //sat_count from 1 to 2
            {
                score[sat_var[c]] += clause_weight[c];
            }
            else if (sat_count[c] == 1) // sat_count from 0 to 1
            {
                score[sat_var[c]] -= clause_weight[c];

                for (p = clause_c; (v = p->var_num) != 0; p++)
                {
                    score[v] -= clause_weight[c];
                }
            }
        }
        else // cur_soln[flipvar] != cur_lit.sense
        {
            if (sat_count[c] == 1) //sat_count from 2 to 1
            {
                score[sat_var[c]] -= clause_weight[c];
            }
            else if (sat_count[c] == 0) //sat_count from 1 to 0
            {
                for (p = clause_c; (v = p->var_num) != 0; p++)
                {
                    score[v] += clause_weight[c];
                }
                score[flipvar] += clause_weight[c];
            } //end else if
        }     //end else
    }
}

//for break_score
void Solver::init_break_score()
{
    int v, c, i;

    for (v = 1; v <= num_vars; v++)
    {
        break_score[v] = 0;
        for (i = 0; i < var_lit_count[v]; i++)
        {
            c = var_lit[v][i].clause_num;
            if (sat_count[c] == 1 && var_lit[v][i].sense == cur_soln[v])
                break_score[v] += clause_weight[c];
        }
    }

    break_score[0] = 0;
    return;
}

void Solver::update_break_score(int flipvar)
{
    int c, v;
    lit *clause_c;
    lit *p;
    lit *q;

    //update related clauses and neighbor vars
    for (q = var_lit[flipvar]; (c = q->clause_num) != -1; q++)
    {
        clause_c = clause_lit[c];
        if (cur_soln[flipvar] == q->sense)
        {
            if (sat_count[c] == 2) //sat_count from 1 to 2
            {
                break_score[sat_var[c]] -= clause_weight[c];
            }
            else if (sat_count[c] == 1) // sat_count from 0 to 1
            {
                break_score[sat_var[c]] += clause_weight[c];
            }
        }
        else // cur_soln[flipvar] != cur_lit.sense
        {
            if (sat_count[c] == 1) //sat_count from 2 to 1
            {
                break_score[sat_var[c]] += clause_weight[c];
            }
            else if (sat_count[c] == 0) //sat_count from 1 to 0
            {
                break_score[flipvar] -= clause_weight[c];
            } //end else if
        }     //end else
    }
}

//for make_score
void Solver::init_make_score()
{
    int v, c, i;

    for (v = 1; v <= num_vars; v++)
    {
        make_score[v] = 0;
        for (i = 0; i < var_lit_count[v]; i++)
        {
            c = var_lit[v][i].clause_num;
            if (sat_count[c] == 0)
                make_score[v] += clause_weight[c];
        }
    }
}

void Solver::update_make_score(int flipvar)
{
    int c, v;
    lit *clause_c;
    lit *p;
    lit *q;

    //update related clauses and neighbor vars
    for (q = var_lit[flipvar]; (c = q->clause_num) != -1; q++)
    {
        clause_c = clause_lit[c];
        if (cur_soln[flipvar] == q->sense)
        {
            if (sat_count[c] == 1) // sat_count from 0 to 1
            {
                for (p = clause_c; (v = p->var_num) != 0; p++)
                {
                    make_score[v] -= clause_weight[c];
                }
            }
        }
        else // cur_soln[flipvar] != cur_lit.sense
        {
            if (sat_count[c] == 0) //sat_count from 1 to 0
            {
                for (p = clause_c; (v = p->var_num) != 0; p++)
                {
                    make_score[v] += clause_weight[c];
                }
            } //end else if
        }     //end else
    }
}

//for nvcc_conf_change
void Solver::build_neighbor_relation()
{
    int i, j, count;
    int v, c;
    int malloc_adding_length = 10;
    int malloc_var_length = num_vars + malloc_adding_length;

    vector<int> temp_neighbor;
    temp_neighbor.clear();
    vector<int>().swap(temp_neighbor);

    int *neighbor_flag = new int[malloc_var_length];
    for (v = 1; v <= num_vars; ++v)
    {
        neighbor_flag[v] = 0;
    }
    neighbor_flag[0] = 0;

    for (v = 1; v <= num_vars; ++v)
    {
        var_neighbor_count[v] = 0;
        neighbor_flag[v] = 1;
        temp_neighbor.clear();
        vector<int>().swap(temp_neighbor);

        for (i = 0; i < var_lit_count[v]; ++i)
        {
            c = var_lit[v][i].clause_num;
            for (j = 0; j < clause_lit_count[c]; ++j)
            {
                if (neighbor_flag[clause_lit[c][j].var_num] == 0)
                {
                    var_neighbor_count[v]++;
                    neighbor_flag[clause_lit[c][j].var_num] = 1;
                    temp_neighbor.push_back(clause_lit[c][j].var_num);
                }
            }
        }
        neighbor_flag[v] = 0;

        var_neighbor[v] = new int[var_neighbor_count[v] + 1];
        count = 0;
        for (i = 0; i < temp_neighbor.size(); i++)
        {
            var_neighbor[v][count++] = temp_neighbor[i];
            neighbor_flag[temp_neighbor[i]] = 0;
        }
        var_neighbor[v][count] = 0;
        var_neighbor_count[v] = count;

        //cout<<"c var_neighbor_count["<<v<<"] = "<<var_neighbor_count[v]<<endl;
    }

    delete[] neighbor_flag;
    return;
}

void Solver::init_nvcc_conf_change()
{
    int v;
    for (v = 1; v <= num_vars; v++)
    {
        nvcc_conf_change[v] = 1;
    }
    nvcc_conf_change[0] = 0;
}

void Solver::update_nvcc_conf_change(int flipvar)
{
    int v;
    int *np = var_neighbor[flipvar];
    for (; (v = *np) != 0; np++)
    {
        nvcc_conf_change[v] = 1;
    }
    //update information of flipvar
    nvcc_conf_change[flipvar] = 0;
}

void Solver::init_cscc_conf_change()
{
    int v;
    for (v = 1; v <= num_vars; v++)
    {
        cscc_conf_change[v] = 1;
    }
    cscc_conf_change[0] = 0;
}

void Solver::update_cscc_conf_change(int flipvar)
{
    int v, c;
    lit *clause_c, *q, *p;

    for (q = var_lit[flipvar]; (c = q->clause_num) != -1; q++)
    {
        clause_c = clause_lit[c];
        if (cur_soln[flipvar] == q->sense)
        {
            if (sat_count[c] == 1)
            {
                for (p = clause_c; (v = p->var_num) != 0; p++)
                    cscc_conf_change[v] = 1;
            }
        }
        else
        {
            if (sat_count[c] == 0)
            {
                for (p = clause_c; (v = p->var_num) != 0; p++)
                    cscc_conf_change[v] = 1;
            }
        }
    }

    cscc_conf_change[flipvar] = 0;
    return;
}

int Solver::select_var_pac_heuristic()
{
    int c = rng.next(num_clauses);
    return clause_lit[c][rng.next(clause_lit_count[c])].var_num;
}

int Solver::select_unsat_clause_random_heuristic()
{
    return unsat_stack[rng.next(unsat_stack_fill_pointer)];
}

int Solver::select_unsat_clause_weight_distribution_heuristic()
{
    long long random_value;
    double random_prob;
    long long value_sum = 0;
    int i, c;

    if (unsat_stack_fill_pointer == 1)
        return unsat_stack[0];

    if (total_unsat_clause_weight < maximum_clause_weight_limit)
    {
        random_value = rng.next(total_unsat_clause_weight) + 1;
    }
    else
    {
        random_prob = rng.nextClosed();
        random_value = (total_unsat_clause_weight - 1) * random_prob + 1;
        if (random_value < 1)
            random_value = 1;
        else if (random_value > total_unsat_clause_weight)
            random_value = total_unsat_clause_weight;
    }

    c = -1;
    value_sum = 0;
    for (i = 0; i < unsat_stack_fill_pointer; i++)
    {
        c = unsat_stack[i];
        value_sum += clause_weight[c];
        if (random_value <= value_sum)
            break;
    }
    if (c == -1)
        c = unsat_stack[rng.next(unsat_stack_fill_pointer)];
    return c;
}

int Solver::select_var_from_clause_random_heuristic(int clause_num)
{
    int c = clause_num;
    return clause_lit[c][rng.next(clause_lit_count[c])].var_num;
}

int Solver::select_var_from_clause_greatest_score_heuristic(int clause_num)
{
    int c = clause_num;
    int v, i, best_v;

    best_v = clause_lit[c][0].var_num;
    for (i = 1; i < clause_lit_count[c]; i++)
    {
        v = clause_lit[c][i].var_num;
        if (score[v] > score[best_v])
            best_v = v;
        else if (score[v] == score[best_v] && time_stamp[v] < time_stamp[best_v])
            best_v = v;
    }
    return best_v;
}

int Solver::select_var_from_clause_greatest_age_heuristic(int clause_num)
{
    int c = clause_num;
    int v, i, best_v;

    best_v = clause_lit[c][0].var_num;
    for (i = 1; i < clause_lit_count[c]; i++)
    {
        v = clause_lit[c][i].var_num;
        if (time_stamp[v] < time_stamp[best_v])
            best_v = v;
    }
    return best_v;
}

int Solver::select_var_from_clause_novelty_heuristic(int clause_num)
{
    int c = clause_num;
    int v, i, best_v, second_best_v;
    long long greatest_time_stamp;

    if (clause_lit_count[c] == 1)
        return clause_lit[c][0].var_num;

    best_v = clause_lit[c][0].var_num;
    greatest_time_stamp = time_stamp[clause_lit[c][0].var_num];
    for (i = 1; i < clause_lit_count[c]; i++)
    {
        v = clause_lit[c][i].var_num;
        if (greatest_time_stamp < time_stamp[v])
            greatest_time_stamp = time_stamp[v];

        if (score[v] > score[best_v])
            best_v = v;
        else if (score[v] == score[best_v] && time_stamp[v] < time_stamp[best_v])
            best_v = v;
    }

    second_best_v = -1;
    for (i = 0; i < clause_lit_count[c]; i++)
    {
        v = clause_lit[c][i].var_num;
        if (v != best_v)
        {
            second_best_v = v;
            break;
        }
    }
    for (i++; i < clause_lit_count[c]; i++)
    {
        v = clause_lit[c][i].var_num;
        if (v == best_v)
            continue;
        if (score[v] > score[second_best_v])
            second_best_v = v;
        else if (score[v] == score[second_best_v] && time_stamp[v] < time_stamp[second_best_v])
            second_best_v = v;
    }

    if (second_best_v == -1)
        return best_v;

    if (time_stamp[best_v] == greatest_time_stamp)
    {
        if (score[best_v] == 0 || rng.nextClosed() < para_prob_novelty)
            return second_best_v;
    }

    return best_v;
}

int Solver::select_var_from_clause_sparrow_heuristic(int clause_num)
{
    int c = clause_num;
    int v, i;
    double sparrow_score;
    double sparrow_age;
    double age_v;
    double sum_prob_value = 0.0;
    double random_prob;
    double random_prob_value;

    sum_prob_value = 0.0;
    for (i = 0; i < clause_lit_count[c]; i++)
    {
        v = clause_lit[c][i].var_num;
        if (score[v] < 0)
            sparrow_score = pow(para_sparrow_c1, score[v]);
        else
            sparrow_score = 1.0;
        age_v = step - time_stamp[v];
        sparrow_age = 1.0 + pow((double)age_v / (double)para_sparrow_c3, para_sparrow_c2);
        var_prob_value[v] = sparrow_score * sparrow_age;
        sum_prob_value += var_prob_value[v];
    }

    random_prob = rng.nextClosed();
    random_prob_value = random_prob * sum_prob_value;

    sum_prob_value = 0;
    for (i = 0; i < clause_lit_count[c]; i++)
    {
        v = clause_lit[c][i].var_num;
        sum_prob_value += var_prob_value[v];
        if (sum_prob_value >= random_prob_value)
            return v;
    }

    return clause_lit[c][rng.next(clause_lit_count[c])].var_num;
}

int Solver::select_var_from_clause_probsat_heuristic(int clause_num)
{
    int c = clause_num;
    int v, i;
    double sum_prob_value = 0.0;
    double random_prob;
    double random_prob_value;

    sum_prob_value = 0.0;
    for (i = 0; i < clause_lit_count[c]; i++)
    {
        v = clause_lit[c][i].var_num;
        var_prob_value[v] = pow(para_probsat_cb, -break_score[v]);
        sum_prob_value += var_prob_value[v];
    }

    random_prob = rng.nextClosed();
    random_prob_value = random_prob * sum_prob_value;

    sum_prob_value = 0;
    for (i = 0; i < clause_lit_count[c]; i++)
    {
        v = clause_lit[c][i].var_num;
        sum_prob_value += var_prob_value[v];
        if (sum_prob_value >= random_prob_value)
            return v;
    }

    return clause_lit[c][rng.next(clause_lit_count[c])].var_num;
}

int Solver::select_var_cscc_heuristic_from_nvccdvar_stack()
{
    int i, v, flipvar;
    long long best_score = -1;

    candidate_var_stack_fill_pointer = 0;
    for (i = 0; i < nvccdvar_stack_fill_pointer; i++)
    {
        v = nvccdvar_stack[i];
        if (cscc_conf_change[v] == 1)
        {
            candidate_var_stack[0] = v;
            candidate_var_stack_fill_pointer = 1;
            best_score = score[v];
            break;
        }
    }

    for (i++; i < nvccdvar_stack_fill_pointer; i++)
    {
        v = nvccdvar_stack[i];
        if (cscc_conf_change[v] == 0)
            continue;
        if (score[v] > best_score)
        {
            candidate_var_stack[0] = v;
            candidate_var_stack_fill_pointer = 1;
            best_score = score[v];
        }
        else if (score[v] == best_score)
            candidate_var_stack[candidate_var_stack_fill_pointer++] = v;
    }

    if (candidate_var_stack_fill_pointer == 0)
        return -1;
    if (best_score <= 0)
        return -1;
    flipvar = (this->*select_var_by_break_tie_in_set_ptr)(candidate_var_stack, candidate_var_stack_fill_pointer);
    return flipvar;
}

int Solver::select_var_nvcc_heuristic_from_nvccdvar_stack()
{
    int i, v, flipvar;
    long long best_score = -1;

    if (nvccdvar_stack_fill_pointer == 0)
        return -1;

    v = nvccdvar_stack[0];
    candidate_var_stack[0] = v;
    candidate_var_stack_fill_pointer = 1;
    best_score = score[v];

    for (i = 1; i < nvccdvar_stack_fill_pointer; i++)
    {
        v = nvccdvar_stack[i];
        if (score[v] > best_score)
        {
            candidate_var_stack[0] = v;
            candidate_var_stack_fill_pointer = 1;
            best_score = score[v];
        }
        else if (score[v] == best_score)
            candidate_var_stack[candidate_var_stack_fill_pointer++] = v;
    }

    if (best_score <= 0)
        return -1;
    flipvar = (this->*select_var_by_break_tie_in_set_ptr)(candidate_var_stack, candidate_var_stack_fill_pointer);
    return flipvar;
}

int Solver::select_var_aspiration_heuristic()
{
    int i, v, flipvar;
    long long sig_score = avg_clause_weight;
    long long best_score = -1;

    candidate_var_stack_fill_pointer = 0;
    for (i = 0; i < unsatvar_stack_fill_pointer; i++)
    {
        v = unsatvar_stack[i];
        if (score[v] > sig_score)
        {
            candidate_var_stack[0] = v;
            candidate_var_stack_fill_pointer = 1;
            best_score = score[v];
            break;
        }
    }

    for (i++; i < unsatvar_stack_fill_pointer; i++)
    {
        v = unsatvar_stack[i];
        if (score[v] > best_score)
        {
            candidate_var_stack[0] = v;
            candidate_var_stack_fill_pointer = 1;
            best_score = score[v];
        }
        else if (score[v] == best_score)
            candidate_var_stack[candidate_var_stack_fill_pointer++] = v;
    }

    if (candidate_var_stack_fill_pointer == 0)
        return -1;
    if (best_score <= sig_score)
        return -1;
    flipvar = (this->*select_var_by_break_tie_in_set_ptr)(candidate_var_stack, candidate_var_stack_fill_pointer);
    return flipvar;
}

int Solver::select_var_by_random_break_tie_in_set(int *the_set, int the_set_length)
{
    if (the_set_length <= 0)
        return -1;
    if (the_set_length == 1)
        return the_set[0];
    return the_set[rng.next(the_set_length)];
}

int Solver::select_var_by_greatest_age_break_tie_in_set(int *the_set, int the_set_length)
{
    int i, v, best_v;

    if (the_set_length <= 0)
        return -1;
    if (the_set_length == 1)
        return the_set[0];

    best_v = the_set[0];
    for (i = 1; i < the_set_length; i++)
    {
        v = the_set[i];
        if (time_stamp[v] < time_stamp[best_v])
            best_v = v;
    }
    return best_v;
}

int Solver::select_var_by_greatest_hscore_break_tie_in_set(int *the_set, int the_set_length)
{
    int i, v, best_v;
    long long hscore_best_v, hscore_v;

    if (the_set_length <= 0)
        return -1;
    if (the_set_length == 1)
        return the_set[0];

    best_v = the_set[0];
    hscore_best_v = calc_hscore(best_v);
    for (i = 1; i < the_set_length; i++)
    {
        v = the_set[i];
        hscore_v = calc_hscore(v);
        if (hscore_v > hscore_best_v)
        {
            best_v = v;
            hscore_best_v = hscore_v;
        }
        else if (hscore_v == hscore_best_v && time_stamp[v] < time_stamp[best_v])
            best_v = v;
    }
    return best_v;
}

int Solver::select_var_from_clause_greatest_hscore_heuristic(int clause_num)
{
    int c = clause_num;
    int v, i, best_v;
    long long hscore_best_v, hscore_v;

    best_v = clause_lit[c][0].var_num;
    hscore_best_v = calc_hscore(best_v);

    for (i = 1; i < clause_lit_count[c]; i++)
    {
        v = clause_lit[c][i].var_num;
        hscore_v = calc_hscore(v);
        if (hscore_v > hscore_best_v)
        {
            best_v = v;
            hscore_best_v = hscore_v;
        }
        else if (hscore_v == hscore_best_v && time_stamp[v] < time_stamp[best_v])
            best_v = v;
    }
    return best_v;
}

int Solver::select_var_by_greatest_hscore2_break_tie_in_set(int *the_set, int the_set_length)
{
    int i, v, best_v;
    long long hscore2_best_v, hscore2_v;

    if (the_set_length <= 0)
        return -1;
    if (the_set_length == 1)
        return the_set[0];

    best_v = the_set[0];
    hscore2_best_v = calc_hscore2(best_v);
    for (i = 1; i < the_set_length; i++)
    {
        v = the_set[i];
        hscore2_v = calc_hscore2(v);
        if (hscore2_v > hscore2_best_v)
        {
            best_v = v;
            hscore2_best_v = hscore2_v;
        }
        else if (hscore2_v == hscore2_best_v && time_stamp[v] < time_stamp[best_v])
            best_v = v;
    }
    return best_v;
}

int Solver::select_var_from_clause_greatest_hscore2_heuristic(int clause_num)
{
    int c = clause_num;
    int v, i, best_v;
    long long hscore2_best_v, hscore2_v;

    best_v = clause_lit[c][0].var_num;
    hscore2_best_v = calc_hscore2(best_v);

    for (i = 1; i < clause_lit_count[c]; i++)
    {
        v = clause_lit[c][i].var_num;
        hscore2_v = calc_hscore2(v);
        if (hscore2_v > hscore2_best_v)
        {
            best_v = v;
            hscore2_best_v = hscore2_v;
        }
        else if (hscore2_v == hscore2_best_v && time_stamp[v] < time_stamp[best_v])
            best_v = v;
    }
    return best_v;
}

void Solver::init_clause_weight_scheme_swt()
{
    scale_avg = (para_clause_weight_threshold_swt + 1) * para_q_scale_swt;
}

void Solver::smooth_clause_weight_swt()
{
    int c, v, i;
    long long new_total_weight = 0;
    for (v = 1; v <= num_vars; v++)
        score[v] = 0;

    total_unsat_clause_weight = 0;
    for (c = 0; c < num_clauses; c++)
    {
        clause_weight[c] = clause_weight[c] * para_p_scale_swt + scale_avg;
        if (clause_weight[c] < 1)
            clause_weight[c] = 1;
        new_total_weight += clause_weight[c];
        if (sat_count[c] == 0)
        {
            total_unsat_clause_weight += clause_weight[c];
            for (i = 0; i < clause_lit_count[c]; i++)
                score[clause_lit[c][i].var_num] += clause_weight[c];
        }
        else if (sat_count[c] == 1)
            score[sat_var[c]] -= clause_weight[c];
    }

    avg_clause_weight = new_total_weight / num_clauses;
    delta_total_clause_weight = new_total_weight - avg_clause_weight * num_clauses;
    return;
}

void Solver::clause_weight_scheme_swt()
{
    int i, v;
    for (i = 0; i < unsat_stack_fill_pointer; i++)
        clause_weight[unsat_stack[i]]++;

    total_unsat_clause_weight += unsat_stack_fill_pointer;

    for (i = 0; i < unsatvar_stack_fill_pointer; i++)
    {
        v = unsatvar_stack[i];
        score[v] += unsatvar_app_count[v];
    }

    (this->*update_nvccdvar_related_in_swt_scheme_ptr)();

    delta_total_clause_weight += unsat_stack_fill_pointer;
    if (delta_total_clause_weight > num_clauses)
    {
        avg_clause_weight++;
        delta_total_clause_weight -= num_clauses;

        if (avg_clause_weight > para_clause_weight_threshold_swt)
        {
            smooth_clause_weight_swt();
        }
    }
}

void Solver::init_clause_weight_scheme_paws()
{
    large_weight_clauses_count = 0;
    large_clause_weight_count_threshold = 10;
}

void Solver::dec_clause_weight_paws()
{
    int c, v, i;
    for (i = 0; i < large_weight_clauses_count; i++)
    {
        c = large_weight_clauses[i];
        if (sat_count[c] > 0)
        {
            delta_total_clause_weight -= clause_weight[c];
            clause_weight[c]--;

            if (clause_weight[c] < 1)
                clause_weight[c] = 1;

            delta_total_clause_weight += clause_weight[c];
            if (delta_total_clause_weight < 0)
            {
                avg_clause_weight--;
                delta_total_clause_weight += num_clauses;
            }

            if (clause_weight[c] == 1)
            {
                large_weight_clauses[i] = large_weight_clauses[--large_weight_clauses_count];
                i--;
            }
            if (sat_count[c] == 1)
            {
                v = sat_var[c];
                score[v]++;
            }
        }
    }

    (this->*update_nvccdvar_related_in_dec_paws_scheme_ptr)();
    return;
}

void Solver::inc_clause_weight_paws()
{
    int c, v, i;
    for (i = 0; i < unsat_stack_fill_pointer; i++)
    {
        c = unsat_stack[i];
        clause_weight[c]++;
        if (clause_weight[c] == 2)
            large_weight_clauses[large_weight_clauses_count++] = c;
    }

    total_unsat_clause_weight += unsat_stack_fill_pointer;

    delta_total_clause_weight += unsat_stack_fill_pointer;
    if (delta_total_clause_weight > num_clauses)
    {
        avg_clause_weight++;
        delta_total_clause_weight -= num_clauses;
    }

    for (i = 0; i < unsatvar_stack_fill_pointer; i++)
    {
        v = unsatvar_stack[i];
        score[v] += unsatvar_app_count[v];
    }

    (this->*update_nvccdvar_related_in_inc_paws_scheme_ptr)();
    return;
}

void Solver::clause_weight_scheme_paws()
{
    if (rng.nextClosed() < para_smooth_probability_paws && large_weight_clauses_count > large_clause_weight_count_threshold)
        dec_clause_weight_paws();
    else
        inc_clause_weight_paws();
    return;
}

string Solver::get_last_level_name(string file_path)
{
    long long file_path_size = file_path.size();
    if (file_path[file_path_size - 1] == '/')
        file_path = file_path.substr(0, file_path_size - 1);
    long long pos = file_path.find_last_of('/');
    if (pos < 0 || pos >= file_path.size())
        return file_path;
    else
        return file_path.substr(pos + 1, file_path.size() - pos - 1);
}

void Solver::print_general_information(char *executable_name)
{
    string executable_name_str = executable_name;
    executable_name_str = get_last_level_name(executable_name_str);
    cout << "c" << endl;
    cout << "c This is " << executable_name_str << " (a local_search SAT solver)" << endl;
    cout << "c Version: " << version_str << endl;
    cout << "c" << endl;
    return;
}

int Solver::get_default_init_solution()
{
    if (init_solution_ptr == &Solver::init_solution_random)
        return 1;
    else if (init_solution_ptr == &Solver::init_solution_all_true_or_false)
        return 2;
    else if (init_solution_ptr == &Solver::init_solution_each_lit_true_or_false)
        return 3;
    else
        return 1;
}

int Solver::get_default_sel_clause_first_div()
{
    if (!decision_perform_first_div)
        return 1;
    else
    {
        if (select_unsat_clause_heuristic_first_div_ptr == &Solver::select_unsat_clause_random_heuristic)
            return 1;
        else if (select_unsat_clause_heuristic_first_div_ptr == &Solver::select_unsat_clause_weight_distribution_heuristic)
            return 2;
        else
            return 1;
    }
}

int Solver::get_default_sel_var_first_div()
{
    if (!decision_perform_first_div)
        return 1;
    else
    {
        if (select_var_from_clause_heuristic_first_div_ptr == &Solver::select_var_from_clause_random_heuristic)
            return 1;
        else if (select_var_from_clause_heuristic_first_div_ptr == &Solver::select_var_from_clause_greatest_age_heuristic)
            return 2;
        else if (select_var_from_clause_heuristic_first_div_ptr == &Solver::select_var_from_clause_greatest_score_heuristic)
            return 3;
        else if (select_var_from_clause_heuristic_first_div_ptr == &Solver::select_var_from_clause_greatest_hscore_heuristic)
            return 4;
        else if (select_var_from_clause_heuristic_first_div_ptr == &Solver::select_var_from_clause_greatest_hscore2_heuristic)
            return 5;
        else if (select_var_from_clause_heuristic_first_div_ptr == &Solver::select_var_from_clause_novelty_heuristic)
            return 6;
        else if (select_var_from_clause_heuristic_first_div_ptr == &Solver::select_var_from_clause_sparrow_heuristic)
            return 7;
        else if (select_var_from_clause_heuristic_first_div_ptr == &Solver::select_var_from_clause_probsat_heuristic)
            return 8;
        else
            return 1;
    }
}

int Solver::get_default_sel_var_break_tie_greedy()
{
    if (select_var_by_break_tie_in_set_ptr == &Solver::select_var_by_random_break_tie_in_set)
        return 1;
    else if (select_var_by_break_tie_in_set_ptr == &Solver::select_var_by_greatest_age_break_tie_in_set)
        return 2;
    else if (select_var_by_break_tie_in_set_ptr == &Solver::select_var_by_greatest_hscore_break_tie_in_set)
        return 3;
    else if (select_var_by_break_tie_in_set_ptr == &Solver::select_var_by_greatest_hscore2_break_tie_in_set)
        return 4;
    else
        return 1;
}

int Solver::get_default_sel_clause_weight_scheme()
{
    if (!decision_perform_clause_weight_scheme)
        return 1;
    else
    {
        if (init_clause_weight_scheme_ptr == &Solver::init_clause_weight_scheme_swt)
            return 1;
        else if (init_clause_weight_scheme_ptr == &Solver::init_clause_weight_scheme_paws)
            return 2;
        else
            return 1;
    }
}

int Solver::get_default_sel_clause_div()
{
    if (select_unsat_clause_heuristic_div_ptr == &Solver::select_unsat_clause_random_heuristic)
        return 1;
    else if (select_unsat_clause_heuristic_div_ptr == &Solver::select_unsat_clause_weight_distribution_heuristic)
        return 2;
    else
        return 1;
}

int Solver::get_default_sel_var_div()
{
    if (select_var_from_clause_heuristic_div_ptr == &Solver::select_var_from_clause_random_heuristic)
        return 1;
    else if (select_var_from_clause_heuristic_div_ptr == &Solver::select_var_from_clause_greatest_age_heuristic)
        return 2;
    else if (select_var_from_clause_heuristic_div_ptr == &Solver::select_var_from_clause_greatest_score_heuristic)
        return 3;
    else if (select_var_from_clause_heuristic_div_ptr == &Solver::select_var_from_clause_greatest_hscore_heuristic)
        return 4;
    else if (select_var_from_clause_heuristic_div_ptr == &Solver::select_var_from_clause_greatest_hscore2_heuristic)
        return 5;
    else if (select_var_from_clause_heuristic_div_ptr == &Solver::select_var_from_clause_novelty_heuristic)
        return 6;
    else if (select_var_from_clause_heuristic_div_ptr == &Solver::select_var_from_clause_sparrow_heuristic)
        return 7;
    else if (select_var_from_clause_heuristic_div_ptr == &Solver::select_var_from_clause_probsat_heuristic)
        return 8;
    else
        return 1;
}

void Solver::print_usage_info(char *executable_name)
{
    default_algorithm_settings();
    cout << "c" << endl;
    cout << "c Usage: " << executable_name << "   (*Required*)" << endl
         << "c -inst <instance_name>   (*Required*)" << endl
         << "c -seed <seed>   (*Required*)" << endl
         << "c -maxi_flips <maxi_flips>   (Default: " << maxi_flips << ")" << endl
         << "c -init_solution {1, 2, 3}   (Default: " << get_default_init_solution() << ")" << endl
         << "c -perform_pac {0, 1}   (Default: " << (int)decision_perform_pac_heuristic << ")" << endl
         << "c -prob_pac [0, 1]   (Default: " << para_prob_pac << ")" << endl
         << "c -perform_first_div {0, 1}   (Default: " << (int)decision_perform_first_div << ")" << endl
         << "c -prob_first_div [0, 1]   (Default: " << para_prob_first_div << ")" << endl
         << "c -perform_double_cc {0, 1}   (Default: " << (int)decision_perform_double_cc << ")" << endl
         << "c -perform_aspiration {0, 1}   (Default: " << (int)decision_perform_aspiration << ")" << endl
         << "c -sel_var_break_tie_greedy {1, 2, 3, 4}   (Default: " << get_default_sel_var_break_tie_greedy() << ")" << endl
         << "c -d_hscore {int, from 1 to 15}   (Default: " << para_d_hscore << ")" << endl
         << "c -beta_hscore {int, from 100 to 1000000}   (Default: " << para_beta_hscore << ")" << endl
         << "c -gamma_hscore2 {int, from 100 to 1000000}   (Default: " << para_gamma_hscore2 << ")" << endl
         << "c -perform_clause_weight {0, 1}   (Default: " << (int)decision_perform_clause_weight_scheme << ")" << endl
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

void Solver::print_algorithm_settings_information()
{
    cout << "c" << endl;
    cout << "c instance = " << inst << endl;
    cout << "c ori_num_vars = " << ori_num_vars << endl;
    cout << "c ori_num_clauses = " << ori_num_clauses << endl;
    cout << "c maxi_clause_len = " << maxi_clause_len << endl;
    cout << "c mini_clause_len = " << mini_clause_len << endl;
    cout << "c seed = " << seed << endl;
    cout << "c maxi_flips = " << maxi_flips << endl;

    if (init_solution_ptr == &Solver::init_solution_random)
        cout << "c init_solution_ptr = init_solution_random" << endl;
    else if (init_solution_ptr == &Solver::init_solution_all_true_or_false)
        cout << "c init_solution_ptr = init_solution_all_true_or_false" << endl;
    else if (init_solution_ptr == &Solver::init_solution_each_lit_true_or_false)
        cout << "c init_solution_ptr = init_solution_each_lit_true_or_false" << endl;

    if (decision_perform_pac_heuristic)
    {
        cout << "c decision_perform_pac_heuristic = true" << endl;
        if (select_var_pac_heuristic_ptr == &Solver::select_var_pac_heuristic)
            cout << "c select_var_pac_heuristic_ptr = select_var_pac_heuristic" << endl;
        cout << "c prob_pac = " << para_prob_pac << endl;
    }

    if (decision_perform_first_div)
    {
        cout << "c decision_perform_first_div = true" << endl;
        cout << "c prob_first_div = " << para_prob_first_div << endl;
    }

    if (decision_perform_double_cc)
    {
        cout << "c decision_perform_double_cc = true" << endl;
    }

    if (decision_perform_aspiration)
    {
        cout << "c decision_perform_aspiration = true" << endl;
    }

    if (select_var_by_break_tie_in_set_ptr == &Solver::select_var_by_random_break_tie_in_set)
        cout << "c select_var_by_break_tie_in_set_ptr = select_var_by_random_break_tie_in_set" << endl;
    else if (select_var_by_break_tie_in_set_ptr == &Solver::select_var_by_greatest_age_break_tie_in_set)
        cout << "c select_var_by_break_tie_in_set_ptr = select_var_by_greatest_age_break_tie_in_set" << endl;
    else if (select_var_by_break_tie_in_set_ptr == &Solver::select_var_by_greatest_hscore_break_tie_in_set)
    {
        cout << "c select_var_by_break_tie_in_set_ptr = select_var_by_greatest_hscore_break_tie_in_set" << endl;
        cout << "c d_hscore = " << para_d_hscore << endl;
        cout << "c beta_hscore = " << para_beta_hscore << endl;
    }
    else if (select_var_by_break_tie_in_set_ptr == &Solver::select_var_by_greatest_hscore2_break_tie_in_set)
    {
        cout << "c select_var_by_break_tie_in_set_ptr = select_var_by_greatest_hscore2_break_tie_in_set" << endl;
        cout << "c gamma_hscore2 = " << para_gamma_hscore2 << endl;
    }

    if (decision_perform_clause_weight_scheme)
    {
        cout << "c decision_perform_clause_weight_scheme = true" << endl;
        if (init_clause_weight_scheme_ptr == &Solver::init_clause_weight_scheme_swt)
        {
            cout << "c init_clause_weight_scheme_ptr = init_clause_weight_scheme_swt" << endl;
            cout << "c clause_weight_scheme_ptr = clause_weight_scheme_swt" << endl;
            cout << "c threshold_swt = " << para_clause_weight_threshold_swt << endl;
            cout << "c p_swt = " << para_p_scale_swt << endl;
            cout << "c q_swt = " << para_q_scale_swt << endl;
        }
        else if (init_clause_weight_scheme_ptr == &Solver::init_clause_weight_scheme_paws)
        {
            cout << "c init_clause_weight_scheme_ptr = init_clause_weight_scheme_paws" << endl;
            cout << "c clause_weight_scheme_ptr = clause_weight_scheme_paws" << endl;
            cout << "c sp_paws = " << para_smooth_probability_paws << endl;
        }
    }

    if (select_unsat_clause_heuristic_div_ptr == &Solver::select_unsat_clause_random_heuristic)
        cout << "c select_unsat_clause_heuristic_div_ptr = select_unsat_clause_random_heuristic" << endl;
    else if (select_unsat_clause_heuristic_div_ptr == &Solver::select_unsat_clause_weight_distribution_heuristic)
        cout << "c select_unsat_clause_heuristic_div_ptr = select_unsat_clause_weight_distribution_heuristic" << endl;

    if (select_var_from_clause_heuristic_div_ptr == &Solver::select_var_from_clause_random_heuristic)
        cout << "c select_var_from_clause_heuristic_div_ptr = select_var_from_clause_random_heuristic" << endl;
    else if (select_var_from_clause_heuristic_div_ptr == &Solver::select_var_from_clause_greatest_age_heuristic)
        cout << "c select_var_from_clause_heuristic_div_ptr = select_var_from_clause_greatest_age_heuristic" << endl;
    else if (select_var_from_clause_heuristic_div_ptr == &Solver::select_var_from_clause_greatest_score_heuristic)
        cout << "c select_var_from_clause_heuristic_div_ptr = select_var_from_clause_greatest_score_heuristic" << endl;
    else if (select_var_from_clause_heuristic_div_ptr == &Solver::select_var_from_clause_greatest_hscore_heuristic)
    {
        cout << "c select_var_from_clause_heuristic_div_ptr = select_var_from_clause_greatest_hscore_heuristic" << endl;
        cout << "c d_hscore = " << para_d_hscore << endl;
        cout << "c beta_hscore = " << para_beta_hscore << endl;
    }
    else if (select_var_from_clause_heuristic_div_ptr == &Solver::select_var_from_clause_greatest_hscore2_heuristic)
    {
        cout << "c select_var_from_clause_heuristic_div_ptr = select_var_from_clause_greatest_hscore2_heuristic" << endl;
        cout << "c gamma_hscore2 = " << para_gamma_hscore2 << endl;
    }
    else if (select_var_from_clause_heuristic_div_ptr == &Solver::select_var_from_clause_novelty_heuristic)
    {
        cout << "c select_var_from_clause_heuristic_div_ptr = select_var_from_clause_novelty_heuristic" << endl;
        cout << "c prob_novelty = " << para_prob_novelty << endl;
    }
    else if (select_var_from_clause_heuristic_div_ptr == &Solver::select_var_from_clause_sparrow_heuristic)
    {
        cout << "c select_var_from_clause_heuristic_div_ptr = select_var_from_clause_sparrow_heuristic" << endl;
        cout << "c sparrow_c1 = " << para_sparrow_c1 << endl;
        cout << "c sparrow_c2 = " << para_sparrow_c2 << endl;
        cout << "c sparrow_c3 = " << para_sparrow_c3 << endl;
    }
    else if (select_var_from_clause_heuristic_div_ptr == &Solver::select_var_from_clause_probsat_heuristic)
    {
        cout << "c select_var_from_clause_heuristic_div_ptr = select_var_from_clause_probsat_heuristic" << endl;
        cout << "c probsat_cb = " << para_probsat_cb << endl;
    }

    cout << "c" << endl;
    return;
}

void Solver::print_solution()
{
    int v;

    cout << "v";
    for (v = 1; v <= num_vars; v++)
    {
        cout << " ";
        if (cur_soln[v] == 0)
            cout << "-";
        cout << v;
    }
    cout << endl;
    return;
}

void Solver::print_SAT_result()
{
    cout << "s SATISFIABLE" << endl;
    print_solution();
    return;
}

void Solver::print_UNKNOWN_result()
{
    cout << "s UNKNOWN" << endl;
    return;
}

void Solver::print_run_info(double run_time)
{
    cout << "c run time = " << run_time << " second(s)" << endl;
    cout << "c total steps = " << step << " + (" << tries << " * " << maxi_flips << ")" << endl;
    return;
}

#endif