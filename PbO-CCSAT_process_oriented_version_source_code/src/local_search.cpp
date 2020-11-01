#ifndef _LOCAL_SEARCH_CPP
#define _LOCAL_SEARCH_CPP

#include "local_search.h"

/*
double wp = 0.567;
int pick_var_walksat_skc()
{
	//walksat_skc
	int c, v, i;
	vector<int> best_var_vector;
	long long best_break_score = -1;
	
	best_var_vector.clear();
	
	c = unsat_stack[rng.next(unsat_stack_fill_pointer)];
	for(i=0; i<clause_lit_count[c]; i++)
	{
		v = clause_lit[c][i].var_num;
		if(best_break_score<0)
		{
			best_var_vector.clear();
			best_var_vector.push_back(v);
			best_break_score = break_score[v];
		}
		else if(best_break_score>break_score[v])
		{
			best_var_vector.clear();
			best_var_vector.push_back(v);
			best_break_score = break_score[v];
		}
		else if(best_break_score == break_score[v])
		{
			best_var_vector.push_back(v);
		}
	}
	
	if(best_break_score==0) return best_var_vector[rng.next(best_var_vector.size())];
	
	if(rng.nextClosed() < wp) return clause_lit[c][rng.next(clause_lit_count[c])].var_num;
	else return best_var_vector[rng.next(best_var_vector.size())];	
}
*/


int pick_var_pbocc()
{
	int v, c;
	if(decision_perform_pac_heuristic)
	{
		if(rng.nextClosed() < para_prob_pac)
		{
			v = select_var_pac_heuristic_ptr();
			return v;
		}
	}
	
	if(decision_perform_first_div)
	{
		if(rng.nextClosed() < para_prob_first_div)
		{
			c = select_unsat_clause_heuristic_first_div_ptr();
			v = select_var_from_clause_heuristic_first_div_ptr(c);
			return v;
		}
	}
	
	if(decision_perform_double_cc)
	{
		v = select_var_cscc_heuristic_ptr();
		if(v >= 1)
		{
			//cout<<"c double_cc, "<<"flipvar = "<<v<<", score = "<<score[v]<<endl;
			return v;
		}
	}
	
	v = select_var_nvcc_heuristic_ptr();
	if(v >= 1)
	{
		//cout<<"c nvcc, "<<"flipvar = "<<v<<", score = "<<score[v]<<endl;
		return v;
	}
	
	if(decision_perform_aspiration)
	{
		v = select_var_aspiration_heuristic_ptr();
		if(v >= 1)
		{
			//cout<<"c aspiration, "<<"flipvar = "<<v<<", score = "<<score[v]<<endl;
			return v;
		}
	}
	
	if(decision_perform_clause_weight_scheme)
	{
		clause_weight_scheme_ptr();
	}
	
	c = select_unsat_clause_heuristic_div_ptr();
	v = select_var_from_clause_heuristic_div_ptr(c);
	//cout<<"c last_div, "<<"flipvar = "<<v<<", score = "<<score[v]<<endl;
	return v;
}

bool local_search()
{
	bool answer = false;
	
	for(tries=0; tries<maxi_tries; tries++)
	{
		init();
		//cout << "c init() successful" << endl;
		answer = local_search_each_try();
		if(answer) return true;
	}
	
	if(tries>0 && tries==maxi_tries) tries--;
	
	return false;
}

bool local_search_each_try()
{
	int flipvar;
	best_unsat_stack_fill_pointer = unsat_stack_fill_pointer;
	//cout<<"c best_unsat_stack_fill_pointer = " << best_unsat_stack_fill_pointer<<endl;
	
	for(step=0; step<maxi_flips; step++)
	{
		
		if(unsat_stack_fill_pointer<best_unsat_stack_fill_pointer)
		{
			best_unsat_stack_fill_pointer = unsat_stack_fill_pointer;
			//cout<<"c best_unsat_stack_fill_pointer = "<<best_unsat_stack_fill_pointer<<endl;
		}
		
		if(unsat_stack_fill_pointer==0) return true;
		flipvar = pick_var_pbocc();
		flip(flipvar);
		update(flipvar, step);
	}
	
	if(unsat_stack_fill_pointer==0) return true;
	return false;
}

void flip(int flipvar)
{
	int c, v;
	lit* clause_c;
	lit* p;
	lit* q;
	
	cur_soln[flipvar] = 1-cur_soln[flipvar];
	
	for(q=var_lit[flipvar]; (c=q->clause_num)!=-1; q++)
	{
		clause_c = clause_lit[c];
		if(cur_soln[flipvar] == q->sense)
		{
			sat_count[c]++;
			
			if (sat_count[c] == 1) // sat_count from 0 to 1
			{
				sat_var[c] = flipvar;//record the only true lit's var
				sat(c);
			}
		}
		else // cur_soln[flipvar] != cur_lit.sense
		{
			sat_count[c]--;
			if (sat_count[c] == 1) //sat_count from 2 to 1
			{
				for(p=clause_c; (v=p->var_num)!=0; p++)
				{
					if(p->sense == cur_soln[v] )
					{
						sat_var[c] = v;
						break;
					}
				}
			}
			else if (sat_count[c] == 0) //sat_count from 1 to 0
			{
				unsat(c);
			}//end else if
			
		}//end else
	}
}

void update(int flipvar, long long step)
{
	update_time_stamp_ptr(flipvar, step);
	update_score_ptr(flipvar);
	update_break_score_ptr(flipvar);
	update_make_score_ptr(flipvar);
	update_sat_var2_ptr(flipvar);
	update_score2_ptr(flipvar);
	update_nvcc_conf_change_ptr(flipvar);
	update_cscc_conf_change_ptr(flipvar);
	update_nvccdvar_related_in_update_function_ptr(flipvar);
}

#endif
