#include <iostream>
#include <unordered_map>
#include <algorithm>
#include <zconf.h>

using namespace std;

int dodaniaDoMapy = 0;

unordered_map<string, int> moves;

string atos(int int_array[], int size_of_array) {
    string returnstring = "";
    for (int temp = 0; temp < size_of_array - 1; temp++) {
        returnstring += to_string(int_array[temp]) + ", ";
    }
    returnstring += to_string(int_array[size_of_array-1]);
    return returnstring;
}

int check(int processorInCS, int n, int processors[], int buffer) {
    int previousProcessors[n];
    copy(processors, processors+n, previousProcessors);

    if (processorInCS == 0) {
        int registry = processors[processorInCS];
        processors[processorInCS] = (registry + 1) % (n + 1);
    } else {
        int prevRegistry = processors[processorInCS - 1];
        processors[processorInCS] = prevRegistry;
    }

    // Check if this combination has already been checked
    if (moves.find(atos(processors, n)) != moves.end()) {
        return moves[atos(processors, n)] + buffer + 1;
    }

    // First check how many processors will go to the critical section. If only one, we can return value
    int total = 0;
    if (processors[0] == processors[n-1]) {
        total++;
    }
    for (int i = 1; i < n; i++) {
        if (processors[i] != processors[i-1]) {
            total++;
        }
    }

    if (total > 1) {
        // Find max amount of moves left
        int max = 0;
        if (processors[0] == processors[n - 1]) {
            int tempProcessors[n];
            copy(processors, processors+n, tempProcessors);
            int movesLeft = check(0, n, tempProcessors, buffer);
            if (movesLeft > max)
                max = movesLeft;
        }
        for (int i = 1; i < n; i++) {
            if (processors[i] != processors[i - 1]) {
                int tempProcessors[n];
                copy(processors, processors+n, tempProcessors);
                int movesLeft = check(i, n, tempProcessors, buffer);
                if (movesLeft > max)
                    max = movesLeft;
            }
        }
        if (moves.find(atos(previousProcessors, n)) == moves.end() || moves[atos(previousProcessors, n)] < max) {
            moves[atos(previousProcessors, n)] = max;
            dodaniaDoMapy++;
        }
        return max + buffer + 1;
    } else {
        if (moves.find(atos(previousProcessors, n)) == moves.end()) {
            moves[atos(previousProcessors, n)] = 0;
            dodaniaDoMapy++;
        }
        return buffer + 1;
    }
}

void fill(int index, int n, int processors[]) {
    if (index == 0) {
        for (int i = 0; i < n + 1; i++) {
            processors[index] = i;
            //------ CRITICAL SECTION ---------
            if (moves.find(atos(processors, n)) == moves.end()) { // If such configuration hasn't been tested yet
                int previousProcessors[n];
                copy(processors, processors+n, previousProcessors);

                int total = 0;
                if (processors[0] == processors[n-1]) {
                    total++;
                }
                for (int i = 1; i < n; i++) {
                    if (processors[i] != processors[i-1]) {
                        total++;
                    }
                }

                if (total > 1) {
                    int max = 0;
                    if (processors[0] == processors[n - 1]) {
                        int tempProcessors[n];
                        copy(previousProcessors, previousProcessors+n, tempProcessors);
                        int movesLeft = check(0, n, tempProcessors, 0);
                        if (movesLeft > max)
                            max = movesLeft;
                    }
//                    #pragma omp parallel for
                    for (int i = 1; i < n; i++) {
                        if (processors[i] != processors[i - 1]) {
                            int tempProcessors[n];
                            copy(previousProcessors, previousProcessors+n, tempProcessors);
                            int movesLeft = check(i, n, tempProcessors, 0);
                            if (movesLeft > max)
                                max = movesLeft;
                        }
                    }
                    if (moves.find(atos(previousProcessors, n)) == moves.end() || moves[atos(previousProcessors, n)] < max) {
                        moves[atos(previousProcessors, n)] = max;
                        dodaniaDoMapy++;
                    }
                } else {
                    if (moves.find(atos(previousProcessors, n)) == moves.end()) {
                        moves[atos(previousProcessors, n)] = 0;
                        dodaniaDoMapy++;
                    }
                }
            }
        }
    }  else {
//        #pragma omp parallel for
        for (int i = 0; i < n + 1; i++) {
            fill(index - 1, n, processors);
            processors[index] = processors[index] + 1;
        }
        processors[index] = 0;
    }
}




int main(int argc, char** args) {
    clock_t begin = clock();

    int n = atoi(args[1]);
    int processors[n] = {0};

//    #pragma omp task
    fill(n - 1, n, processors);

    int max = 0;
    unordered_map<string, int>::iterator it;
    for ( it = moves.begin(); it != moves.end(); it++ ) {
        if (it->second > max)
            max = it->second;

        /*std::cout << it->first  // string (key)
                  << ": "
                  << it->second   // string's value
                  << std::endl ;*/
    }


    cout << "Max = " << max << endl;

    cout << "Całkowita liczba dodań do mapy = " << dodaniaDoMapy << endl;

    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    cout << time_spent << endl;

    return 0;
}