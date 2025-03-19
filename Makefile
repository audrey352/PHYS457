all: solve_test deriv_test extremum_test numerov_test schrodinger_test

solve_test: main_solve.c solve.c
	gcc -o solve_test main_solve.c solve.c -lm

deriv_test: main_deriv.c derivatives.c
	gcc -o deriv_test main_deriv.c derivatives.c -lm

extremum_test: main_extremum.c extremum.c derivatives.c solve.c
	gcc -o extremum_test main_extremum.c extremum.c derivatives.c solve.c -lm

numerov_test: main_numerov.c numerov.c vector_mtx.c
	gcc -o numerov_test main_numerov.c numerov.c vector_mtx.c -lm

schrodinger_test: main_schroedinger.c init.c vector_mtx.c radial_eq_functions.c \
                  solve.c extremum.c derivatives.c
	gcc -o schrodinger_test main_schroedinger.c init.c vector_mtx.c \
	    radial_eq_functions.c solve.c extremum.c derivatives.c -lm
