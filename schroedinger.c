// File schroedinger.c
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "numerov.h"
#include "numerov_params.h"
#include "radial_eq_functions.h"
#include "params.h"
#include "schroedinger.h"
#include "solve.h"
#include "init.h"

// Variables
NumerovParams *COM_NUM_PARAMS_F;
NumerovParams *COM_NUM_PARAMS_B;
DynamicVars *COM_DYN_VARS;
double *COM_Y_F; // Forward wavefunction
double *COM_Y_B; // Backward wavefunction

// Functions
double Schroedinger_GetDf_nmax(double *y, NumerovParams *Num_Params);
void Schroedinger_InitializeCommonVar(void);
double Schroedinger_GetError(void);
void Schroedinger_EvolveForward(void);
void Schroedinger_EvolveBackward(void);
void Schroedinger_CalcRunScales(double Et);
double Schroedinger_CalcRc(double Eb, double r_init);
double Schroedinger_GetBoundStateError(double Et);
void Schroedinger_PlotData(double Et_min, double Et_max);

//
// Schroedinger_GetBoundState
void Schroedinger_GetBoundState(DynamicVars *Dyn_Vars, NumerovParams *Num_Params_f,
    NumerovParams *Num_Params_b, double *yf, double *yb) {
    double Et_min, Et_max, tol, err;
    int count;
    double x, y;
    COM_NUM_PARAMS_F = Num_Params_f;
    COM_NUM_PARAMS_B = Num_Params_b;
    COM_DYN_VARS = Dyn_Vars;
    COM_Y_F = yf;
    COM_Y_B = yb;
    Et_min = Dyn_Vars->Et_min;
    Et_max = Dyn_Vars->Et_max;

    // To plot the error err = u’_I/u_I - u’_II/u_II at x = xc
    Schroedinger_PlotData(Et_min, Et_max);
    
    count = 0;
    tol = 1.0e-6;
    Solve_Bisect(0.0, Schroedinger_GetBoundStateError, Et_min, Et_max, tol, &count);
    fprintf(stderr, "count = %d\n", count);
    
    return;
}// Schroedinger_GetBoundState

//
// Schroedinger_PlotData
void Schroedinger_PlotData(double Et_min, double Et_max) {
    /*
    To plot err = u’_I/u_I - u’_II/u_II at x = xc between Et_min and Et_max
    */

    FILE *output;
    int n, nmax;
    double dEt, Et, err;

    output = fopen("schroedinger_plot.dat", "w");

    nmax = 1000;
    dEt = (Et_max - Et_min)/(double)nmax;
    
    for(n=0; n<=nmax; n++){
        Et = Et_min + n*dEt;
        err = Schroedinger_GetBoundStateError(Et);
        fprintf(output, "%e %e\n", Et, err);
    }

    fclose(output);

    return;
}// Schroedinger_Plot_Data

//
// Schroedinger_GetBoundStateError
double Schroedinger_GetBoundStateError(double Et) {
    /*
    err = u’_I/u_I - u’_II/u_II at x = xc
    */

    double err;

    Schroedinger_CalcRunScales(Et);
    Schroedinger_InitializeCommonVar();
    Schroedinger_EvolveForward();
    Schroedinger_EvolveBackward();

    err = Schroedinger_GetError();

    return err;
}// Schroedinger_GetBoundStateError

//
// Schroedinger_CalcRunScales
void Schroedinger_CalcRunScales(double Et) {
    /*
    Get the classical turning point r_c and use it to set xc and xf
    */

    double r_init, r_min, r_c, Ea, Eb;
    Ea = PARAM_DATA.Ea; // Energy scale
    Eb = Et*Ea; // Energy corresponding to Et

    COM_DYN_VARS->kb = sqrt(2.0*Eb*PARAM_DATA.mass);
    COM_DYN_VARS->Et = Et;
    COM_DYN_VARS->Eb = Eb;

    r_min = PARAM_DATA.r0;
    r_init = 1.1*r_min;

    r_c = Schroedinger_CalcRc(Eb, r_init);  // Classical turning point
    COM_DYN_VARS->rc = r_c;
    COM_DYN_VARS->xc = r_c*PARAM_DATA.ka;

    // The wavefunction behaves like exp(-(kb/ka)*x)
    COM_DYN_VARS->xf = 20.0*PARAM_DATA.ka/COM_DYN_VARS->kb;  // x_f = 20*(ka/kb)
    COM_DYN_VARS->rf = COM_DYN_VARS->xf/PARAM_DATA.ka;  // r_f = x_f/ka
    
    return;
}// Schroedinger_CalcRunScales

