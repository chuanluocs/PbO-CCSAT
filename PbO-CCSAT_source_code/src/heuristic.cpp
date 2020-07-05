#ifndef _HEURISTIC_CPP
#define _HEURISTIC_CPP

#include "heuristic.h"

//for empty_functions
void empty_function_ret_void(){}
void empty_function_ret_void(int a){}
void empty_function_ret_void(int a, long long b){}
int empty_function_ret_int(){return -1;}
int empty_function_ret_int(int a){return -1;}
int empty_function_ret_int(int* a, int b){return -1;}

void init_solution_random()
{
	int v;
	for(v=1; v<=num_vars; v++)
	{
		if(rng.next(2)==0) cur_soln[v] = 0;
		else cur_soln[v] = 1;
	}
	return;
}

void init_solution_all_true_or_false()
{
	int init_assign_value, v;
	if(all_pos_lit_count>all_neg_lit_count) init_assign_value = 1;
	else init_assign_value = 0;
	for(v=1; v<=num_vars; v++)
		cur_soln[v] = init_assign_value;
	return;
}

void init_solution_each_lit_true_or_false()
{
	int v;
	for(v=1; v<=num_vars; v++)
	{
		if(var_pos_lit_count[v]>var_neg_lit_count[v]) cur_soln[v] = 1;
		else cur_soln[v] = 0; 
	}
	return;
}


void init_unsatvar_related()
{
	int v;
	unsatvar_stack_fill_pointer = 0;
	for (v = 1; v <= num_vars; v++)
		unsatvar_app_count[v] = 0;
}

void update_unsatvar_related_in_unsat_function(int clause)
{
	int v;
	lit *p = clause_lit[clause];
	
	for(; (v=p->var_num)!=0; p++)
	{
		unsatvar_app_count[v]++;
		if(unsatvar_app_count[v] == 1)
		{
			index_in_unsatvar_stack[v] = unsatvar_stack_fill_pointer;
			push(v,unsatvar_stack);	
		}
	}
	return;
}

void update_unsatvar_related_in_sat_function(int clause)
{
	int v, last_unsatvar, index;
	lit *p = clause_lit[clause];
	
	for(; (v=p->var_num)!=0; p++)
	{
		unsatvar_app_count[v]--;
		if(unsatvar_app_count[v] == 0)
		{
			last_unsatvar = pop(unsatvar_stack);
			index = index_in_unsatvar_stack[v];
			unsatvar_stack[index] = last_unsatvar;
			index_in_unsatvar_stack[last_unsatvar] = index;
		}
	}
	return;
}


void init_nvccdvar_related()
{
	int v;
	nvccdvar_stack_fill_pointer = 0;
	for(v=1; v<=num_vars; v++)
	{
		if(score[v]>0 && nvcc_conf_change[v]==1)
		{
			already_in_nvccdvar_stack[v] = true;
			push(v,nvccdvar_stack);
		}
		else already_in_nvccdvar_stack[v] = false;
	}
	return;
}

void update_nvccdvar_related_in_update_function(int flipvar)
{
	int i, v, *p;
	for(i=nvccdvar_stack_fill_pointer-1; i>=0; i--)
	{
		v = nvccdvar_stack[i];
		if(score[v]<=0 || nvcc_conf_change[v] == 0)
		{
			nvccdvar_stack[i] = pop(nvccdvar_stack);
			already_in_nvccdvar_stack[v] = false;
		}
	}
	
	for(p=var_neighbor[flipvar]; (v=*p)!=0; p++)
	{
		if(score[v]>0 && !already_in_nvccdvar_stack[v])
		{
			push(v,nvccdvar_stack);
			already_in_nvccdvar_stack[v] = true;
		}
	}
	return;
}

void update_nvccdvar_related_in_swt_scheme()
{
	int v, i;
	for(i=0; i<unsatvar_stack_fill_pointer; i++)
	{
		v = unsatvar_stack[i];
		if(score[v]>0 && nvcc_conf_change[v]==1 && !already_in_nvccdvar_stack[v])
		{
			push(v,nvccdvar_stack);
			already_in_nvccdvar_stack[v] = true;
		}
	}
	return;
}

void update_nvccdvar_related_in_dec_paws_scheme()
{
	int c, v, i;
	for(i=0; i<large_weight_clauses_count; i++)
	{
		c = large_weight_clauses[i];
		if(sat_count[c] == 1)
		{
			v = sat_var[c];
			if(score[v]>0 && nvcc_conf_change[v]==1 && !already_in_nvccdvar_stack[v])
			{
				push(v,nvccdvar_stack);
				already_in_nvccdvar_stack[v] = true;
			}
		}
	}
	return;
}

