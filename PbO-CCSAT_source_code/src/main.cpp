#ifndef _MAIN_CPP
#define _MAIN_CPP

#include "basis.h"

void handle_interrupt(int sig) {
	cout << "c" << endl;
	cout << "c caught signal... exiting" << endl;
	cout << "s UNKNOWN" << endl;
	cout << "c best_unsat_stack_fill_pointer = " << best_unsat_stack_fill_pointer << endl;
	cout << "c" <<endl;
	free_memory(ori_num_vars, ori_num_clauses);
	exit(-1);
}

void setup_signal_handler() {
	signal(SIGTERM, handle_interrupt);
	signal(SIGINT, handle_interrupt);
	signal(SIGQUIT, handle_interrupt);
	//signal(SIGABRT, handle_interrupt);
	signal(SIGKILL, handle_interrupt);
}


double compute_time(clock_t start, clock_t stop)
{
	double run_time = (double)(stop - start) / CLOCKS_PER_SEC;
	return run_time;
}


int main(int argc, char **argv)
{
	clock_t start, stop;
	start = clock();
	
	setup_signal_handler();
	
	double run_time;
	bool answer = false;
	
	print_general_information(argv[0]);
	
	if(!parse_arguments(argc, argv))
	{
		cout << "c" << endl;
		cout << "c Arguments Error!" << endl;
		cout << "c" << endl;
		print_usage_info(argv[0]);
		return -1;
	}
	
	if(!build_instance(inst))
	{
		cout << "c" << endl;
		cout << "c Instance File Error!" << endl;
		cout << "c" << endl;
		print_usage_info(argv[0]);
		return -1;
	}
	
	rng.seed(seed);
	
	print_algorithm_settings_information();
	
	answer = local_search();
	
	stop = clock();
	run_time = compute_time(start, stop);
	
	if(answer)
	{
		if(verify_soln(cur_soln))
		{
			print_SAT_result(cur_soln, num_vars);
			cout<<"c Solution Verified!"<<endl;
		}
		else print_UNKNOWN_result();
	}
	else print_UNKNOWN_result();
	
	print_run_info(run_time, tries, step);
	
	free_memory(ori_num_vars, ori_num_clauses);
	
	return 0;
}

#endif
