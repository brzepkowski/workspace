#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>
#include "main.h"


typedef struct symbols_number {
	char *string;
	int quantity;
} symbols_number;

void bubble_sort(symbols_number *tab[], int n) {
  symbols_number *pom;
  int i, j;

  for(i=0;i<n;i++)
    for(j=0;j<n-i-1;j++) //pętla wewnętrzna
    if(tab[j]->quantity>tab[j+1]->quantity)
    {
      //zamiana miejscami
      pom = tab[j];
      tab[j] = tab[j+1];
      tab[j+1] = pom;
    }
}


void optimize_symbols_table(symbols_list *symbols_list_root, char *file_name) {
	symbol *temp = symbols_list_root->data;
	int symbols_quantity = 0;
	while(temp!= NULL) {
		if (strcmp(temp->type, "ID") == 0) {
			symbols_quantity++;
		}
		temp = temp->next;
	}
	temp = symbols_list_root->data->next; // Przejscie do next, poniewaz pierwszy jest SYMBOLS_ROOT

	symbols_number *symbols_number_table[symbols_quantity];

	int i = 0;
	while(temp!= NULL) {
		if (strcmp(temp->type, "ID") == 0) {
			symbols_number_table[i] = malloc(sizeof(symbols_number));
			symbols_number_table[i]->string = temp->string;
			symbols_number_table[i]->quantity = 0;
			i++;
		}
		temp = temp->next;
	}



	FILE *read_file = fopen(file_name, "r");

	char oneword[100];
	char c;

	do {
	   c = fscanf(read_file,"%s",oneword); /* got one word from the file */
	   symbol *found_symbol = find_symbol(oneword);
	   if(found_symbol != NULL) {
		   if(strcmp(found_symbol->type, "ID") == 0) {
			   i = 0;
			   while(strcmp(symbols_number_table[i]->string, oneword) != 0) {
				   i++;
			   }
			   symbols_number_table[i]->quantity++;
		   }
	   }
	   //printf("%s\n",oneword);       /* display it on the monitor  */
	} while (c != EOF);              /* repeat until EOF           */

	fclose(read_file);

	bubble_sort(symbols_number_table, symbols_quantity);

	temp = symbols_list_root->data->next;

	unsigned long int addresses[symbols_quantity];
	for(i = 0; i < symbols_quantity; i++) {
		while(temp != NULL) {
			if(strcmp(temp->type, "ID") == 0) {
				break;
			}
			temp = temp->next;
		}
		addresses[i] = temp->address;

		temp = temp->next;
	}

	for(i = 0; i < symbols_quantity; i++) {
		symbol *found_symbol = find_symbol(symbols_number_table[i]->string);

		found_symbol->address = addresses[symbols_quantity - i - 1];
	}




}
