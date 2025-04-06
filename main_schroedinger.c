// File: main_schroedinger.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "params.h"
#include "numerov_params.h"
#include "init.h"
#include "schroedinger.h"
#include "vector_mtx.h"

// Functions for this file
// Read in Params parameters
void ReadIn_Params(char *input_file, DynamicVars *Dyn_Vars);
// Read in NumerovParams parameters
void ReadIn_Num_Params(
    char *input_file_name, NumerovParams *Num_Params_f, NumerovParams *Num_Params_b
);
// Record parameters
void Record_Params(NumerovParams Num_Params_f, NumerovParams Num_Params_b);
// Record results
void Record_Results(
    DynamicVars Dyn_Vars, NumerovParams Num_Params_f, NumerovParams Num_Params_b,
    double *yf, double *yb
);

Params PARAM_DATA;


int main(int argc, char **argv) {
    DynamicVars Dyn_Vars;  // These parameters are calculated
    NumerovParams Num_Params_f;  // For the forward evolution of u_I
    NumerovParams Num_Params_b;  // For the backward evolution of u_II
    double *yf, *yb;  // yf contains u_I, yb contains u_II

    ReadIn_Params(argv[1], &Dyn_Vars);  // for PARAM_DATA
    ReadIn_Num_Params(argv[2], &Num_Params_f, &Num_Params_b);  // for Num_Params_f and Num_Params_b
    Init_CalcScales();  // Get the energy and length scales
    Record_Params(Num_Params_f, Num_Params_b);  // Record the parameters

    // Allocate memory for the forward wavefunction yf
    yf = vector_malloc(Num_Params_f.nmax+1);
    // Allocate memory for the backward wavefunction yb
    yb = vector_malloc(Num_Params_b.nmax+1);
    Schroedinger_GetBoundState(&Dyn_Vars, &Num_Params_f, &Num_Params_b, yf, yb);
    Record_Results(Dyn_Vars, Num_Params_f, Num_Params_b, yf, yb);
    
    return 0;
} // main

//
// ReadIn_Params
void ReadIn_Params(char *input_file, DynamicVars *Dyn_Vars)
{
    // Declare variables
    FILE *input;
    double x;
    // int ix;
    char *mass_unit;

    input = fopen(input_file, "r");  // Open the input file

    // Read in the mass
    fscanf(input, "%le", &PARAM_DATA.mass);
    PARAM_DATA.mass /= hbarc;  // Divide by hbarc

    // Read in the mass unit
    mass_unit = (char *) malloc(sizeof(char)*10);  // allocate memory
    fscanf(input, "%s", mass_unit);  // read in a line
    PARAM_DATA.mass_unit = mass_unit;

    // Determine the length unit
    if (strcmp(mass_unit, "eV")==0) {PARAM_DATA.length_unit = "nm";}
    else if (strcmp(mass_unit, "MeV")==0) {PARAM_DATA.length_unit = "fm";}
    else {
        fprintf(stderr, "ReadIn_Params: %s is an unknown unit.\n", mass_unit);
        fprintf(stderr, "Known units are eV and MeV.\n");
        fprintf(stderr, "Exiting.\n");
        exit(0);
    }

    fscanf(input, "%d", &PARAM_DATA.ell);  // Orbital angular momentum
    fscanf(input, "%le", &PARAM_DATA.nucA);  // Atomic mass A
    fscanf(input, "%le", &PARAM_DATA.nucZ);  // Atomic mass Z

    // Read in the minimum value of Et for the energy eigenvalue search
    fscanf(input, "%le", &Dyn_Vars->Et_min);
    // Read in the maximum value of Et for the energy eigenvalue search
    fscanf(input, "%le", &Dyn_Vars->Et_max);

    fclose(input); // Always close an opened file
    fprintf(stderr, "Done reading in.\n");
    return;
} // ReadIn_Params

