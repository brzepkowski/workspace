#include <stdio.h>
#include <stdlib.h>

static int *array;
//static int *lengths;
//-------http://rosettacode.org/wiki/Longest_increasing_subsequence#C-----
struct node {
	int val, len;
	struct node *next;
};

void lis(int *v, int len)
{
	int i;
	struct node *p, *n = calloc(len, sizeof *n);
	for (i = 0; i < len; i++) {
		n[i].val = v[i];
	}

	for (i = len; i--; ) {
		// find longest chain that can follow n[i]
		for (p = n + i; p++ < n + len; ) {
			if (p->val > n[i].val && p->len >= n[i].len) {
				n[i].next = p;
				n[i].len = p->len + 1;
			}
		}
	}

	// find longest chain
	for (i = 0, p = n; i < len; i++)
		if (n[i].len > p->len) p = n + i;

	//do printf(" %d", p->val); while ((p = p->next));
	printf("%d\n", p->len);

	free(n);
}
//-----------------------------------------------------------------
int main(int argc, char *argv[]) {


	FILE *file;

	//int* array;

	int n, number, longest_sub = 0;
	//int* lenghts;

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

	array =malloc(n * sizeof(int));
/*	lengths = malloc(sizeof (n * sizeof(int)));
	memset(lengths, 0, sizeof(lengths));
*/
	for (int i = 0; i < n; i++) {
		fscanf(file, "%d", &number);
		array[i] = number;
		//printf("%d ", number);
	}
	//printf("\n");
/*
	for (int i = n-2; i >= 0; i--) {
			for (int j = i+1; j<n; j++) {
				if (array[j] > array[i]){
					if ((lengths[j]+1) > lengths[i]) {
					lengths[i] = lengths[j]+1;
					}
				}
			}
			printf("\n");
		}

		longest_sub = lengths[0];
		for (int i = 0; i < n; i++) {
			if (lengths[i] > longest_sub) longest_sub = lengths[i];
		}
		printf("%d\n", longest_sub);

*/

	lis(array, n);

	return 0;
}
