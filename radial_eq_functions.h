// File radial_eq_functions.h
#ifndef RADIAL_EQ_FUNCTIONS_H
#define RADIAL_EQ_FUNCTIONS_H
#include "params.h"

// Potential energy in r
double RadialEqFunctions_V(double r);

// V_eff = V + ell(ell+1)/(2m r^2)
double RadialEqFunctions_Veff(double r);

// Evolution forward from x = x_i
double RadialEqFunctions_F_Forward(double x, DynamicVars *Dyn_Vars);
// Evolution backward from x = x_f
double RadialEqFunctions_F_Backward(double x, DynamicVars *Dyn_Vars);

#endif