// File extremum.c
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "derivatives.h"
#include "extremum.h"
#include "solve.h"

typedef double (*FuncPT)(double);
FuncPT ORIG_FUNC; // A common variable. Only valid within this file
double Extremum_DF(double x); // Used only within this file

//
// Extremum_GetExtremum
double Extremum_GetExtremum(FuncPT func, double x_init, double *curvature) {
    /*
    Finds the miminum or maximum near x_init
    Returns the value of x where the extremum is
    Curvature has the value of the second derivative at the extremum
    */

    // Declare variables
    double x, tol, ddf;
    int count;
    ORIG_FUNC = func; // To communicate with Extremum_DF

    // Solve 0 = Extremum_DF(x), starting with x_init
    count = 0;
    tol = 1.0e-10;
    x = Solve_Newton(0.0, Extremum_DF, x_init, tol, &count);  // extremum is at x
    
    // Second derivative at the extremum
    ddf = Derivative_SecondD(x, ORIG_FUNC);
    // Set curvature to the second derivative.
    *curvature = ddf;

    return x;
}  // Extremum_GetExtremum

//
// Extremum_DF
double Extremum_DF(double x) {
    double f;

    // Calculate the first derivative of ORIG_FUNC using Derivative_FirstD
    f = Derivative_FirstD(x, ORIG_FUNC);

    return f;
}  // Extremum_DF