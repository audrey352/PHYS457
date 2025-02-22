all: solve_test deriv_test extremum_test numerov_test

solve_test: main_solve.c solve.c
	gcc -o solve_test main_solve.c solve.c -lm

deriv_test: main_deriv.c derivatives.c
	gcc -o deriv_test main_deriv.c derivatives.c -lm

extremum_test: main_extremum.c extremum.c derivatives.c solve.c
	gcc -o extremum_test main_extremum.c extremum.c derivatives.c solve.c -lm

numerov_test: main_numerov.c numerov.c
	gcc -o numerov_test main_numerov.c numerov.c vector_mtx.c -lm