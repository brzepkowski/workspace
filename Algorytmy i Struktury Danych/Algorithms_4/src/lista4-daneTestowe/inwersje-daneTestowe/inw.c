#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {

	FILE *file;

	int * array;
	int n, number, counter = 0;

	if ((file=fopen(argv[1], "r"))==NULL) {
					printf ("I can't open file %s to read!\n", argv[1]);
					exit(1);
		}
		else {
			fscanf(file, "%d", &n);
		}
		//Checking the conditions of exercise
		if(n <= 0 || n > 1000000 ){
			printf("You gave wrong n. Program will exit. \n");
			return 0;
		}

	array = malloc (n * sizeof(int));


	for (int i = 0; i < n; i++) {
		fscanf(file, "%d", &number);
		array[i] = number;
		//printf("array[%d] = %d\n", i, number);
	}

	for (int i = 0; i < n; i++) {
		for (int j = i; j < n; j++) {
			if (array[i] > (2*array[j])) {
				counter++;
			}
		}
	}
	printf("%d\n", counter);
	return 0;
}
