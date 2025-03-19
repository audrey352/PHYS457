// File main_schroedinger.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "params.h"
#include "numerov_params.h"
#include "init.h"
//#include "schroedinger.h"
#include "vector_mtx.h"

// Functions for this file
// Read in Params parameters
void ReadIn_Params(char *input_file);
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

    ReadIn_Params(argv[1]);  // for PARAM_DATA
    ReadIn_Num_Params(argv[2], &Num_Params_f, &Num_Params_b);  // for Num_Params_f and Num_Params_b
    Init_CalcScales();  // Get the energy and length scales
    Record_Params(Num_Params_f, Num_Params_b);  // Record the parameters

    /* Will be implemented in the next section */
    /*
    // Allocate memory for the forward wavefunction yf
    yf = vector_malloc(Num_Params_f.nmax+1);
    // Allocate memory for the backward wavefunction yb
    yb = vector_malloc(Num_Params_b.nmax+1);
    Schroedinger_GetBoundState(&Dyn_Vars, &Num_Params_f, &Num_Params_b, yf, yb);
    Record_Results(Dyn_Vars, Num_Params_f, Num_Params_b, yf, yb);
    */
    
    return 0;
} // main

//
// ReadIn_Params
void ReadIn_Params(char *input_file)
{
    // Declare variables
    FILE *input;
    // double x;
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

    fclose(input); // Always close an opened file
    return;
} // ReadIn_Params

//
// Record_Params
void Record_Params(NumerovParams Num_Params_f, NumerovParams Num_Params_b) {
    double x;
    int i;
    FILE *output;

    output = fopen("num_params.dat","w");
    
    // Record parameters in the data file.
    fprintf(output, "mass = %le\n", PARAM_DATA.mass);  // divided by hbarc in ReadIn_Params
    fprintf(output, "r0 = %le\n", PARAM_DATA.r0);
    fprintf(output, "Ea = %le\n", PARAM_DATA.Ea);  // divided by hbarc since uses mass
    fprintf(output, "ka = %le\n", PARAM_DATA.ka);
    fprintf(output, "ell = %d\n", PARAM_DATA.ell);
    fprintf(output, "x0 = %le\n", PARAM_DATA.x0);
    fprintf(output, "nucA = %le\n", PARAM_DATA.nucA);
    fprintf(output, "nucZ = %le\n", PARAM_DATA.nucZ);

    fprintf(output, "nmax_f = %d\n", Num_Params_f.nmax);
    fprintf(output, "nmax_b = %d\n", Num_Params_b.nmax);

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
    // Next section
    return;
} // Record_Results