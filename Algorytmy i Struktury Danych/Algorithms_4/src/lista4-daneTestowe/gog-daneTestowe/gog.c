#include <stdio.h>
#include <stdlib.h>

void quick_sort (int **a, int n) {
    int i, j, p, t;
    if (n < 2)
        return;
    p = a[n / 2][1];
    for (i = 0, j = n - 1;; i++, j--) {
        while (a[i][1] > p)
            i++;
        while (p > a[j][1])
            j--;
        if (i >= j)
            break;
        t = a[i][0];
        a[i][0] = a[j][0];
        a[j][0] = t;

        t = a[i][1];
        a[i][1] = a[j][1];
        a[j][1] = t;
    }
    quick_sort(a, i);
    quick_sort(a + i, n - i);
}

void bubble_sort (int **a, int n) {
    int i, t, s = 1;
    while (s) {
        s = 0;
        for (i = 1; i < n; i++) {
            if (a[i][0] < a[i - 1][0] && a[i][1] >= a[i-1][1] ) {
                t = a[i][0];
                a[i][0] = a[i - 1][0];
                a[i - 1][0] = t;
                s = 1;
            }
        }
    }
}

int main(int argc, char *argv[]) {

	FILE *file;
	int n;
	int **array;

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
	array = (int **) malloc(n*sizeof(int *));
	for (int i = 0; i < n; i++) {
		array[i] = malloc(2*sizeof(int));
	}

	int p, f;
	for (int i = 0; i < n; i++) {
		fscanf(file, "%d", &p);
		fscanf(file, "%d", &f);
		array[i][0] = p;
		array[i][1] = f;
	}
	quick_sort(array, n);
	bubble_sort(array, n);

	int super_counter = 0, normal_counter = 0, max = 0;

	for (int i = 0; i < n; i++) {
		super_counter += array[i][0];
		normal_counter = (super_counter + array[i][1]);
		if (normal_counter > max) max = normal_counter;
	}

	for (int i = 0; i < n; i++) {
		printf("%d %d\n", array[i][0], array[i][1]);
	}

	printf("%d\n", max);

	return 0;
}
