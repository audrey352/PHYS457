all: solve_test deriv_test extremum_test numerov_test schroedinger_test schroedinger

solve_test: main_solve.c solve.c
	gcc -o solve_test main_solve.c solve.c -lm

deriv_test: main_deriv.c derivatives.c
	gcc -o deriv_test main_deriv.c derivatives.c -lm

extremum_test: main_extremum.c extremum.c derivatives.c solve.c
	gcc -o extremum_test main_extremum.c extremum.c derivatives.c solve.c -lm

numerov_test: main_numerov.c numerov.c vector_mtx.c
	gcc -o numerov_test main_numerov.c numerov.c vector_mtx.c -lm

schroedinger_test: main_schroedinger.c init.c vector_mtx.c radial_eq_functions.c \
                  solve.c extremum.c derivatives.c schroedinger.c numerov.c
	gcc -o schroedinger_test main_schroedinger.c init.c vector_mtx.c \
	    radial_eq_functions.c solve.c extremum.c derivatives.c schroedinger.c numerov.c -lm

SRCS = derivatives.c extremum.c init.c main_schroedinger.c numerov.c \
		radial_eq_functions.c schroedinger.c solve.c vector_mtx.c

schroedinger: $(SRCS)
	gcc -o schroedinger $(SRCS) -lm
