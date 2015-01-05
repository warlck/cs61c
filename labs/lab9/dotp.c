#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

#define ARRAY_SIZE 1000000
#define REPEAT     100

double* gen_array(int n) {
	double* array = (double*) malloc(n*sizeof(double));
	for(int i=0; i<n; i++)
		array[i] = drand48();
	return array;
}

/*
double dotp(double* x, double* y) {
	double global_sum = 0.0;   //loop unrolling?
	//#pragma omp parallel private(nthreads)
	{
		double temp = 0.0;
		#pragma omp for
		for(int i=0; i<ARRAY_SIZE; i++)
			temp += x[i] * y[i]; //slow? if here since only one of parallelized threads in for loop will get it
		#pragma omp critical  //critical specifies region of code that is executed 1 thread at a time, slow? 
		global_sum += temp;  //so only one will add once instead of doing so multiple times when critical was located in for loop
	}  //using temp variable to hold accumulated thread values
	return global_sum;
} */

double dotp(double* x, double* y) {
	double global_sum = 0.0;
	#pragma omp parallel 
	{ //no temps needed when using reduction keyword
		//double temp = 0.0;
		#pragma omp for reduction(+:global_sum)
		for (int i = 0; i < ARRAY_SIZE; i++)
			//#pragma omp critical //just added this, no need for this
			global_sum += x[i] * y[i];  
	}
	return global_sum;
} 

int main() {
	// Generate input vectors
	double *x = gen_array(ARRAY_SIZE);
	double *y = gen_array(ARRAY_SIZE);
	double serial_result = 0.0;
	double result;

	// calculate result serially
	for(int i=0; i<ARRAY_SIZE; i++)
		serial_result += x[i] * y[i];

	// Test framework that sweeps the number of threads and times each ru
	double start_time, run_time;
	int num_threads = omp_get_max_threads();
	for(int i=1; i<=num_threads; i++) {
		omp_set_num_threads(i);
		start_time = omp_get_wtime();
		for(int j=0; j<REPEAT; j++)
			result = dotp(x,y);
		run_time = omp_get_wtime() - start_time;
  		printf(" %d thread(s) took %f seconds\n",i,run_time);

		// verify result is correct (within some threshold)
		if (fabs(serial_result - result) > 0.001) {
			printf("Incorrect result!\n");
			return -1;
		}
	}
	return 0;
}