void update_nvccdvar_related_in_inc_paws_scheme()
{
	int v, i;
	for(i=0; i<unsatvar_stack_fill_pointer; i++)
	{
		v = unsatvar_stack[i];
		if(score[v]>0 && nvcc_conf_change[v]==1 && !already_in_nvccdvar_stack[v])
		{
			push(v,nvccdvar_stack);
			already_in_nvccdvar_stack[v] = true;
		}
	}
	return;
}




void init_sat_var2()
{
	int c, v, i;
	bool flag;
	
	for (c=0; c<num_clauses; c++) 
	{
		if(sat_count[c]==2)
		{
			for(i=0; i<clause_lit_count[c]; i++)
			{
				v = clause_lit[c][i].var_num;
				if(clause_lit[c][i].sense == cur_soln[v])
				{
					sat_var[c] = v;
					break;
				}
			}
			for(i++; i<clause_lit_count[c]; i++)
			{
				v = clause_lit[c][i].var_num;
				if(clause_lit[c][i].sense == cur_soln[v])
				{
					sat_var2[c] = v;
					break;
				}
			}
		}
	}
	return;
}

void update_sat_var2(int flipvar)
{
	int c, v;
	lit* clause_c;
	lit* p;
	lit* q;
	
	for(q=var_lit[flipvar]; (c=q->clause_num)!=-1; q++)
	{
		clause_c = clause_lit[c];
		if(cur_soln[flipvar] == q->sense)
		{
			if(sat_count[c] == 2)
			{
				sat_var2[c] = flipvar;
			}
		}
		else
		{
			if(sat_count[c] == 2)
			{
				for(p=clause_c; (v=p->var_num)!=0; p++)
				{
					if(p->sense == cur_soln[v])
					{
						sat_var[c] = v;
						break;
					}
				}
				for(p++; (v=p->var_num)!=0; p++)
				{
					if(p->sense == cur_soln[v])
					{
						sat_var2[c] = v;
						break;
					}
				}
			}
		}
	}
}



void init_score2()
{
	int c, v, i;
	
	for (v = 1; v <= num_vars; v++)
		score2[v] = 0;
	
	for (c=0; c<num_clauses; c++) 
	{
		if (sat_count[c]==1)
		{
			for(i=0; i<clause_lit_count[c]; i++)
			{
				v = clause_lit[c][i].var_num;
				if(v!=sat_var[c]) score2[v]++;
			}
		}
		else if(sat_count[c]==2)
		{
			for(i=0; i<clause_lit_count[c]; i++)
			{
				v = clause_lit[c][i].var_num;
				if(clause_lit[c][i].sense == cur_soln[v])
					score2[v]--;
			}
		}
	}
	
	score2[0] = 0;
	return;
}

void update_score2(int flipvar)
{
	int c, v;
	lit* clause_c;
	lit* p;
	lit* q;
	
	for(q = var_lit[flipvar]; (c=q->clause_num)!=-1; q++)
	{
		clause_c = clause_lit[c];
		if(cur_soln[flipvar] == q->sense)
		{
			if(sat_count[c] == 3)
			{
				score2[sat_var[c]]++;
				score2[sat_var2[c]]++;
			}
			else if(sat_count[c] == 2)
			{
				score2[flipvar]--;
				for(p=clause_c; (v=p->var_num)!=0; p++)
					score2[v]--;
			}
			else if(sat_count[c] == 1)
			{
				score2[flipvar]--;
				for(p=clause_c; (v=p->var_num)!=0; p++)
					score2[v]++;
			}
		}
		else
		{
			if(sat_count[c] == 2)
			{
				score2[sat_var[c]]--;
				score2[sat_var2[c]]--;
			}
			else if(sat_count[c] == 1)
			{
				score2[flipvar]++;
				for(p=clause_c; (v=p->var_num)!=0; p++)
					score2[v]++;
			}
			else if(sat_count[c] == 0)
			{
				score2[flipvar]++;
				for(p=clause_c; (v=p->var_num)!=0; p++)
					score2[v]--;
			}
		}
	}
	return;
}


//for time_stamp
void init_time_stamp()
{
	int v;
	for(v=1; v<=num_vars; v++)
		time_stamp[v] = 0;
	time_stamp[0] = 0;
	return;
}

void update_time_stamp(int flipvar, long long step)
{
	time_stamp[flipvar] = step;
}


