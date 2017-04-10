#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int main(int argc, char *argv[]) {

	FILE* file;

	int n, number;
	int* A;
	int* T;

	if ((file=fopen(argv[1], "r"))==NULL) {
		printf ("I can't open file %s to read!\n", argv[1]);
		exit(1);
	}
	else {
		fscanf(file, "%d", &n);
		printf("n = %d\n", n);
	}
	//Checking the conditions of exercise
	if(n <= 0 || n > 1000000 ){
		printf("You gave wrong n. Program will exit. \n");
		return 0;
	}

	A = malloc(n * sizeof(int));
	T = malloc(n * sizeof(int));
	T[0] = 0;

	for (int i = 1; i < n; i++) {
		T[i] = INT_MAX;
	}

	for (int i = 0; i < n; i++) {
		fscanf(file, "%d", &number);
		A[i] = number;
		//printf("%d ", number);
	}

	for (int k = 0; k < n; k++) {
		printf("k = %d\n", k);
		for(int s = 0; s < n; s++) {
			if ((T[s] < A[k]) && (T[s+1] > A[k])) {
				T[s+1] = A[k];
				//printf("%d\n", s+1);
				break;
			}
		}
	}

	int s = 0;
	while(T[s] < INT_MAX) {
		s++;
	}
	s--; //Tablica zaczyna sie od 0 -> musimy odjac 1
	printf("%d\n", s);

	return 0;
}
