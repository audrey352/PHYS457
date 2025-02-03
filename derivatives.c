// File derivatives.c
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "derivatives.h"


// Derivative_FirstD
double Derivative_FirstD(double x, double (*func)(double)) {
    /*
    computes the first derivative of a function using
    f’(x) = (f(x+h) - f(x-h))/(2h) + O(h^2)
    */

    // Declare  and initialize variables
    double df, h;
    h = 1.0E-5;

    // If x is non-zero, set h to h*x
    if(x != 0.0) { 
        h = h*x; 
    }

    // Calculate df using the first derivative formula
    df = ((*func)(x+h) - (*func)(x-h)) / (2.0*h);
    return df;
}  // Derivative_FirstD

//
// Derivative_SecondD
double Derivative_SecondD(double x, double (*func)(double)) {
    /*
    Computes the second derivative of a function using
    f’’(x) = (f(x+h) + f(x-h) - 2f(x)) / (h^2) + O(h^2)
    */
    
    // Declare and initialize variables
    double ddf, h;
    h = 1.0E-5;

    // If x is non-zero, set h to h*x
    if(x != 0.0) { 
        h = h*x; 
    }

    // Calculate ddf using the second derivative formula
    ddf = ((*func)(x+h) + (*func)(x-h) - 2*(*func)(x)) / (h*h);
    return ddf;
}  // Derivative_SecondD