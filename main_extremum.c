// File main_extremum.c
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "extremum.h"

// Functions to test
double test_func1(double x);
double test_func2(double x);
double test_func3(double x);

//
// Main function
int main(void) {
    // Declare variables
    double x_extremum1, curvature1, x_init1;
    double x_extremum2, curvature2, x_init2;
    double x_extremum3, curvature3, x_init3;

    // Test 1
    x_init1 = 0.5; // initial guess for the extremum
    x_extremum1 = Extremum_GetExtremum(test_func1, x_init1, &curvature1);
    // Print results
    fprintf(
        stdout, "Test 1: x_extremum = %e, f_extremum = %e, curvature = %e\n", 
        x_extremum1, test_func1(x_extremum1), curvature1
    );

    // Test 2
    x_init2 = 1.0; // initial guess for the extremum
    x_extremum2 = Extremum_GetExtremum(test_func2, x_init2, &curvature2);
    // Print results
    fprintf(
        stdout, "Test 2: x_extremum = %e, f_extremum = %e, curvature = %e\n", 
        x_extremum2, test_func2(x_extremum2), curvature2
    );

    // Test 3
    x_init3 = 1.0; // initial guess for the extremum
    x_extremum3 = Extremum_GetExtremum(test_func3, x_init3, &curvature3);
    // Print results
    fprintf(
        stdout, "Test 3: x_extremum = %e, f_extremum = %e, curvature = %e\n", 
        x_extremum3, test_func3(x_extremum3), curvature3
    );

    return 0;
}  // main

//
// Define the functions to test
double test_func1(double x) {
    /*
    Test 1
    Minimum at x = 1
    */

    double f;
    f = x*x - 2.0*x + 1;
    
    return f;
}  // test_func1

double test_func2(double x) {
    /*
    Test 2
    Assume x > 0, don't set x_init = 0.0 which is a singular point
    A good way to guess x_init is to plot the function
    */

    double f;
    f = 1.0/x/x - 2.0/x;
    
    return f;
}  // test_func2

double test_func3(double x) {
    /*
    Test 3
    Assume x > 0, don't set x_init = 0.0 which is a singular point
    A good way to guess x_init is to plot the function
    */

    double f;
    f = 1.0/x/x - 1.0/(1.0 + exp(x - 5.0));
    
    return f;
}  // test_func3