//for score
void init_score()
{
	int v, c, i;
	for(v=1; v<=num_vars; v++)
	{
		score[v] = 0;
		for(i=0; i<var_lit_count[v]; i++)
		{
			c = var_lit[v][i].clause_num;
			if(sat_count[c] == 0) score[v] += clause_weight[c];
			else if(sat_count[c] == 1 && var_lit[v][i].sense == cur_soln[v]) score[v] -= clause_weight[c];
		}
	}
	
	score[0] = 0;
	return;
}

void update_score(int flipvar)
{
	int c, v;
	lit* clause_c;
	lit* p;
	lit* q;
	
	//update related clauses and neighbor vars
	for(q=var_lit[flipvar]; (c=q->clause_num)!=-1; q++)
	{
		clause_c = clause_lit[c];
		if(cur_soln[flipvar] == q->sense)
		{	
			if (sat_count[c] == 2) //sat_count from 1 to 2
			{
				score[sat_var[c]]+=clause_weight[c];
			}
			else if (sat_count[c] == 1) // sat_count from 0 to 1
			{
				score[sat_var[c]]-=clause_weight[c];
				
				for(p=clause_c; (v=p->var_num)!=0; p++)
				{
					score[v]-=clause_weight[c];
				}
			}
		}
		else // cur_soln[flipvar] != cur_lit.sense
		{
			if (sat_count[c] == 1) //sat_count from 2 to 1
			{
				score[sat_var[c]]-=clause_weight[c];
			}
			else if (sat_count[c] == 0) //sat_count from 1 to 0
			{
				for(p=clause_c; (v=p->var_num)!=0; p++)
				{
					score[v]+=clause_weight[c];
				}
				score[flipvar]+=clause_weight[c];
			}//end else if	
		}//end else
	}
}


//for break_score
void init_break_score()
{
	int v, c, i;
	
	for(v=1; v<=num_vars; v++)
	{
		break_score[v] = 0;
		for(i=0; i<var_lit_count[v]; i++)
		{
			c = var_lit[v][i].clause_num;
			if(sat_count[c] == 1 && var_lit[v][i].sense == cur_soln[v]) break_score[v] += clause_weight[c];
		}
	}
	
	break_score[0] = 0;
	return;
}

void update_break_score(int flipvar)
{
	int c, v;
	lit* clause_c;
	lit* p;
	lit* q;
	
	//update related clauses and neighbor vars
	for(q=var_lit[flipvar]; (c=q->clause_num)!=-1; q++)
	{
		clause_c = clause_lit[c];
		if(cur_soln[flipvar] == q->sense)
		{	
			if (sat_count[c] == 2) //sat_count from 1 to 2
			{
				break_score[sat_var[c]]-=clause_weight[c];
			}
			else if (sat_count[c] == 1) // sat_count from 0 to 1
			{
				break_score[sat_var[c]]+=clause_weight[c];
			}
		}
		else // cur_soln[flipvar] != cur_lit.sense
		{
			if (sat_count[c] == 1) //sat_count from 2 to 1
			{
				break_score[sat_var[c]]+=clause_weight[c];
			}
			else if (sat_count[c] == 0) //sat_count from 1 to 0
			{
				break_score[flipvar]-=clause_weight[c];
			}//end else if
		}//end else
	}
}



//for make_score
void init_make_score()
{
	int v, c, i;
	
	for(v=1; v<=num_vars; v++)
	{
		make_score[v] = 0;
		for(i=0; i<var_lit_count[v]; i++)
		{
			c = var_lit[v][i].clause_num;
			if(sat_count[c] == 0) make_score[v] += clause_weight[c];
		}
	}
}

void update_make_score(int flipvar)
{
	int c, v;
	lit* clause_c;
	lit* p;
	lit* q;
	
	//update related clauses and neighbor vars
	for(q=var_lit[flipvar]; (c=q->clause_num)!=-1; q++)
	{
		clause_c = clause_lit[c];
		if(cur_soln[flipvar] == q->sense)
		{	
			if (sat_count[c] == 1) // sat_count from 0 to 1
			{
				for(p=clause_c; (v=p->var_num)!=0; p++)
				{
					make_score[v]-=clause_weight[c];
				}
			}
		}
		else // cur_soln[flipvar] != cur_lit.sense
		{
			if (sat_count[c] == 0) //sat_count from 1 to 0
			{
				for(p=clause_c; (v=p->var_num)!=0; p++)
				{
					make_score[v]+=clause_weight[c];
				}
			}//end else if
		}//end else
	}
}



