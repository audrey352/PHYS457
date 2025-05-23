// File radial_eq_functions.c
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "radial_eq_functions.h"
#include "numerov_params.h"
#include "params.h"

#define alpha_EM (1.0/137.0) // The fine structure constant

// User defined function:
double RadialEqFunctions_V(double r) {
    /*
    The only function one should change, in the unit of 1/fm or 1/nm
    */

    double f;
    double A, R_A, a, R0, V0;

    if (PARAM_DATA.nucA == 0.0) {
        // Coulomb potential
        f = -alpha_EM/r;
    }
    else {
        // Nuclear Woods-Saxon potential
        V0 = 50.0/hbarc; // MeV to 1/fm
        a = 0.7;
        R_A = PARAM_DATA.r0;
        f = -V0/(1.0 + exp((r-R_A)/a));
    }

    return f;
}   // RadialEqFunctions_V

//
// RadialEqFunctions_Veff
double RadialEqFunctions_Veff(double r) {
    /*
    Veff(r) = V(r) + ell(ell+1)/(2 m r^2)
    */
   
    double f, ell, mass;

    ell = (double) PARAM_DATA.ell; 
    mass = PARAM_DATA.mass;
    f = RadialEqFunctions_V(r) + ell*(ell+1)/(2.0*mass*r*r);

    return f;
} // RadialEqFunctions_Veff

//
// RadialEqFunctions_F_Forward
double RadialEqFunctions_F_Forward(double x, DynamicVars *Dyn_Vars) {
    /*
    F in y’’ = Fy for u_I, this is in x = ka*r
    */
    double x0, ka, r, f, Ea, Et, ell, eps;

    ell = (double) PARAM_DATA.ell;
    x0 = PARAM_DATA.x0;
    ka = PARAM_DATA.ka;
    Ea = PARAM_DATA.Ea;
    Et = Dyn_Vars->Et;

    eps = 1.0e-15;  // Small number to prevent x = 0
    x += eps;
    r = x/ka;
    
    // f = ell(ell+1)/x^2 + V(r)/Ea + Et
    f = ell*(ell+1)/(x*x) + RadialEqFunctions_V(r)/Ea + Et;
    
    return f;
} // RadialEqFunctions_F_Forward

//
// RadialEqFunctions_F_Backward
double RadialEqFunctions_F_Backward(double y, DynamicVars *Dyn_Vars) {
    /*
    F in Y’’ = FY Backward evolution, we use y = x_f - x
    */
    double x0, ka, r, f, ell, Ea, Et, x;

    // Get parameters from PARAM_DATA
    ell = (double) PARAM_DATA.ell;
    x0 = PARAM_DATA.x0;
    ka = PARAM_DATA.ka;
    Ea = PARAM_DATA.Ea;
    Et = Dyn_Vars->Et;

    // Get x and r
    x = Dyn_Vars->xf - y;
    r = x/ka;

    // f = ell(ell+1)/x^2 + V(r)/Ea + Et
    f = ell*(ell+1)/(x*x) + RadialEqFunctions_V(r)/Ea + Et;
    
    return f;
} // RadialEqFunctions_F_Backward