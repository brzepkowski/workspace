#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static int table[1000000][3];
static int possible_ways[1000000][3];

//Partially from http://www.algorytm.org/algorytmy-sortowania/sortowanie-stogowe-heapsort/heap-2-c.html
//********************************************************
void heapify (int heap_size, int i) {
    int largest, temp[3] = {0, 0, 0};
    int l=2*i, r=(2*i)+1;
    if (l<=heap_size && possible_ways[l][2]>possible_ways[i][2])
        largest=l;
    else largest=i;
    if (r<=heap_size && possible_ways[r][2]>possible_ways[largest][2])
        largest=r;
    if (largest!=i) {
        temp[0]=possible_ways[largest][0];
        temp[1]=possible_ways[largest][1];
        temp[2]=possible_ways[largest][2];

        possible_ways[largest][0]=possible_ways[i][0];
        possible_ways[largest][1]=possible_ways[i][1];
        possible_ways[largest][2]=possible_ways[i][2];

        possible_ways[i][0]=temp[0];
        possible_ways[i][1]=temp[1];
        possible_ways[i][2]=temp[2];

        heapify(heap_size,largest);
    }
}

void create_Heap(int size) {
    for (int i=size/2;i>=0;i--) {
        heapify(size, i);
    }
}

void sort(int size) {
    int temp[3] = {0, 0, 0};
    create_Heap(size);
    for (int i=size;i>0;i--) {
        temp[0]=possible_ways[i][0];
        temp[1]=possible_ways[i][1];
        temp[2]=possible_ways[i][2];

        possible_ways[i][0]=possible_ways[0][0];
        possible_ways[i][1]=possible_ways[0][1];
        possible_ways[i][2]=possible_ways[0][2];

        possible_ways[0][0]=temp[0];
        possible_ways[0][1]=temp[1];
        possible_ways[0][2]=temp[2];

        size--;
        heapify(size,0);
    }
}
//************************************************************