//for nvcc_conf_change
void build_neighbor_relation()
{
	int	i, j, count;
	int v, c;
	int malloc_adding_length = 10;
	int malloc_var_length = num_vars+malloc_adding_length;
	
	vector<int> temp_neighbor;
	temp_neighbor.clear();
	vector<int>().swap(temp_neighbor);
	
	int *neighbor_flag = new int[malloc_var_length];
	for(v=1; v<=num_vars; ++v)
	{
		neighbor_flag[v] = 0;
	}
	neighbor_flag[0] = 0;

	for(v=1; v<=num_vars; ++v)
	{
		var_neighbor_count[v] = 0;
		neighbor_flag[v] = 1;
		temp_neighbor.clear();
		vector<int>().swap(temp_neighbor);
		
		for(i=0; i<var_lit_count[v]; ++i)
		{
			c = var_lit[v][i].clause_num;
			for(j=0; j<clause_lit_count[c]; ++j)
			{
				if(neighbor_flag[clause_lit[c][j].var_num]==0)
				{
					var_neighbor_count[v]++;
					neighbor_flag[clause_lit[c][j].var_num] = 1;
					temp_neighbor.push_back(clause_lit[c][j].var_num);
				}
			}
			
		}
		neighbor_flag[v] = 0;
		
		var_neighbor[v] = new int[var_neighbor_count[v]+1];
		count = 0;
		for(i=0; i<temp_neighbor.size(); i++)
		{
			var_neighbor[v][count++] = temp_neighbor[i];
			neighbor_flag[temp_neighbor[i]] = 0;
		}
		var_neighbor[v][count]=0;
		var_neighbor_count[v] = count;
		
		//cout<<"c var_neighbor_count["<<v<<"] = "<<var_neighbor_count[v]<<endl;
	}
	
	delete [] neighbor_flag;
	return;
}


void init_nvcc_conf_change()
{
	int v;
	for(v=1; v<=num_vars; v++)
	{	
		nvcc_conf_change[v] = 1;
	}
	nvcc_conf_change[0] = 0;
}

void update_nvcc_conf_change(int flipvar)
{
	int v;
	int *np = var_neighbor[flipvar];
	for(; (v=*np)!=0; np++)
	{
		nvcc_conf_change[v] = 1;
	}
	//update information of flipvar
	nvcc_conf_change[flipvar] = 0;
}


void init_cscc_conf_change()
{
	int v;
	for(v=1; v<=num_vars; v++)
	{
		cscc_conf_change[v] = 1;
	}
	cscc_conf_change[0] = 0;
}

void update_cscc_conf_change(int flipvar)
{
	int v, c;
	lit *clause_c, *q, *p;
	
	for(q = var_lit[flipvar]; (c=q->clause_num)!=-1; q++)
	{
		clause_c = clause_lit[c];
		if(cur_soln[flipvar] == q->sense)
		{
			if(sat_count[c] == 1)
			{
				for(p = clause_c; (v=p->var_num)!=0; p++)
					cscc_conf_change[v] = 1;
			}
		}
		else
		{
			if(sat_count[c] == 0)
			{
				for(p = clause_c; (v=p->var_num)!=0; p++)
					cscc_conf_change[v] = 1;
			}
		}
	}
	
	cscc_conf_change[flipvar] = 0;
	return;
}




int select_var_pac_heuristic()
{
	int c = rng.next(num_clauses);
	return clause_lit[c][rng.next(clause_lit_count[c])].var_num;
}


int select_unsat_clause_random_heuristic()
{
	return unsat_stack[rng.next(unsat_stack_fill_pointer)];
}

int select_unsat_clause_weight_distribution_heuristic()
{
	long long random_value;
	double random_prob;
	long long value_sum = 0;
	int i, c;
	
	if(unsat_stack_fill_pointer==1) return unsat_stack[0];
	
	if(total_unsat_clause_weight<maximum_clause_weight_limit)
	{
		random_value = rng.next(total_unsat_clause_weight) + 1;
	}
	else
	{
		random_prob = rng.nextClosed();
		random_value = (total_unsat_clause_weight - 1) * random_prob + 1;
		if(random_value<1) random_value = 1;
		else if(random_value>total_unsat_clause_weight) random_value = total_unsat_clause_weight;
	}
	
	c = -1;
	value_sum = 0;
	for(i=0; i<unsat_stack_fill_pointer; i++)
	{
		c = unsat_stack[i];
		value_sum += clause_weight[c];
		if(random_value<=value_sum)
			break;
	}
	if (c==-1) c = unsat_stack[rng.next(unsat_stack_fill_pointer)];
	return c;
}


int select_var_from_clause_random_heuristic(int clause_num)
{
	int c = clause_num;
	return clause_lit[c][rng.next(clause_lit_count[c])].var_num;
}

