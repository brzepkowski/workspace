#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int table[1000000][3];

void heapify (int heap_size, int i) {
    int largest, temp[3] = {0, 0, 0};
    int l=2*i, r=(2*i)+1;
    if (l<=heap_size && table[l][2]>table[i][2])
        largest=l;
    else largest=i;
    if (r<=heap_size && table[r][2]>table[largest][2])
        largest=r;
    if (largest!=i) {
        temp[0]=table[largest][0];
        temp[1]=table[largest][1];
        temp[2]=table[largest][2];

        table[largest][0]=table[i][0];
        table[largest][1]=table[i][1];
        table[largest][2]=table[i][2];

        table[i][0]=temp[0];
        table[i][1]=temp[1];
        table[i][2]=temp[2];

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
        temp[0]=table[i][0];
        temp[1]=table[i][1];
        temp[2]=table[i][2];

        table[i][0]=table[0][0];
        table[i][1]=table[0][1];
        table[i][2]=table[0][2];

        table[0][0]=temp[0];
        table[0][1]=temp[1];
        table[0][2]=temp[2];

        size--;
        heapify(size,0);
    }
}

int main() {
    FILE *file;
    int cost = 0; // !!! TOTAL COST OF MINIMAL SPANNING TREE

    int n = 0, m = 0, mult = 1; //n - number of vertexes m- number of edges, mult - necessary value by which we will multiply
    int i = 0,j = 0, k = 0;    // i and j are indexes in "table", k is needed to find cycles in graph
    int buf = 0;    // buf - buffered number which will be seved in "table"
    char c;

    memset(table, 0, sizeof(table[0][0]) * 1000000 * 3);
    file = fopen("data.txt", "r");

    if ((file=fopen("data.txt", "r"))==NULL) {
                printf ("I can't open file 'data.txt' to read!\n");
                exit(1);
        }

    // First while is to get the value of n
    while ((c = fgetc(file)) != ' ') {
        c = c- 48; // We are getting normal value from ASCII
        n = n*mult + c;
        mult = mult*10;
    }
    mult = 1;
    //Second while is to get value of m
    while ((c = fgetc(file)) != '\n') {
        c = c- 48; // We are getting normal value from ASCII
        m = m*mult + c;
        mult = mult*10;
    }
    printf("n = %d, m = %d \n", n, m);
    c = fgetc(file);    //We are moving one step further than '\n' sign
    mult = 1;

    // Moving all number to the "table"
    while (c != EOF) {
        if (c == ' ') {
            table[i][j] = buf;

            buf = 0;
            mult = 1;
            j++;
        }
        else if (c == '\n') {
            table[i][j] = buf;

            buf = 0;
            mult = 1;
            i++;
            j = 0;
        }
        else {
            c = c - 48;
            buf = buf*mult + c;
            mult = mult*10;
        }
        c = fgetc(file);
    }

    for(i = 0; i < m; i++) {
        for(j = 0; j <= 2; j++) {
            printf("%d ", table[i][j]);
        }
        printf("\n");
    }

    printf("------------\n");
    //Now we will be sorting by the third element in each row in the "table"
    //****************HEAPSORT*********************
    int size = m-1;

    sort (size);

    for (i=0;i<=size;i++) {
        printf("%d %d %d\n", table[i][0], table[i][1], table[i][2]);
    }

    //Create new table, where we will have all already visited vertexes
        //If in one row is at least one vertex, which is not in this table, it means,
        //that there will be no cycle after adding this row
    int visited_vertexes[n];
    memset(visited_vertexes, 0, sizeof(visited_vertexes));
    int used_vertexes = 0;  //This is necessary to check, if vertexes, which we are going to add are already in "visited_vertexes"
    int cohesion_condition = 0; //This must be equal to n-1 if graph must be coherent [number of edges = vertexes - 1]
    int total_vertexes = n;

    //Take first row from the table and start adding values of edges
        //First row
    visited_vertexes[0] = table[0][0];
    visited_vertexes[1] = table[0][1];
    cost = table[0][2];

    used_vertexes = 2;
    cohesion_condition = 1; //We used one edge
    n = n - 2; //We have to substract 2 from n, because this is our number of vertexes, and we alreade used by copying them
        //to the "visited_vertexes"

/*
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    printf("Visited_vertexes = ");
    for(int k = 0; k < used_vertexes; k++) {
        printf("%d ", visited_vertexes[k]);
    }
    printf("cost = %d\n", cost);
    //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
*/

        //All rows left
    i = 1;  //i at this point is our iterator in "table"
    int local_vertex_counter = 0; //It will count vertexes, which are in "visited_vertexes" in every checking of new row
    int vertex_one_used = 0, vertex_two_used = 0; //We have to check, which vertex was already used [0 - not used / 1 - used]
    while (n > 0) {
        printf("i = %d, n = %d\n", i, n);
        //We are checking in these two fors, if vertexes, which we are going to use are already in "visited_vertexes"
            for (j = 0; j < used_vertexes; j++) {
                if (table[i][0] == visited_vertexes[j]) {   //Check first vertex
                    local_vertex_counter++;
                    vertex_one_used = 1;
                }
                if (table[i][1] == visited_vertexes[j]) {   //Check second vertex
                    local_vertex_counter++;
                    vertex_two_used = 1;
                }
            }
        
        if (local_vertex_counter == 2) {}
        else {
            if (local_vertex_counter == 1 && vertex_one_used == 1) {
                visited_vertexes[used_vertexes] = table[i][1];
                used_vertexes++;
                cohesion_condition++;
            }
            if (local_vertex_counter == 1 && vertex_two_used == 1) {
                visited_vertexes[used_vertexes] = table[i][0];
                used_vertexes++;
                cohesion_condition++;
            }
            if (local_vertex_counter == 0) {
                visited_vertexes[used_vertexes] = table[i][0];
                used_vertexes++;
                visited_vertexes[used_vertexes] = table[i][1];
                used_vertexes++;
                cohesion_condition++;
                n--;    //We are removing only one n, because in lower line we are removing it second time
            }
/*
            //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
            printf("Visited_vertexes = ");
            for(int k = 0; k < used_vertexes; k++) {
                printf("%d ", visited_vertexes[0][k]);
            }
            //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
*/
            cost = cost + table[i][2];
            printf("cost = %d + %d\n", cost-table[i][2], table[i][2]);
            n--;
        }
        local_vertex_counter = 0;
        vertex_one_used = 0;
        vertex_two_used = 0;
        i++;
    }
 /*   //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
            printf("----->Visited_vertexes = ");
            for(int k = 0; k < used_vertexes; k++) {
                printf("%d ", visited_vertexes[k]);
            }
            //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
   */ printf("Total cost = %d, cohesion_condition = %d\n", cost, cohesion_condition);
    if (cohesion_condition == total_vertexes - 1) {
        printf("Graph is coherent\n");
    }
    else {
        printf("Graph is not coherent\n");
    }

	return 0;
}
