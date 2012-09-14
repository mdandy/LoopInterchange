#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>

void print_array(int** array, int size)
{
	int i = 0;
	int j = 0;

	for (i = 0; i < size; i++)
	{
		for (j = 0; j < size; j++)
			printf("%d\t", array[i][j]);
		printf("\n");
	}
}

int no_locality_count(int** array, int size)
{
	int i = 0;
	int j = 0;
	int sum = 0;
	
	for (j = 0; j < size; j++)
	{
		for (i = 0; i < size; i++)
			sum += array[i][j];
	}
	
	return sum;
}

int locality_count(int** array, int size)
{
	int i = 0;
	int j = 0;
	int sum = 0;
	
	for (i = 0; i < size; i++)
	{
		for (j = 0; j < size; j++)
			sum += array[i][j];
	}
	
	return sum;
}

int main (int argc, char **argv)
{
	int array_size = 0;
	int trial = 0;
	int** matrix;
	unsigned long locality_run_sum = 0;
	unsigned long no_locality_run_sum = 0;
	
	int i = 0;
	int j = 0;
	
	/** Check for command line argument **/
	if (argc != 3)
	{
		printf("Usage: %s array_size number_of_trial\n", argv[0]);
		return 0;
	}
	
	array_size = atoi(argv[1]);
	trial = atoi(argv[2]);
	
	/** Allocate memory for 2D array **/
	matrix = (int**) malloc(array_size * sizeof(int*));
	for (i = 0; i < array_size; i++)
		matrix[i] = (int*) malloc(array_size * sizeof(int));
		
	/** Initialize 2D array **/
	for (i = 0; i < array_size; i++)
	{
		for (j = 0; j < array_size; j++)
			matrix[i][j] = (i * array_size) + j + 1;
	}
	
	/** Perform array iteration **/
	/** print_array(matrix, array_size); **/
	
	printf("Perform array iteration without spatial locality...\n");
	for (i = 0; i < trial; i++)
	{
		struct timeval t0;
		struct timeval t1;
		unsigned long elapsed;

		/** No locality trial **/
		gettimeofday(&t0, NULL);
		no_locality_count(matrix, array_size);
		gettimeofday(&t1, NULL);
		elapsed= ((t1.tv_sec - t0.tv_sec) * 1000000) + (t1.tv_usec - t0.tv_usec);
		
		no_locality_run_sum += elapsed;
		/** printf("%lu ns\n", elapsed); **/
	}
	printf ("Average: %lu ns \n\n", (no_locality_run_sum / trial));
	
	printf("Perform array iteration with spatial locality...\n");
	for (i = 0; i < trial; i++)
	{
		struct timeval t0;
		struct timeval t1;
		unsigned long elapsed;

		/** Locality trial **/
		gettimeofday(&t0, NULL);
		locality_count(matrix, array_size);
		gettimeofday(&t1, NULL);
		elapsed= ((t1.tv_sec - t0.tv_sec) * 1000000) + (t1.tv_usec-t0.tv_usec);
		
		locality_run_sum += elapsed;
		/** printf("%lu ns\n", elapsed); **/
	}
	printf ("Average: %lu ns \n\n", (locality_run_sum / trial));
	
	/** Free 2D array **/
	for (i = 0; i < array_size; i++)
	  free(matrix[i]);
	free(matrix);
	
	return 0;
}