int select_var_from_clause_greatest_score_heuristic(int clause_num)
{
	int c = clause_num;
	int v, i, best_v;
	
	best_v = clause_lit[c][0].var_num;
	for(i=1; i<clause_lit_count[c]; i++)
	{
		v = clause_lit[c][i].var_num;
		if(score[v]>score[best_v]) best_v = v;
		else if(score[v]==score[best_v] && time_stamp[v]<time_stamp[best_v]) best_v = v;
	}
	return best_v;
}

int select_var_from_clause_greatest_age_heuristic(int clause_num)
{
	int c = clause_num;
	int v, i, best_v;
	
	best_v = clause_lit[c][0].var_num;
	for(i=1; i<clause_lit_count[c]; i++)
	{
		v = clause_lit[c][i].var_num;
		if(time_stamp[v]<time_stamp[best_v]) best_v = v;
	}
	return best_v;
}

int select_var_from_clause_novelty_heuristic(int clause_num)
{
	int c = clause_num;
	int v, i, best_v, second_best_v;
	long long greatest_time_stamp;
	
	if(clause_lit_count[c] == 1)
		return clause_lit[c][0].var_num;
	
	best_v = clause_lit[c][0].var_num;
	greatest_time_stamp = time_stamp[clause_lit[c][0].var_num];
	for(i=1; i<clause_lit_count[c]; i++)
	{
		v = clause_lit[c][i].var_num;
		if(greatest_time_stamp<time_stamp[v])
			greatest_time_stamp = time_stamp[v];
		
		if(score[v]>score[best_v]) best_v = v;
		else if(score[v]==score[best_v] && time_stamp[v]<time_stamp[best_v]) best_v = v;
	}
	
	second_best_v = -1;
	for(i=0; i<clause_lit_count[c]; i++)
	{
		v = clause_lit[c][i].var_num;
		if(v != best_v)
		{
			second_best_v = v;
			break;
		}
	}
	for(i++; i<clause_lit_count[c]; i++)
	{
		v = clause_lit[c][i].var_num;
		if(v == best_v) continue;
		if(score[v]>score[second_best_v]) second_best_v = v;
		else if(score[v]==score[second_best_v] && time_stamp[v]<time_stamp[second_best_v]) second_best_v = v;
	}
	
	if(second_best_v == -1) return best_v;
	
	if(time_stamp[best_v] == greatest_time_stamp)
	{
		if(score[best_v] == 0 || rng.nextClosed() < para_prob_novelty)
			return second_best_v;
	}
	
	return best_v;
}

int select_var_from_clause_sparrow_heuristic(int clause_num)
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
	for(i=0; i<clause_lit_count[c]; i++)
	{
		v = clause_lit[c][i].var_num;
		if(score[v]<0) sparrow_score = pow(para_sparrow_c1, score[v]);
		else sparrow_score = 1.0;
		age_v = step - time_stamp[v];
		sparrow_age = 1.0 + pow((double)age_v/(double)para_sparrow_c3, para_sparrow_c2);
		var_prob_value[v] = sparrow_score * sparrow_age;
		sum_prob_value += var_prob_value[v];
	}
	
	random_prob = rng.nextClosed();
	random_prob_value = random_prob * sum_prob_value;
	
	sum_prob_value = 0;
	for(i=0; i<clause_lit_count[c]; i++)
	{
		v = clause_lit[c][i].var_num;
		sum_prob_value += var_prob_value[v];
		if(sum_prob_value>=random_prob_value)
			return v;
	}
	
	return clause_lit[c][rng.next(clause_lit_count[c])].var_num;
}

int select_var_from_clause_probsat_heuristic(int clause_num)
{
	int c = clause_num;
	int v, i;
	double sum_prob_value = 0.0;
	double random_prob;
	double random_prob_value;
	
	sum_prob_value = 0.0;
	for(i=0; i<clause_lit_count[c]; i++)
	{
		v = clause_lit[c][i].var_num;
		var_prob_value[v] = pow(para_probsat_cb, -break_score[v]);
		sum_prob_value += var_prob_value[v];
	}
	
	random_prob = rng.nextClosed();
	random_prob_value = random_prob * sum_prob_value;
	
	sum_prob_value = 0;
	for(i=0; i<clause_lit_count[c]; i++)
	{
		v = clause_lit[c][i].var_num;
		sum_prob_value += var_prob_value[v];
		if(sum_prob_value>=random_prob_value)
			return v;
	}
	
	return clause_lit[c][rng.next(clause_lit_count[c])].var_num;
}



