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
```  

### **Running the Tests**  
After compiling, you can run the test scripts with:  

```sh
./solve_test
./deriv_test
./extremum_test
./numerov_test filename
```  

where `filename` is the input file containing the parameters, formatted as:  

```
x_i  
x_f  
y_0  
y_1  
nmax  
```  

For testing purposes, an additional Jupyter Notebook is provided to plot the results 
from numerov_test alongside the corresponding input file `params_test.txt`.