//
// Record_Params
void Record_Params(NumerovParams Num_Params_f, NumerovParams Num_Params_b) {
    double x;
    int i;
    FILE *output;

    output = fopen("schroed_params.dat","w");
    
    // Record PARAM_DATA parameters
    fprintf(output, "PARAM_DATA\n");
    fprintf(output, "mass = %e %s\n", hbarc*PARAM_DATA.mass, PARAM_DATA.mass_unit);
    fprintf(output, "r0 = %le %s\n", PARAM_DATA.r0, PARAM_DATA.length_unit);
    fprintf(output, "Ea = %le %s\n", PARAM_DATA.Ea, PARAM_DATA.mass_unit);
    fprintf(output, "ka = %le %s\n", PARAM_DATA.ka *hbarc, PARAM_DATA.length_unit);
    fprintf(output, "ell = %d\n", PARAM_DATA.ell);
    fprintf(output, "x0 = %le\n", PARAM_DATA.x0);
    fprintf(output, "nucA = %le\n", PARAM_DATA.nucA);
    fprintf(output, "nucZ = %le\n", PARAM_DATA.nucZ);
    fprintf(output, "Et_min = %le\n", PARAM_DATA.Et_min);
    fprintf(output, "Et_max = %le\n", PARAM_DATA.Et_max);

    // Record Num_Params parameters
    fprintf(output, "\nNum_Params\n");
    // Forward
    fprintf(output, "nmax_f = %d\n", Num_Params_f.nmax);
    fprintf(output, "x_i_f = %le\n", Num_Params_f.x_i);
    fprintf(output, "x_f_f = %le\n", Num_Params_f.x_f);
    fprintf(output, "y_0_f = %le\n", Num_Params_f.y_0);
    fprintf(output, "y_1_f = %le\n", Num_Params_f.y_1);
    fprintf(output, "h_f = %le\n", Num_Params_f.h);
    // Backward
    fprintf(output, "nmax_b = %d\n", Num_Params_b.nmax);
    fprintf(output, "x_i_b = %le\n", Num_Params_b.x_i);
    fprintf(output, "x_f_b = %le\n", Num_Params_b.x_f);
    fprintf(output, "y_0_b = %le\n", Num_Params_b.y_0);
    fprintf(output, "y_1_b = %le\n", Num_Params_b.y_1);
    fprintf(output, "h_b = %le\n", Num_Params_b.h);


    fclose(output);
    return;
} // Record_Params

//
// ReadIn_Num_Params
void ReadIn_Num_Params(
    char *input_file_name, NumerovParams *Num_Params_f, NumerovParams *Num_Params_b
) {
    FILE *input_file;
    double x;
    int ix;

    input_file = fopen(input_file_name, "r");

    // Read in nmax for the forward & backward evolutions
    fscanf(input_file, "%d", &Num_Params_f->nmax);
    fscanf(input_file, "%d", &Num_Params_b->nmax);

    fclose(input_file); 
    return;
} // ReadIn_Num_Params

//
// Record_Results
void Record_Results(
    DynamicVars Dyn_Vars, NumerovParams Num_Params_f, NumerovParams Num_Params_b,
    double *yf, double *yb
) {
    double Et;
    FILE *output;
    int n;
    double x;
    
    // Record the final results.
    output = fopen("schroedinger.dat","w");
    Et = Dyn_Vars.Et;
    fprintf(output, "Et = %e\n", Et);
    fprintf(output, "Eb = %e\n", Dyn_Vars.Eb);  // Eb = Et*Ea
    fclose(output);

    // Record the forward going solution
    output = fopen("yf.dat","w");
    for(n=0; n<=Num_Params_f.nmax; n++){
        x = Num_Params_f.x_i + n*Num_Params_f.h; // Dimensionless
        x /= PARAM_DATA.ka; // 1/energy
        x *= hbarc; // length dimension
        fprintf(output, "%e %e\n", x, yf[n]/yf[Num_Params_f.nmax]);
    }
    fclose(output);

    // Record the backward going solution
    output = fopen("yb.dat","w");
    for(n=0; n<=Num_Params_b.nmax; n++){
        // 0 < x’ < x_f - x_c
        x = Dyn_Vars.xf - (Num_Params_b.x_i + n*Num_Params_b.h);  // x = x_f - (x_i + n*h)
        x = x / PARAM_DATA.ka; // Convert x to the length unit: 1/energy
        fprintf(output, "%e %e\n", x, yb[n]/yb[Num_Params_b.nmax]);
    }
    fclose(output);
} // Record_Results