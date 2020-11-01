#ifndef _MAIN_CPP
#define _MAIN_CPP

#include "solver.h"

void handle_interrupt(int sig)
{
    cout << "c" << endl;
    cout << "c caught signal... exiting" << endl;
    cout << "s UNKNOWN" << endl;
    cout << "c" << endl;
    exit(-1);
}

void setup_signal_handler()
{
    signal(SIGTERM, handle_interrupt);
    signal(SIGINT, handle_interrupt);
    signal(SIGQUIT, handle_interrupt);
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

    Solver solver(argc, argv);

    solver.print_general_information(argv[0]);
    solver.print_algorithm_settings_information();
    answer = solver.solve();
    stop = clock();
    run_time = compute_time(start, stop);
    if (answer)
    {
        if (solver.verify_soln(solver.get_soln()))
        {
            solver.print_SAT_result();
            cout << "c Solution Verified!" << endl;
        }
        else
            solver.print_UNKNOWN_result();
    }
    else
        solver.print_UNKNOWN_result();
    solver.print_run_info(run_time);
    solver.free_memory();

    return 0;
}

#endif