int select_var_cscc_heuristic_from_unsatvar_stack()
{
	int i, v, flipvar;
	long long best_score = -1;
	
	candidate_var_stack_fill_pointer = 0;
	for(i=0; i<unsatvar_stack_fill_pointer; i++)
	{
		v = unsatvar_stack[i];
		if(cscc_conf_change[v] == 1)
		{
			candidate_var_stack[0] = v;
			candidate_var_stack_fill_pointer = 1;
			best_score = score[v];
			break;
		}
	}
	
	for(i++; i<unsatvar_stack_fill_pointer; i++)
	{
		v = unsatvar_stack[i];
		if(cscc_conf_change[v]==0) continue;
		if(score[v] > best_score)
		{
			candidate_var_stack[0] = v;
			candidate_var_stack_fill_pointer = 1;
			best_score = score[v];
		}
		else if(score[v] == best_score)
			candidate_var_stack[candidate_var_stack_fill_pointer++] = v;
	}
	
	if(candidate_var_stack_fill_pointer == 0) return -1;
	if(best_score <= 0) return -1;
	flipvar = select_var_by_break_tie_in_set_ptr(candidate_var_stack, candidate_var_stack_fill_pointer);
	return flipvar;
}


int select_var_cscc_heuristic_from_nvccdvar_stack()
{
	int i, v, flipvar;
	long long best_score = -1;
	
	candidate_var_stack_fill_pointer = 0;
	for(i=0; i<nvccdvar_stack_fill_pointer; i++)
	{
		v = nvccdvar_stack[i];
		if(cscc_conf_change[v] == 1)
		{
			candidate_var_stack[0] = v;
			candidate_var_stack_fill_pointer = 1;
			best_score = score[v];
			break;
		}
	}
	
	for(i++; i<nvccdvar_stack_fill_pointer; i++)
	{
		v = nvccdvar_stack[i];
		if(cscc_conf_change[v]==0) continue;
		if(score[v] > best_score)
		{
			candidate_var_stack[0] = v;
			candidate_var_stack_fill_pointer = 1;
			best_score = score[v];
		}
		else if(score[v] == best_score)
			candidate_var_stack[candidate_var_stack_fill_pointer++] = v;
	}
	
	if(candidate_var_stack_fill_pointer == 0) return -1;
	if(best_score <= 0) return -1;
	flipvar = select_var_by_break_tie_in_set_ptr(candidate_var_stack, candidate_var_stack_fill_pointer);
	return flipvar;
}


int select_var_nvcc_heuristic_from_unsatvar_stack()
{
	int i, v, flipvar;
	long long best_score = -1;
	
	candidate_var_stack_fill_pointer = 0;
	for(i=0; i<unsatvar_stack_fill_pointer; i++)
	{
		v = unsatvar_stack[i];
		if(nvcc_conf_change[v] == 1)
		{
			candidate_var_stack[0] = v;
			candidate_var_stack_fill_pointer = 1;
			best_score = score[v];
			break;
		}
	}
	
	for(i++; i<unsatvar_stack_fill_pointer; i++)
	{
		v = unsatvar_stack[i];
		if(nvcc_conf_change[v]==0) continue;
		if(score[v] > best_score)
		{
			candidate_var_stack[0] = v;
			candidate_var_stack_fill_pointer = 1;
			best_score = score[v];
		}
		else if(score[v] == best_score)
			candidate_var_stack[candidate_var_stack_fill_pointer++] = v;
	}
	
	if(candidate_var_stack_fill_pointer == 0) return -1;
	if(best_score <= 0) return -1;
	flipvar = select_var_by_break_tie_in_set_ptr(candidate_var_stack, candidate_var_stack_fill_pointer);
	return flipvar;
}


int select_var_nvcc_heuristic_from_nvccdvar_stack()
{
	int i, v, flipvar;
	long long best_score = -1;
	
	if(nvccdvar_stack_fill_pointer==0) return -1;
	
	v = nvccdvar_stack[0];
	candidate_var_stack[0] = v;
	candidate_var_stack_fill_pointer = 1;
	best_score = score[v];
	
	for(i=1; i<nvccdvar_stack_fill_pointer; i++)
	{
		v = nvccdvar_stack[i];
		if(score[v] > best_score)
		{
			candidate_var_stack[0] = v;
			candidate_var_stack_fill_pointer = 1;
			best_score = score[v];
		}
		else if(score[v] == best_score)
			candidate_var_stack[candidate_var_stack_fill_pointer++] = v;
	}
	
	if(best_score <= 0) return -1;
	flipvar = select_var_by_break_tie_in_set_ptr(candidate_var_stack, candidate_var_stack_fill_pointer);
	return flipvar;
}

