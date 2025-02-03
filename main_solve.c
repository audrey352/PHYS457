// File main_solve.c
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "solve.h"

// Function to test
double f_solve(double x);

int main(void) {
    // Declare variables
    double x_max, x_min, x, tol;
    int count;

    // Test Solve_Bisect
    fprintf(stdout, "Solve_Bisect\n");
    count = 0;
    tol = 1.0e-10;
    x = Solve_Bisect(0.0, f_solve, 0.1, 4.0, tol, &count);
    fprintf(stdout, "count = %d\n", count);
    fprintf(stdout, "x = %e\n", x);

    // Test Solve_Newton
    fprintf(stdout, "Solve_Newton\n");
    count = 0;
    tol = 1.0e-10;
    x = Solve_Newton(0.0, f_solve, 4.0, tol, &count);
    fprintf(stdout, "count = %d\n", count);
    fprintf(stdout, "x = %e\n", x);
    
    return 1;
}  // main

// Define the function to test
double f_solve(double x) {
    // return sin(x);
    return x*exp(x) - 3.0;
}