int main(int argc, char *argv[]) {
    FILE *file;
    int cost = 0; // !!! TOTAL COST OF MINIMAL SPANNING TREE

    int n = 0, m = 0, mult = 10; //n - number of vertexes m- number of edges, mult - necessary value by which we will multiply
    int i = 0,j = 0, k = 0;    // i and j are indexes in "table", k is needed to find cycles in graph
    int buf = 0;    // buf - buffered number which will be seved in "table"
    char c;

    memset(table, 0, sizeof(table[0][0]) * 1000000 * 3);
    file = fopen("data.txt", "r");

    if ((file=fopen(argv[1], "r"))==NULL) {
                printf ("I can't open file %s to read!\n", argv[1]);
                exit(1);
        }

    // First while is to get the value of n
    while ((c = fgetc(file)) != ' ') {
        c = c- 48; // We are getting normal value from ASCII
        n = n*mult + c;
        //mult = mult*10;
    }
    //mult = 1;
    //Second while is to get value of m
    while ((c = fgetc(file)) != '\n') {
        c = c- 48; // We are getting normal value from ASCII
        m = m*mult + c;
        //mult = mult*10;
    }
    printf("n = %d, m = %d \n", n, m);
    c = fgetc(file);    //We are moving one step further than '\n' sign
    //mult = 1;

    // Moving all number to the "table"
    while (c != EOF) {
        if (c == ' ') {
            table[i][j] = buf;

            buf = 0;
            //mult = 1;
            j++;
        }
        else if (c == '\n') {
            table[i][j] = buf;

            buf = 0;
            //mult = 1;
            i++;
            j = 0;
        }
        else {
            c = c - 48;
            buf = buf*mult + c;
            //mult = mult*10;
        }
        c = fgetc(file);
    }
/*
    for(i = 0; i < m; i++) {
        for(j = 0; j <= 2; j++) {
            printf("%d ", table[i][j]);
        }
        printf("\n");
    }

    printf("------------\n");
*/
    //Create table of possible ways and already visited vertexes
    int visited[n];
    memset(visited, 0, sizeof(visited));
    int ways = 0;
    memset(possible_ways, 0, sizeof(possible_ways[0][0]) * 1000000 * 3);

    //Take first vertex from the "table"
    int first_vertex = table[0][0];
    int checked_vertex_one = 0, checked_vertex_two = 0;
    int visited_quantity = 0; //It will be also length of table "visited"
    int used_vertex_one = 0, used_vertex_two = 0;
    int vertex_added_in_last_run = 0;
    int checked_ways = 0;

    visited[0] = table[0][0];
    visited_quantity++;
    vertex_added_in_last_run = table[0][0]; //In this place vertex_added_in_last_run means that this is first_vertex

while (visited_quantity != n && checked_ways < m) {
/*
    printf("---->Checking way: %d\n", checked_ways);
    for(i = 0; i < m; i++) {
        for(j = 0; j <= 2; j++) {
            printf("%d ", table[i][j]);
        }
        printf("\n");
    }*/
    i = 0;
    while (i < m) {
        if (table[i][0] == vertex_added_in_last_run) {
            possible_ways[ways][0] = table[i][0];
            possible_ways[ways][1] = table[i][1];
            possible_ways[ways][2] = table[i][2];
            ways++;
     }
        else if (table[i][1] == vertex_added_in_last_run) {
            possible_ways[ways][0] = table[i][0];
            possible_ways[ways][1] = table[i][1];
            possible_ways[ways][2] = table[i][2];
            ways++;
        }
        i++;
    }

    //Huge removal from "table"
    for (i = 0; i < ways; i++) {
        j = 0;
        while (j < m) {
            if (possible_ways[i][0] == table[j][0] && possible_ways[i][1] == table[j][1] && possible_ways[i][2] == table[j][2]) {
                while(j < m) {
                    table[j][0] = table[j+1][0];
                    table[j][1] = table[j+1][1];
                    table[j][2] = table[j+1][2];
                    j++;
                }
            }
            j++;
        }

    }


/*
    //!!!! Here we can write a case, when ways = 1, which means, that there is no way out of our group -> error
    printf("Possible ways: \n");
    for (i = 0; i < ways; i++) {
        for (j = 0; j < 3; j++) {
            printf("%d ", possible_ways[i][j]);
        }
        printf("\n");
    }
*/
    int size = ways-1;
    sort (size);
/*
    printf("\nSorted possible ways: \n");
    for (i=0;i< ways;i++) {
        printf("%d %d %d\n", possible_ways[i][0], possible_ways[i][1], possible_ways[i][2]);
    }
*/
    checked_vertex_one = possible_ways[0][0];
    checked_vertex_two = possible_ways[0][1];
    i = 0;
    while (i < visited_quantity) {
        if (visited[i] == checked_vertex_one) {
            used_vertex_one = 1;
        }
        if(visited[i] == checked_vertex_two) {
            used_vertex_two = 1;
        }
        i++;
    }
    if (used_vertex_one == 1 && used_vertex_two == 1) {
        //printf("We have cycle\n");
        //We are not changing vertex_added_in_last_run
    }
    else {
        printf("Cost = %d + %d\n", cost, possible_ways[0][2]);
        cost += possible_ways[0][2];
        //We are adding new vertex to the "visited"
        if (used_vertex_one == 1) {
            visited[visited_quantity] = checked_vertex_two;
            vertex_added_in_last_run = checked_vertex_two;
            visited_quantity++;

        }
        else {
            visited[visited_quantity] = checked_vertex_one;
            vertex_added_in_last_run = checked_vertex_one;
            visited_quantity++;
        }
    }
/*
    printf("Visited: ");
    for (i=0;i<visited_quantity;i++) {
        printf("%d ", visited[i]);
    }
    printf("\n");
*/
    //Removal of used edge
    i = 0;
    while (i < ways) {
        possible_ways[i][0] = possible_ways[i+1][0];
        possible_ways[i][1] = possible_ways[i+1][1];
        possible_ways[i][2] = possible_ways[i+1][2];
        i++;
    }
    ways--;
    used_vertex_one = 0;
    used_vertex_two = 0;
    checked_ways++;
}
    printf("\nTOTAL COST = %d\n", cost);

	return 0;
}
