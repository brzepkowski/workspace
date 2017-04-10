#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static int table[1000000][3];

//Partially from http://www.algorytm.org/algorytmy-sortowania/sortowanie-stogowe-heapsort/heap-2-c.html
//********************************************************

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
//********************************************************

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
        printf("n = %d\n", n);
        c = c- 48; // We are getting normal value from ASCII
        n = n*mult + c;
        //mult = mult*10;
    }
    //mult = 1;
    //Second while is to get value of m
    while ((c = fgetc(file)) != '\n') {
        printf("m = %d\n", m);
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
*/    //Now we will be sorting by the third element in each row in the "table"
    //****************HEAPSORT*********************
    int size = m-1;

    sort (size);
/*
    for (i=0;i<=size;i++) {
        printf("%d %d %d\n", table[i][0], table[i][1], table[i][2]);
    }
*/
    //Create new table, where we will have all already visited vertexes
        //If in one row is at least one vertex, which is not in this table, it means,
        //that there will be no cycle after adding this row
    int visited_vertexes[n][n];
    memset(visited_vertexes, 0, sizeof(visited_vertexes[0][0]) * n * n);
    int used_vertexes = 0;  //This is necessary to check, if vertexes, which we are going to add are already in "visited_vertexes"
    int cohesion_condition = 0; //This must be equal to n-1 if graph must be coherent [number of edges = vertexes - 1]
    int total_vertexes = n;

    //Take first row from the table and start adding values of edges
        //First row
    visited_vertexes[0][0] = table[0][0];
    visited_vertexes[0][1] = table[0][1];
    cost = table[0][2];

    used_vertexes = 2;
    cohesion_condition = 1; //We used one edge
    //n = n - 2; //We have to substract 2 from n, because this is our number of vertexes, and we alreade used by copying them
        //to the "visited_vertexes"

        //All rows left
    i = 1;  //i at this point is our iterator in "table"
    int local_vertex_counter = 0; //It will count vertexes, which are in "visited_vertexes" in every checking of new row
    int vertex_one_used = 0, vertex_two_used = 0; //We have to check, which vertex was already used [0 - not used / 1 - used]
    int vertex_found_somewhere_else = 0, cycle = 0, index_of_first_group, index_of_second_group;
    int first_free_index = 1;

    int checked_edges = 1;

    printf("n = %d, m = %d\n", n, m);
    while (cohesion_condition != n-1 && checked_edges != m) {

        //********************************************************************************
        printf("n = %d Row: %d %d %d\n", n, table[i][0], table[i][1], table[i][2]);

        //*********************************************************************************
        for (j = 0; j < n; j++) {
            for (k = 0; k < n; k++) {
               if (table[i][0] == visited_vertexes[j][k]) {   //Check first vertex
                    local_vertex_counter++;
                    vertex_one_used = 1;
                }
                if (table[i][1] == visited_vertexes[j][k]) {   //Check second vertex
                    local_vertex_counter++;
                    vertex_two_used = 1;
                }
            }
            //Nothing found
            if (vertex_found_somewhere_else == 0 && vertex_one_used == 0 && vertex_two_used == 0) {}
            //Cycle
            else if (vertex_found_somewhere_else == 0 && vertex_one_used == 1 && vertex_two_used == 1) {
                cycle = 1;
                goto end;
            }
            //Found first vertex
            else if (vertex_found_somewhere_else == 0 && (vertex_one_used == 1 || vertex_two_used == 1)) {
                vertex_found_somewhere_else = 1;
                index_of_first_group = j;
            }
            //Binding of graphs [two vertexes in different groups]
           /* else if (vertex_found_somewhere_else == 1 && (vertex_one_used == 1 || vertex_two_used == 1)) {
                index_of_second_group = j;
                goto end;
            }*/
            else if (vertex_found_somewhere_else == 1 && vertex_one_used == 1 && vertex_two_used == 1) {
                index_of_second_group = j;
                goto end;
            }

        }

        end:
        //****************************************************
      //  printf("[Values] somewhere_else: %d, vertex_one: %d, vertex_two: %d\n",vertex_found_somewhere_else, vertex_one_used, vertex_two_used);
        //****************************************************
        if (local_vertex_counter == 0) {
            //Create brand new group
            printf("Creation of new group ---> table[%d][0] = %d, table[%d][1] = %d || First_free_index = %d\n", i, table[i][0], i, table[i][1], first_free_index);
            visited_vertexes[first_free_index][0] = table[i][0];
            visited_vertexes[first_free_index][1] = table[i][1];
            first_free_index++;
            used_vertexes = used_vertexes + 2;
            //n = n - 2;
            cost += table[i][2];
            cohesion_condition++;
        }
        else if (local_vertex_counter == 1) {
            //Add new vertex to existing group
            printf("Adding new vertex to existing group, index_of_first_group = %d\n", index_of_first_group);
            vertex_one_used = 0;
            vertex_two_used = 0;
            j = 0;
            while (visited_vertexes[index_of_first_group][j] != 0) {
                if (visited_vertexes[index_of_first_group][j] == table[i][0]) {
                    vertex_one_used = 1;
                }
                else if (visited_vertexes[index_of_first_group][j] == table[i][1]) {
                    vertex_two_used = 1;
                }
                j++;
            }
            if (vertex_one_used == 0) { //Add first vertex to this group
                visited_vertexes[index_of_first_group][j] = table[i][0];
            }
            else if (vertex_two_used == 0) {    //Add second vertex to this group
                visited_vertexes[index_of_first_group][j] = table[i][1];
            }
            cost += table[i][2];
            used_vertexes++;
            cohesion_condition++;
        }
        else if (local_vertex_counter == 2) {
            if (cycle == 1) {
              //  printf("Cycle\n");
            }
            else {
                //Binding
               // printf("Binding. Index_of_first_group = %d, index_of_second_group = %d \n", index_of_first_group, index_of_second_group);
                j = 0; k = 0; //Indexes in each row
                    //Find first element which is equal to 0
                while (visited_vertexes[index_of_first_group][j] != 0) {
                    j++;
                }
                while (visited_vertexes[index_of_second_group][k] != 0) {
                    visited_vertexes[index_of_first_group][j] = visited_vertexes[index_of_second_group][k];
                    visited_vertexes[index_of_second_group][k] = 0;
                    j++;
                    k++;
                }
                cost+=table[i][2];
                cohesion_condition++;
            }
        }
        vertex_one_used = 0;
        vertex_two_used = 0;
        local_vertex_counter = 0;
        vertex_found_somewhere_else = 0;
        cycle = 0;
        i++;

        checked_edges++;
        printf("Cost = %d\n", cost);
/*
        for (int y = 0; y < first_free_index; y++) {
                for (int x = 0; x < used_vertexes; x++) {
                    printf("%d ", visited_vertexes[y][x]);
                }
                printf("\n");
        }
        printf("\n");*/
    } //End of big while

    //********************************************************************************
    /*    printf("n = %d FINAL: \nVisited_vertexes: ", n);
        for (int y = 0; y < first_free_index; y++) {
                for (int x = 0; x < used_vertexes; x++) {
                    printf("%d ", visited_vertexes[y][x]);
                }
                printf("\n");
        }*/
        //*********************************************************************************

        printf("TOTAL COST = %d\n", cost);
   /* if (cohesion_condition == total_vertexes - 1) {
        printf("Graph is coherent\n");
    }
    else {
        printf("Graph is not coherent\n");
    }*/

	return 0;
}
