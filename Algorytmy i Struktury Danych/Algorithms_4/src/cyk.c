#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct neighbourList {
	int value;
	struct neighbourList *next; //Wskaznik na kolejny element listy sasiedztwa
}neighbourList;

bool isCyclic(struct neighbourList ** graph, int value, bool * visited) {

	neighbourList * p; //Wskaznik na element listy sąsiedztwa
	int u;

	visited[value] = true;
	p = graph[value];

	while(p != NULL) {
		u = p->value;
		if (visited[u] == true) {
			return true;
		}
		if (visited[u] != true && isCyclic(graph, u, visited) == true) {
			return true;
		}
		p = p->next;
	}

	return false;
}



int main(int argc, char *argv[]) {

	FILE *file;

	neighbourList ** graph; //Tworzymy tablicę list sasiedztwa
	bool * visited;

	int n, m;

	if ((file=fopen(argv[1], "r"))==NULL) {
				printf ("I can't open file %s to read!\n", argv[1]);
				exit(1);
	}
	else {
		fscanf(file, "%d", &n);
		fscanf(file, "%d", &m);
	}
	//Checking the conditions of exercise
	if(n <= 0 || n > 100000 || m <= 0 || m > 200000){
		printf("You gave wrong n. Program will exit. \n");
		return 0;
	}

	graph = malloc(sizeof(n * sizeof(neighbourList*)));
	visited = malloc(sizeof (n * sizeof(bool)));

	for (int i = 0; i < n; i++) graph[i] = NULL; //"Zerowanie" grafu

	for (int i = 0; i < n; i++) visited[i] = false;

	int vertex1, vertex2;
	for (int i = 0; i < m; i++) {
		fscanf(file, "%d", &vertex1);
		fscanf(file, "%d", &vertex2);
		printf ("%d - %d\n", vertex1, vertex2);

		neighbourList * p;
		p->value = vertex2;
		p->next = graph[vertex1];
		graph[vertex1] = p;
	}
	printf("Doszlo tu\n");

	for (int i = 0; i < n; i++) {

		for(int j = 0; j < n; j++) {         // Zerujemy tablicę odwiedzin
		      visited[j] = false;
		}

		if (isCyclic(graph, i, visited) == true) {
			printf("TAK\n");
			break;
		}
	}

	return 0;
}
