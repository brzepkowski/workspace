#include <stdio.h>
#include <stdlib.h>
#include <string.h>


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
		if(n <= 0 || n > 1000){
			printf("You gave wrong n. Program will exit. \n");
		    return 0;
		}
		else{
			printf("n = %d\n", n);
		}

		int* coins = malloc(n * sizeof(int));
		int coin = 0;

		//Reading all coins from next row
		int j;
		for(j = 0; j < n; j++){
			fscanf(file, "%d ",&coin);
		    if(coin < 1 || coin > 1000){
		    	printf("Wrong data. Program will exit. \n");
		        exit(0);
		    }
		    else {
		    	coins[j] = coin;
		    }
		}

		//Drukowanie wszystkich pobranych monet
		 for (int k = 0; k < 3; k++) {
			 printf("%d ", coins[k]);
		 }
		 printf("\n");

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

		 for (int i = 0; i < n; i++) {
			 coin = coins[i];
			 for (int j = 0; j <= S-coin; j++) {
				 if (change[j] < 100000) {
					 if (change[j] + 1 < change[j + coin]) {
						 change[j + coin] = change[j] + 1;
					 }
				 }
			 }
		 }

		 printf("%d\n", change[S]);

        return 0;
}
