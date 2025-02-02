// File: solve.c
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "solve.h"

// Numerical derivative, visible only within this file
double Solve_Get_Df(double (*func)(double), double x);

//
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

    // Calculate x_mid = (x_min + x_max)/2.0
    x_mid = (x_min + x_max)/2.0;
    // Calculate f_mid = f(x_mid) - nu
    f_mid = (*func)(x_mid) - nu;
    
    // Calculate the error
    if(nu != 0.0) {
        err = fabs(f_mid/nu);
    } else {
        err = fabs(f_mid);
    }

    // If err < tol, we have a solution and the calculation ends.
    if(err < tol) { 
        return x_mid; 
    }

    if(f_mid*f_max < 0.0) {  // the solution is between x_mid and x_max
        return Solve_Bisect(nu, func, x_mid, x_max, tol, count);
    } 
    else if(f_min*f_mid < 0.0) {  // the solution is between x_min and x_mid
        return Solve_Bisect(nu, func, x_min, x_mid, tol, count);
    } else { // one of the factors is zero
        if (f_mid == 0.0) {
            return x_mid;
        }
        else if (f_max == 0.0) {
            return x_max;
        }
        else {
            return x_min;
        }
    }
}  // Solve_Bisect


//
// Solve_Get_Df
double Solve_Get_Df(double (*func)(double), double x_old) {
    /*
    Numerical derivative, uses f’(x) = (f(x+h) - f(x-h))/(2h) + O(h^2)
    */

    double h, df;
    if(x_old != 0.0) { 
        h = x_old*1.0E-5; 
    } else {
        h = 1.0E-5; 
    }
    df = (*func)(x_old+h) - (*func)(x_old-h);
    df /= 2.0*h;
    return df;
}  // Solve_Get_Df


//
// Solve_Newton
double Solve_Newton(
    double nu, double (*func)(double), double x_0, double tol, int *count
) {
    /*
    Solves nu = func(x) by newton’s method, using x_{n+1} = x_n + (nu -f(x_n))/f’(x_n)
    */
    
    // Declare and initialize variables
    double x_old, x_new, err, df, h;
    int count_max;
    count_max = 1000;
    x_old = x_0; // Initial value

    do {
        // Get the derivative
        df = Solve_Get_Df(func, x_old);
        
        // Warn and exit if derivative is too small
        if(fabs(df) < tol) {
            fprintf(stderr, "Solve_Newton: Derivative too small. df=%g, tol=%g", df, tol);
            fprintf(stderr, "Exiting.\n");
            exit(0);
        }

        // Calculate x_new using Newton's method
        x_new = x_old + (nu - (*func)(x_old))/df;
        // Calculate error
        err = fabs((x_new - x_old)/x_old);
        // Update x_old and counts
        x_old = x_new;
        *count += 1;

        // Warn and exit if too many iterations
        if(*count == count_max) {
            fprintf(
                stderr, "Solve_Newton: Max iterations reached. \
                %d iterations done without convergence.\n", 
                count_max
            );
            fprintf(stderr, "Exiting.\n");
            exit(0);
        }
    } while(err > tol);

    return x_new;
}  // Solve_Newton
