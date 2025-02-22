// File main_numerov.c
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "vector_mtx.h"
#include "numerov.h"
#include "numerov_params.h"
#include "params.h"

// Function to test
double TestF(double x, DynamicVars *Dyn_Vars); // Test purposes

void ReadInNum_Params(char *input_file_name, NumerovParams *Num_Params);
void PrintParams(NumerovParams Num_Params);
void PrintY(double *y, NumerovParams Num_Params);

//
// Define the function to test
int main(int argc, char **argv) {
    double *y; // 1-D array of doubles
    char *input; // 1-D array of characters

    NumerovParams Num_Params; // To hold static data needed for Numerov method
    DynamicVars Dyn_Vars; // To hold data that may change during the calculations
    
    input = argv[1];
    
    ReadInNum_Params(input, &Num_Params); // Fill Num_Params with data from input file
    PrintParams(Num_Params);  // Record the read-in data in an output file

    y = vector_malloc(Num_Params.nmax+1); // Allocate memory for y
    
    Num_Params.NumerovFunc_F = TestF; // Assign the F function in y’’ = Fy

    Numerov_Advance(y, &Num_Params, &Dyn_Vars); // Solve y’’ = Fy
    PrintY(y, Num_Params); // Print y to an output file

    free(y); // free allocated memory
    return 0;
} // main

//
// Defining function to test
double TestF(double x, DynamicVars *Dyn_Vars) {
    return -x; // Airy
    // return -1.0; // SHO
} // TestF

//
// ReadInNum_Params
void ReadInNum_Params(char *input_file_name, NumerovParams *Num_Params) {
    FILE *input_file;
    double x;
    int ix;

    input_file = fopen(input_file_name, "r"); // Open the input file

    // Read in the first line and put its value in x
    fscanf(input_file, "%le", &x);
    Num_Params->x_i = x;

    // Read in x_f, y_0 and y_1
    fscanf(input_file, "%le", &Num_Params->x_f);
    fscanf(input_file, "%le", &Num_Params->y_0);
    fscanf(input_file, "%le", &Num_Params->y_1);

    // Read in an integer value and put its value in ix
    fscanf(input_file, "%d", &ix);
    Num_Params->nmax = ix;

    // Calculate h = (x_f - x_i)/nmax
    double h = (Num_Params->x_f - Num_Params->x_i)/Num_Params->nmax;
    Num_Params->h = h;

    fclose(input_file); // Close the input file
    return;
} // ReadInNum_Params

//
// PrintParams
void PrintParams(NumerovParams Num_Params) {
    FILE *output;
    output = fopen("params.dat","w");

    // Print x and y values
    fprintf(output, "x_i = %e\n", Num_Params.x_i);
    fprintf(output, "x_f = %e\n", Num_Params.x_f);
    fprintf(output, "y_0 = %e\n", Num_Params.y_0);
    fprintf(output, "y_1 = %e\n", Num_Params.y_1);

    // Print other params
    fprintf(output, "nmax = %d\n", Num_Params.nmax);
    fprintf(output, "h = %e\n", Num_Params.h);

    fclose(output);
    return;
} // PrintParams

//
// PrintY
void PrintY(double *y, NumerovParams Num_Params) {
    FILE *output;
    int n;
    double xn;

    output = fopen("y.dat", "w");

    for(n=0; n<=Num_Params.nmax; n++) {
        xn = Num_Params.x_i + n*Num_Params.h;  // xn = x_i + n*h
        fprintf(output, "%e %e\n", xn, y[n]);  // print xn and y[n] to output file
    }

    fclose(output);
    return;
} // PrintY
