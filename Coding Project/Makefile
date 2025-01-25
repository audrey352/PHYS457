main_solve_test: main_solve.o solve.o
	gcc -o igdb main_solve.o solve.o

main_solve.o: main_solve.c solve.h
	gcc -c main_solve.c

solve.o: solve.c solve.h
	gcc -c solve.c
