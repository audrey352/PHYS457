// File main_deriv.c
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "derivatives.h"

// Function to test
double test_func(double x);

//
// Main function
int main(void) {
    // Declare  and initialize variables
    int i, imax;
    double x, dx, x_min, x_max, f, df, ddf;
    FILE *output;
    x_min = 0.0;
    x_max = 15.0;
    imax = 1000;
    dx = (x_max - x_min)/((double) imax);

    output = fopen("deriv_test.dat","w"); // open the file for writing

    for(i=0; i<=imax; i++) {
        // Update x
        x = x_min + dx*i;
        // Calculate test_func(x) and its derivatives at x
        f = test_func(x);
        df = Derivative_FirstD(x, test_func);
        ddf = Derivative_SecondD(x, test_func);
        // Write to the file
        fprintf(output, "%e %e %e %e\n", x, f, df, ddf);
    } // i

    fclose(output); // close file
    return 0;
}  // main

//
// Define the function to test
double test_func(double x) {
    return sin(x);
}  // test_func