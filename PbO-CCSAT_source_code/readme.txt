Software:       PbO-CCSAT (Programming by Optimisation with Configuration Checking in Local Search for Satisfiability)

Version:        1.2.3



Building Executable:
    make



Usage:
    ./PbO-CCSAT   (*Required*)
        -inst <instance_name>   (*Required*)
        -seed <seed>   (*Required*)
        -maxi_flips <maxi_flips>   (Default: 2000000000)
        -init_solution {1, 2, 3}   (Default: 2)
        -perform_pac {0, 1}   (Default: 0)
        -prob_pac [0, 1]   (Default: 0.01)
        -perform_first_div {0, 1}   (Default: 0)
        -prob_first_div [0, 1]   (Default: 0.05)
        -perform_double_cc {0, 1}   (Default: 1)
        -perform_aspiration {0, 1}   (Default: 1)
        -sel_var_break_tie_greedy {1, 2, 3, 4}   (Default: 2)
        -d_hscore {int, from 1 to 15}   (Default: 8)
        -beta_hscore {int, from 100 to 1000000}   (Default: 1000)
        -gamma_hscore2 {int, from 100 to 1000000}   (Default: 1000)
        -perform_clause_weight {0, 1}   (Default: 1)
        -sel_clause_weight_scheme {1, 2}   (Default: 1)
        -threshold_swt {int, from 10 to 1000}   (Default: 300)
        -p_swt [0, 1]   (Default: 0.3)
        -q_swt [0, 1]   (Default: 0)
        -sp_paws [0, 1]   (Default: 0.8)
        -sel_clause_div {1, 2}   (Default: 1)
        -sel_var_div {1, 2, 3, 4, 5, 6, 7, 8}   (Default: 7)
        -prob_novelty [0, 1]   (Default: 0.2)
        -sparrow_c1 [2, 10]   (Default: 2.15)
        -sparrow_c2 {int, from 1 to 5}   (Default: 4)
        -sparrow_c3 {int, from 20000 to 100000}   (Default: 75000)
        -probsat_cb [2, 10]   (Default: 2.85)




Calling Examples:
[1] For DCCASat's instantiation:
    ./PbO-CCSAT -inst <instance> -seed <seed> -init_solution 1 -perform_pac 0 -perform_first_div 0 -perform_double_cc 1 -perform_aspiration 1 -sel_var_break_tie_greedy 2 -perform_clause_weight 1 -sel_clause_weight_scheme 1 -threshold_swt 300 -p_swt 0.3 -q_swt 0 -sel_var_div 1 -sel_var_div 3

[2] For general structured SAT: 
    ./PbO-CCSAT -inst <instance> -seed <seed> -init_solution 2 -perform_pac 0 -perform_first_div 0 -perform_double_cc 1 -perform_aspiration 1 -sel_var_break_tie_greedy 2 -perform_clause_weight 1 -sel_clause_weight_scheme 1 -threshold_swt 300 -p_swt 0.3 -q_swt 0 -sel_clause_div 1 -sel_var_div 7 -sparrow_c1 2.15 -sparrow_c2 4 -sparrow_c3 75000

[3] For FCC-SAT and FCC-UNKNOWN:
    ./PbO-CCSAT -inst <instance> -seed <seed> -init_solution 1 -p_swt 0.09697519163726609 -perform_aspiration 1 -perform_clause_weight 1 -perform_double_cc 0 -perform_first_div 0 -perform_pac 0 -prob_novelty 0.12032578283349094 -q_swt 0.2009167390247052 -sel_clause_div 2 -sel_clause_weight_scheme 1 -sel_var_break_tie_greedy 1 -sel_var_div 6 -threshold_swt 450 -maxi_flips 100000000

[4] For PTN and PTN-More:
    ./PbO-CCSAT -inst <instance> -seed <seed> -init_solution 1 -p_swt 0.10305467099383325 -perform_aspiration 0 -perform_clause_weight 1 -perform_double_cc 0 -perform_first_div 1 -perform_pac 0 -prob_first_div 0.04267006927278742 -q_swt 0.7462070417690693 -sel_clause_div 1 -sel_clause_weight_scheme 1 -sel_var_break_tie_greedy 1 -sel_var_div 1 -threshold_swt 52 -maxi_flips 100000000

[5] For SMT-QF-BV:
    ./PbO-CCSAT -inst <instance> -seed <seed> -init_solution 2 -p_swt 0.1629585205916365 -perform_aspiration 1 -perform_clause_weight 1 -perform_double_cc 1 -perform_first_div 1 -perform_pac 0 -prob_first_div 0.05615250814954268 -q_swt 0.08020153811108388 -sel_clause_div 1 -sel_clause_weight_scheme 1 -sel_var_break_tie_greedy 1 -sel_var_div 1 -threshold_swt 958 -maxi_flips 100000000

[6] For Community:
    ./PbO-CCSAT -inst <instance> -seed <seed> -gamma_hscore2 539093 -init_solution 1 -p_swt 0.5835271379082203 -perform_aspiration 1 -perform_clause_weight 1 -perform_double_cc 0 -perform_first_div 1 -perform_pac 1 -prob_first_div 0.003546954428490734 -prob_pac 1.5063535410167932E-5 -q_swt 0.29666262025509293 -sel_clause_div 2 -sel_clause_weight_scheme 1 -sel_var_break_tie_greedy 4 -sel_var_div 1 -threshold_swt 613 -maxi_flips 100000000

[7] For SC17-mp1-9:
    ./PbO-CCSAT -inst <instance> -seed <seed> -init_solution 2 -perform_aspiration 0 -perform_clause_weight 1 -perform_double_cc 0 -perform_first_div 0 -perform_pac 0 -sel_clause_div 1 -sel_clause_weight_scheme 2 -sel_var_break_tie_greedy 1 -sel_var_div 3 -sp_paws 0.6877076185004205 -maxi_flips 100000000

[8] For CBMC:
    ./PbO-CCSAT -inst <instance> -seed <seed> -init_solution 1 -p_swt 0.7240585335929828 -perform_aspiration 1 -perform_clause_weight 1 -perform_double_cc 1 -perform_first_div 1 -perform_pac 1 -prob_first_div 0.0014245925511696947 -prob_pac 1.6098502271492688E-5 -q_swt 0.03055083632000999 -sel_clause_div 2 -sel_clause_weight_scheme 1 -sel_var_break_tie_greedy 1 -sel_var_div 3 -threshold_swt 101 -maxi_flips 100000000

[9] For FAC:
    ./PbO-CCSAT -inst <instance> -seed <seed> -init_solution 1 -p_swt 0.4496356924520398 -perform_aspiration 0 -perform_clause_weight 1 -perform_double_cc 1 -perform_first_div 0 -perform_pac 1 -prob_pac 0.0024545041894723555 -q_swt 0.4416428974711496 -sel_clause_div 2 -sel_clause_weight_scheme 1 -sel_var_break_tie_greedy 2 -sel_var_div 3 -threshold_swt 38 -maxi_flips 100000000