//
// Schroedinger_CalcRc
double Schroedinger_CalcRc(double Eb, double r_init) {
    /*
    Get the turning point larger than Rmin by solving V(r) + ell(ell+1)/(2mu r^2) = -Eb
    */

    double f, E_min, r_c, tol;
    int count;

    // solve -Eb = V_eff(r)
    tol = 1.0e-8;
    count = 0;
    r_c = Solve_Newton(-Eb, RadialEqFunctions_Veff, r_init, tol, &count);
    
    return r_c;
}// Schroedinger_CalcRc

//
// Schroedinger_InitializeCommonVar
void Schroedinger_InitializeCommonVar(void) {
    /*
    Initialize all other necessary variables
    */
    double kb, rf, h;

    // Forward evolution parameters
    COM_NUM_PARAMS_F->x_i = 0.0;
    COM_NUM_PARAMS_F->x_f = COM_DYN_VARS->xc;
    COM_NUM_PARAMS_F->y_0 = 0.0;
    COM_NUM_PARAMS_F->y_1 = 0.1;
    COM_NUM_PARAMS_F->h = (COM_NUM_PARAMS_F->x_f - COM_NUM_PARAMS_F->x_i)/(double)(COM_NUM_PARAMS_F->nmax);
    
    // Backward evolution params
    // The wavefunction behaves like exp(-(kb/ka)*x)where kb = sqrt(2*mass*Eb)
    COM_NUM_PARAMS_B->x_i = 0.0;
    COM_NUM_PARAMS_B->x_f = COM_DYN_VARS->xf - COM_DYN_VARS->xc;
    COM_NUM_PARAMS_B->h = (COM_NUM_PARAMS_B->x_f - COM_NUM_PARAMS_B->x_i)/(double)(COM_NUM_PARAMS_B->nmax);

    kb = COM_DYN_VARS->kb;
    rf = COM_DYN_VARS->rf;
    h = COM_NUM_PARAMS_B->h;

    COM_NUM_PARAMS_B->y_0 = exp(-kb*rf);
    COM_NUM_PARAMS_B->y_1 = exp(-kb*(rf-h));

    return;
}// Schroedinger_InitializeCommonVar

//
// Schroedinger_EvolveForward
void Schroedinger_EvolveForward(void) {
    double f, df;
    int nmax;
    double *yf;
    NumerovParams *Num_Params_f;
    DynamicVars *Dyn_Vars_f;

    yf = COM_Y_F;
    Num_Params_f = COM_NUM_PARAMS_F;
    Dyn_Vars_f = COM_DYN_VARS;

    nmax = Num_Params_f->nmax;
    Num_Params_f->NumerovFunc_F = RadialEqFunctions_F_Forward;
    Numerov_Advance(yf, Num_Params_f, Dyn_Vars_f);
    
    return;
}// Schroedinger_EvolveForward

//
// Schroedinger_EvolveBackward
void Schroedinger_EvolveBackward(void) {
    double f, df;
    int nmax;
    double *yb;
    NumerovParams *Num_Params_b;
    DynamicVars *Dyn_Vars_b;

    yb = COM_Y_B;
    Num_Params_b = COM_NUM_PARAMS_B;
    Dyn_Vars_b = COM_DYN_VARS;

    nmax = Num_Params_b->nmax;
    Num_Params_b->NumerovFunc_F = RadialEqFunctions_F_Backward;
    Numerov_Advance(yb, Num_Params_b, Dyn_Vars_b);
    
    return;
}// Schroedinger_EvolveBackward

//
// Schroedinger_GetDf_nmax
double Schroedinger_GetDf_nmax(double *y, NumerovParams *Num_Params) {
    /*
    Implements the numerical derivative at the end of the list.
    Given y[n-2], y[n-1], y[n], get the slope at y[n].
    Given these 3 points, the slope at x_n is given by y’[n] = (3y[n] - 4y[n-1] + y[n-2])/(2h)
    */

    double df, h;
    int nmax;

    nmax = Num_Params->nmax;
    h = Num_Params->h;

    // df = (3y[n] - 4y[n-1] + y[n-2])/(2h) with n=nmax
    df = (3.0*y[nmax] - 4.0*y[nmax-1] + y[nmax-2])/(2.0*h);
    
    return df;
}// Schroedinger_GetDf_nmax

//
// Schroedinger_GetError
double Schroedinger_GetError(void) {
    double *yf, *yb;
    NumerovParams *Num_Params_f, *Num_Params_b;
    double df, df_f, df_b;

    Num_Params_f = COM_NUM_PARAMS_F;
    Num_Params_b = COM_NUM_PARAMS_B;
    yf = COM_Y_F;
    yb = COM_Y_B;

    // Get the derivative at nmax (forward)
    df = Schroedinger_GetDf_nmax(yf, Num_Params_f);
    df_f = df/yf[Num_Params_f->nmax];

    // Get the derivative at nmax (backward)
    df = Schroedinger_GetDf_nmax(yb, Num_Params_b);
    df_b = df/yb[Num_Params_b->nmax];

    // Get the error
    df = df_f - df_b;
    
    return df;
}// Schroedinger_GetError