# PHYS457 - Honours Quantum II
Audréanne Bernier (261100643)

## **How to Compile and Run**  

### **Compiling the Code**  
To compile all files, run the following command in the terminal:  

```sh
make
```

To compile only specific parts of the code, use:  

```sh
make solve_test
make deriv_test
make extremum_test
make numerov_test
make schrodinger_test
```  

### **Running the Tests**  
After compiling, you can run the test scripts with:  

```sh
./solve_test
./deriv_test
./extremum_test
./numerov_test filename
./schrodinger_test input_file1 input_file2
```  

where `filename` is the input file containing the parameters, formatted as:  

```
x_i  
x_f  
y_0  
y_1  
nmax
```  

`input_file1` contains 

```
mass  
mass_unit  
l  
nucA  
nucZ
``` 

and `input_file2` contains the Numerov parameters formatted as:

```
nmax_forward
nmax_backward
``` 


## **Input and Output Files** 

### **deriv_test**

Output: `deriv_test.dat`

### **numerov_test**

Input: `params_test.txt` is an example file that can be used when running numerov_test. 

Output: `params.dat` (where the read-in data is recorded) and `y.dat` (contains the 
resulting y values from the Numerov method).

For testing purposes, an additional Jupyter Notebook `numerov_plot.ipynb`is provided to 
plot the results from numerov_test  stored in `y.dat`.

### **schrodinger_test**

Input: `input_coulomb.txt` and `input_n_params.txt` are example files that can be used 
respectively as `input_file1`and `input_file2` when running schrodinger_test.

Output: `num_params.dat`