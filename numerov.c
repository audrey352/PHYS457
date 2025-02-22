// File numerov.c
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "numerov.h"
#include "numerov_params.h"
#include "vector_mtx.h"

void Numerov_Make_Fn(
    double *numerov_F, NumerovParams *Num_Params, DynamicVars *Dyn_Vars
);  // Makes F[n]

void Numerov_Advance_A_Step(
    double *y, int n, double *numerov_F, NumerovParams *Num_Params,
    DynamicVars *Dyn_Vars
);  // Takes a step from y[n-1] to y[n]

//
// Numerov_Advance
void Numerov_Advance(double *y, NumerovParams *Num_Params, DynamicVars *Dyn_Vars) {
    double *numerov_F;
    int n;
    int nmax;

    nmax = Num_Params->nmax;

    // Allocate memory space for numerov_F[0]...numerov_F[nmax]
    numerov_F = vector_malloc(nmax+1);

    // Make numerov_F[n] to be used in the algorithm
    Numerov_Make_Fn(numerov_F, Num_Params, Dyn_Vars);

    // Get the initial values from Num_Params
    y[0] = Num_Params->y_0;
    y[1] = Num_Params->y_1;
    
    // Calculate y[n] for n >= 2
    for (n=2; n<=nmax; n++) {
        Numerov_Advance_A_Step(y, n, numerov_F, Num_Params, Dyn_Vars);
    }

    return;
} // Numerov_Advance

//
// Numerov_Make_Fn
void Numerov_Make_Fn(
    double *numerov_F, NumerovParams *Num_Params, DynamicVars *Dyn_Vars
) {
    /*
    Tabluate F in y’’(t) = F(t)y(t)
    */

    int n;
    double x_n;

    for (n=0; n<=Num_Params->nmax; n++) {
        x_n = Num_Params->x_i + Num_Params->h*n;  // x_n = x_i + h*n
        numerov_F[n] = (Num_Params->NumerovFunc_F)(x_n, Dyn_Vars);
    }

    return;
} // Numerov_Make_Fn

//
// Numerov_Advance_A_Step
void Numerov_Advance_A_Step(
    double *y, int n, double *numerov_F, NumerovParams *Num_Params, 
    DynamicVars *Dyn_Vars
) {
    /*
    Calculates y[n] = ( 1/(1 - (h^2/12)F[n]) )
                        * ( 2(1 + (5h^2/12)F[n-1])y[n-1] - (1 - (h^2/12)F[n-2])y[n-2] )
    */

    double h;
    double h2_over_12;

    h = Num_Params->h;
    h2_over_12 = h*h/12.0;

    y[n] = 0.0;
    if (y[n-1] != 0.0) {
        y[n] += 2.0*(1 + 5.0*h2_over_12*numerov_F[n-1])*y[n-1];
    }
    if (y[n-2] != 0.0) {
        y[n] -= (1 - h2_over_12*numerov_F[n-2])*y[n-2];
    }

    y[n] /= (1 - h2_over_12*numerov_F[n]);

    return;
} // Numerov_Advance_A_Step
