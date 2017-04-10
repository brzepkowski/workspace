#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void quickSort( int[], int, int);
int partition( int[], int, int);

int main(int argc, char *argv[]) {
		FILE *file;

		int n = 0; //n - Number of different coins
		char c;

		if ((file=fopen(argv[1], "r"))==NULL) {
			printf ("I can't open file %s to read!\n", argv[1]);
			exit(1);
		}

		else {
			fscanf(file, "%d", &n);
		}
		    //Checking the conditions of exercise
		if(n <= 0 || n > 100){
			printf("You gave wrong n. Program will exit. \n");
		    return 0;
		}
		else{
			printf("n = %d\n", n);
		}

		//Implementation of two-dimensional arrays - "coins", "temp_coins" and "coins_clone"
		int** temp_coins = malloc(n * sizeof(int));
		for (int i = 0; i < n; i++) {
			temp_coins[i] = malloc(2 * sizeof(int));
		}
		int* coins_clone = malloc(n * sizeof(int));

		int** coins = malloc(n * sizeof(int));
		for (int i = 0; i < n; i++) {
			coins[i] = malloc(2 * sizeof(int));
		}

		int coin = 0;
		int occurrence = 0;

		//Reading all coins from next row
		int j;
		for(j = 0; j < n; j++){
			fscanf(file, "%d ",&coin);
			fscanf(file, "%d ",&occurrence);
		    if(coin < 1 || coin > 1000 || occurrence < 1 || occurrence > 10){
		    	printf("Wrong data. Program will exit. \n");
		        exit(0);
		    }
		    else {
		    	temp_coins[j][0] = coin;
		    	temp_coins[j][1] = occurrence;
		    }
		}

		//Moving values to the coins_clone
		for (int i = 0; i < n; i++) {
					coins_clone[i] = temp_coins[i][0];
		}

		//Printing unsorted array of temp_coins and coins_clone
		for (int i = 0; i < n; i++) {
			printf("%d %d\n", temp_coins[i][0], temp_coins[i][1]);
		}

		printf("Coins_clone\n");
		for (int i = 0; i < n; i++) {
			printf("%d \n", coins_clone[i]);
		}

		//Sorting this array
		quickSort(coins_clone, 0, n);
		int pointer = 0;

		for (int i = 0; i < n; i++) {
			int j = 0;
			while (temp_coins[j][0] != coins_clone[i]) {
				printf("temp_coins[%d][0] = %d, coins_clone[%d] = %d\n", j, temp_coins[j][0], i, coins_clone[i]);
				j++;
			}
			coins[pointer][0] = temp_coins[j][0];
			coins[pointer][1] = temp_coins[j][1];
			pointer++;
		}
		printf("TUUUUUUUUUU\n");

		//Printing sorted array of coins
		printf("------FINAL COINS---------\n");
		for (int i = 0; i < n; i++) {
					printf("%d %d\n", coins[i][0], coins[i][1]);
		}

		 //Getting S
		 int S = 0;
		 fscanf(file, "%d ",&S);
		 if(S < 0 || S > 10000){
			 printf("Wrong data. Program will exit. \n");
			 exit(0);
		 }

		 printf("S = %d\n", S);

		 //Creation of array of all values <= S
		 int* change = malloc((S+1) * sizeof(int));

		 for (int i = 0; i <= S; i++) {
			 if (i == 0) {
				 change[i] = 0;
			 }
			 else {
				 change[i] = 100000;
			 }
		 }


		 //The most important loops
		 for (int i = 0; i < n; i++) {
			 coin = coins[i][0];
			 if (coins[i][1] != 0) {
				 for (int j = 0; j < coins[i][1]; j++) {
					 for (int k = S - coin; k >=0; k--) {
						 if (change[k] < 100000) {
							 if (change[k] + 1 < change[k + coin]) {
								 change[k+coin] = change[k] + 1;
							 }
						 }
					 }
				 }
			 }
		 }




		 if (change[S] != 100000) {
			 printf("%d\n", change[S]);
		 }
		 else {
			 printf("0\n");
		 }

        return 0;
}


void quickSort( int a[], int l, int r)
{
   int j;

   if( l < r )
   {
   	// divide and conquer
        j = partition( a, l, r);
       quickSort( a, l, j-1);
       quickSort( a, j+1, r);
   }

}



int partition( int a[], int l, int r) {
   int pivot, i, j, t;
   pivot = a[l];
   i = l; j = r+1;

   while( 1)
   {
   	do ++i; while( a[i] <= pivot && i <= r );
   	do --j; while( a[j] > pivot );
   	if( i >= j ) break;
   	t = a[i]; a[i] = a[j]; a[j] = t;
   }
   t = a[l]; a[l] = a[j]; a[j] = t;
   return j;
}