int select_var_aspiration_heuristic()
{
	int i, v, flipvar;
	long long sig_score = avg_clause_weight;
	long long best_score = -1;
	
	candidate_var_stack_fill_pointer = 0;
	for(i=0; i<unsatvar_stack_fill_pointer; i++)
	{
		v = unsatvar_stack[i];
		if(score[v] > sig_score)
		{
			candidate_var_stack[0] = v;
			candidate_var_stack_fill_pointer = 1;
			best_score = score[v];
			break;
		}
	}
	
	for(i++; i<unsatvar_stack_fill_pointer; i++)
	{
		v = unsatvar_stack[i];
		if(score[v] > best_score)
		{
			candidate_var_stack[0] = v;
			candidate_var_stack_fill_pointer = 1;
			best_score = score[v];
		}
		else if(score[v] == best_score)
			candidate_var_stack[candidate_var_stack_fill_pointer++] = v;
	}
	
	if(candidate_var_stack_fill_pointer == 0) return -1;
	if(best_score <= sig_score) return -1;
	flipvar = select_var_by_break_tie_in_set_ptr(candidate_var_stack, candidate_var_stack_fill_pointer);
	return flipvar;
}


int select_var_by_random_break_tie_in_set (int* the_set, int the_set_length)
{
	if(the_set_length<=0) return -1;
	if(the_set_length == 1) return the_set[0];
	return the_set[rng.next(the_set_length)];
}

int select_var_by_greatest_age_break_tie_in_set (int* the_set, int the_set_length)
{
	int i, v, best_v;
	
	if(the_set_length<=0) return -1;
	if(the_set_length == 1) return the_set[0];
	
	best_v = the_set[0];
	for(i=1; i<the_set_length; i++)
	{
		v = the_set[i];
		if(time_stamp[v]<time_stamp[best_v]) best_v = v;
	}
	return best_v;
}

inline long long calc_hscore(int v)
{
	return score[v] + score2[v]/para_d_hscore + (step-time_stamp[v])/para_beta_hscore;
}

int select_var_by_greatest_hscore_break_tie_in_set(int* the_set, int the_set_length)
{
	int i, v, best_v;
	long long hscore_best_v, hscore_v;

	if(the_set_length<=0) return -1;
	if(the_set_length == 1) return the_set[0];
	
	best_v = the_set[0];
	hscore_best_v = calc_hscore(best_v);
	for(i=1; i<the_set_length; i++)
	{
		v = the_set[i];
		hscore_v = calc_hscore(v);
		if(hscore_v>hscore_best_v)
		{
			best_v = v;
			hscore_best_v = hscore_v;
		}
		else if(hscore_v==hscore_best_v && time_stamp[v]<time_stamp[best_v])
			best_v = v;
	}
	return best_v;
}

int select_var_from_clause_greatest_hscore_heuristic(int clause_num)
{
	int c = clause_num;
	int v, i, best_v;
	long long hscore_best_v, hscore_v;
	
	best_v = clause_lit[c][0].var_num;
	hscore_best_v = calc_hscore(best_v);
	
	for(i=1; i<clause_lit_count[c]; i++)
	{
		v = clause_lit[c][i].var_num;
		hscore_v = calc_hscore(v);
		if(hscore_v > hscore_best_v)
		{
			best_v = v;
			hscore_best_v = hscore_v;
		}
		else if(hscore_v==hscore_best_v && time_stamp[v]<time_stamp[best_v])
			best_v = v;
	}
	return best_v;
}


inline long long calc_hscore2(int v)
{
	return score2[v] + (step-time_stamp[v])/para_gamma_hscore2;
}

int select_var_by_greatest_hscore2_break_tie_in_set(int* the_set, int the_set_length)
{
	int i, v, best_v;
	long long hscore2_best_v, hscore2_v;
	
	if(the_set_length<=0) return -1;
	if(the_set_length == 1) return the_set[0];
	
	best_v = the_set[0];
	hscore2_best_v = calc_hscore2(best_v);
	for(i=1; i<the_set_length; i++)
	{
		v = the_set[i];
		hscore2_v = calc_hscore2(v);
		if(hscore2_v>hscore2_best_v)
		{
			best_v = v;
			hscore2_best_v = hscore2_v;
		}
		else if(hscore2_v==hscore2_best_v && time_stamp[v]<time_stamp[best_v])
			best_v = v;
	}
	return best_v;
}

int select_var_from_clause_greatest_hscore2_heuristic(int clause_num)
{
	int c = clause_num;
	int v, i, best_v;
	long long hscore2_best_v, hscore2_v;
	
	best_v = clause_lit[c][0].var_num;
	hscore2_best_v = calc_hscore2(best_v);
	
	for(i=1; i<clause_lit_count[c]; i++)
	{
		v = clause_lit[c][i].var_num;
		hscore2_v = calc_hscore2(v);
		if(hscore2_v > hscore2_best_v)
		{
			best_v = v;
			hscore2_best_v = hscore2_v;
		}
		else if(hscore2_v==hscore2_best_v && time_stamp[v]<time_stamp[best_v])
			best_v = v;
	}
	return best_v;
}


