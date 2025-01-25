// File: solve.c
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "solve.h"

// Numerical derivative, visible only within this file
double Solve_Get_Df(double (*func)(double), double x);

// Solve_Bisect
double Solve_Bisect(
    double nu, double (*func)(double), double x_min, double x_max, double tol, 
    int *count
) {
    /*
    Solve f(x) = nu using bisect method
    */

    // Declare variables and initialize counts
    double x_mid, f_max, f_min, f_mid, err;
    int count_max;
    count_max = 1000; // Large enough.
    *count += 1; // Keep track of the number of iterations.

    // Check current count, exit if too large
    if(*count > count_max) {
        fprintf(
            stderr, "Solve_Bisect: Done %d iterations without convergence.\n", 
            count_max
        );
        fprintf(stderr, "Exiting.\n");
        exit(0);
    }

    // Calculate f_max = f(x_max) - nu
    f_max = (*func)(x_max) - nu;
    // Calculate f_min = f(x_min) - nu
    f_min = (*func)(x_min) - nu;
    // Check that we have a solution between xmin and xmax
    if(f_max*f_min > 0.0) { // we can’t find a solution within the range
        // Warn and exit
        fprintf(stderr, "Solve_Bisect: No solution in range [%g, %g].\n", x_min, x_max);
        fprintf(stderr, "Exiting.\n");
        exit(0);
    }

    // TD: Calculate x_mid = (x_min + x_max)/2.0
    // TD: Calculate f_mid = f(x_mid) - nu

}