void init_clause_weight_scheme_swt()
{
	scale_avg = (para_clause_weight_threshold_swt+1) * para_q_scale_swt;
}

void smooth_clause_weight_swt()
{
	int c, v, i;
	long long new_total_weight = 0;
	for(v=1; v<=num_vars; v++)
		score[v] = 0;
	
	total_unsat_clause_weight = 0;
	for(c=0; c<num_clauses; c++)
	{
		clause_weight[c] = clause_weight[c]*para_p_scale_swt + scale_avg;
		if (clause_weight[c]<1) clause_weight[c] = 1;
		new_total_weight += clause_weight[c];
		if(sat_count[c] == 0)
		{
			total_unsat_clause_weight += clause_weight[c];
			for(i=0; i<clause_lit_count[c]; i++)
				score[clause_lit[c][i].var_num] += clause_weight[c];
		}
		else if(sat_count[c]==1)
			score[sat_var[c]] -= clause_weight[c];
	}
	
	avg_clause_weight = new_total_weight/num_clauses;
	delta_total_clause_weight = new_total_weight - avg_clause_weight*num_clauses;
	return;
}

void clause_weight_scheme_swt()
{
	int i, v;
	for(i=0; i<unsat_stack_fill_pointer; i++)
		clause_weight[unsat_stack[i]]++;
	
	total_unsat_clause_weight += unsat_stack_fill_pointer;
	
	for(i=0; i<unsatvar_stack_fill_pointer; i++)
	{
		v = unsatvar_stack[i];
		score[v] += unsatvar_app_count[v];
	}
	
	update_nvccdvar_related_in_swt_scheme_ptr();
	
	delta_total_clause_weight += unsat_stack_fill_pointer;
	if(delta_total_clause_weight > num_clauses)
	{
		avg_clause_weight++;
		delta_total_clause_weight -= num_clauses;
		
		if(avg_clause_weight>para_clause_weight_threshold_swt)
		{
			//cout<<"c smooth_clause_weight_swt, ";
			smooth_clause_weight_swt();
			//cout<<" avg_clause_weight = "<<avg_clause_weight<<endl;
		}
	}
}



void init_clause_weight_scheme_paws()
{
	large_weight_clauses_count = 0;
	large_clause_weight_count_threshold = 10;
}

void dec_clause_weight_paws()
{
	int c, v, i;
	for(i=0; i<large_weight_clauses_count; i++)
	{
		c = large_weight_clauses[i];
		if(sat_count[c]>0)
		{
			delta_total_clause_weight -= clause_weight[c];
			clause_weight[c]--;
			
			if(clause_weight[c]<1) clause_weight[c] = 1;
			
			delta_total_clause_weight += clause_weight[c];
			if(delta_total_clause_weight<0)
			{
				avg_clause_weight--;
				delta_total_clause_weight += num_clauses;
			}
			
			if(clause_weight[c] == 1)
			{
				large_weight_clauses[i] = large_weight_clauses[--large_weight_clauses_count];
				i--;
			}
			if(sat_count[c] == 1)
			{
				v = sat_var[c];
				score[v]++;
			}
		}
	}
	
	update_nvccdvar_related_in_dec_paws_scheme_ptr();
	return;
}

void inc_clause_weight_paws()
{
	int c, v, i;
	for(i=0; i<unsat_stack_fill_pointer; i++)
	{
		c = unsat_stack[i];
		clause_weight[c]++;
		if(clause_weight[c] == 2)
			large_weight_clauses[large_weight_clauses_count++] = c;
	}
	
	total_unsat_clause_weight += unsat_stack_fill_pointer;
	
	delta_total_clause_weight += unsat_stack_fill_pointer;
	if(delta_total_clause_weight > num_clauses)
	{
		avg_clause_weight++;
		delta_total_clause_weight -= num_clauses;
	}
	
	for(i=0; i<unsatvar_stack_fill_pointer; i++)
	{
		v = unsatvar_stack[i];
		score[v] += unsatvar_app_count[v];
	}
	
	update_nvccdvar_related_in_inc_paws_scheme_ptr();
	return;
}

void clause_weight_scheme_paws()
{
	if(rng.nextClosed()<para_smooth_probability_paws && large_weight_clauses_count>large_clause_weight_count_threshold)
		dec_clause_weight_paws();
	else inc_clause_weight_paws();
	return;
}







#endif
