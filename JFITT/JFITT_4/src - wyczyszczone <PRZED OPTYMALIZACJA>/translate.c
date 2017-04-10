#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "main.h"

/* Jeśli prawda, to zwraca wartośc niezerową. Jeśli fałsz, to zwraca 0 */
int is_numeric (const char * s)
{
    if (s == NULL || *s == '\0' || isspace(*s))
      return 0;
    char * p;
    strtod (s, &p);
    return *p == '\0';
}


char *generate_new_address() {
	// Count the number of digits
	int temp = new_address_counter;
	int count = 0;
	while (temp > 0) {
	    count++;
	    temp /= 10;
	}

	char *string = calloc(count + 1, sizeof(char));
	char *number_buffer = malloc(count * sizeof(char) + 1);
	sprintf(number_buffer, "%d", new_address_counter);
	strcat(string, "t");
	strcat(string, number_buffer);
	new_address_counter++;

	return string;
}

char *generate_new_table_address() {
	// Count the number of digits
	int temp = new_table_address_counter;
	int count = 0;
	while (temp > 0) {
	    count++;
	    temp /= 10;
	}

	char *string = calloc(count + 1, sizeof(char));
	char *number_buffer = malloc(count * sizeof(char) + 1);
	sprintf(number_buffer, "%d", new_table_address_counter);
	strcat(string, "r");
	strcat(string, number_buffer);
	new_table_address_counter++;

	return string;
}

char *generate_new_label() {
	// Count the number of digits
	int temp = new_label_counter;
	int count = 0;
	while (temp > 0) {
	    count++;
	    temp /= 10;
	}

	char *string = calloc(count + 1, sizeof(char));
	char *number_buffer = malloc(count * sizeof(char) + 1);
	sprintf(number_buffer, "%d", new_label_counter);
	strcat(string, "L");
	strcat(string, number_buffer);
	new_label_counter++;

	return string;
}

/*********** INICJALIZACJA **********/
void initialize(char *symbol_name) {
	if(check_if_symbol_was_initialized(symbol_name) == 0) {
		initialized_symbols_list *temp = initialized_symbols_list_root;

		initialized_symbol *symbol_found = malloc(sizeof(initialized_symbol));
		symbol_found = NULL;
		initialized_symbol *temp_symbol = temp->data;
		while(temp != NULL) {
			while(temp_symbol != NULL) {
				if (strcmp(temp_symbol->string, symbol_name) == 0) {
					symbol_found = temp_symbol;
				}
				temp_symbol = temp_symbol->next;
			}
			temp = temp->next;
		}

		if(symbol_found != NULL) {
			symbol_found->initialized = true;
		}
	}
}

void add_initialized_symbol(char *symbol_name) {
	initialized_symbol *new_symbol = malloc(sizeof(initialized_symbol));
	new_symbol->string = symbol_name;
	new_symbol->initialized = false;

	initialized_symbols_list *temp_sublist = initialized_symbols_list_root;

	// Przejdź do ostatniej podlisty
	while(temp_sublist->next != NULL) {
		temp_sublist = temp_sublist->next;
	}

	struct initialized_symbol *temp_symbol = temp_sublist->data;

	while(temp_symbol->next != NULL) {
		temp_symbol = temp_symbol->next;
	}

	new_symbol->next = temp_symbol->next;
	temp_symbol->next = new_symbol;
}


void add_initialized_symbols_sublist() {
	initialized_symbols_list *new_list = malloc(sizeof(initialized_symbols_list));
	initialized_symbol *new_symbol =  malloc(sizeof(initialized_symbol));
	new_symbol->string = "ROOT_INIT_SYMBOL";//Inicjowanie nowej listy symboli symbolem "ROOT_INIT_SYMBOL"
	new_symbol->next = NULL;
	new_list->data = new_symbol;

	initialized_symbols_list *temp = initialized_symbols_list_root;

	if(initialized_symbols_list_root == NULL) {
		initialized_symbols_list_root = malloc(sizeof(initialized_symbols_list));
		initialized_symbols_list_root->next = NULL;
		initialized_symbols_list_root->data = new_symbol;
	}
	else {
		while (temp->next != NULL) {
			temp = temp->next;
		}
		new_list->next = temp->next;
		temp->next = new_list;
	}
}

/* remove_initialized_symbols_sublist - usuwa z listy zainicjowanych zmiennych
 * ostatnią podlistę */
void remove_initialized_symbols_sublist() {
	initialized_symbols_list *temp;
	initialized_symbols_list *previous;
	temp = initialized_symbols_list_root;
	while(temp->next != NULL) {
		previous = temp;
		temp = temp->next;
	}
	if(temp == initialized_symbols_list_root) initialized_symbols_list_root = NULL;
	else {
		previous->next = NULL;
	}
	if (temp != NULL) free(temp);
}


int check_if_symbol_was_initialized(char *symbol_name) {
	initialized_symbols_list *temp = initialized_symbols_list_root;

	initialized_symbol *symbol_found = malloc(sizeof(initialized_symbol));
	symbol_found = NULL;
	initialized_symbol *temp_symbol = temp->data;
	while(temp != NULL) {
		while(temp_symbol != NULL) {
			if (strcmp(temp_symbol->string, symbol_name) == 0) {
				symbol_found = temp_symbol;
			}
			temp_symbol = temp_symbol->next;
		}
		temp = temp->next;
	}

	if(symbol_found != NULL && symbol_found->initialized == true) return 1;
	else return 0;

}

void print_initialized_symbols() {
	initialized_symbols_list *initialized_symbols_sublist = initialized_symbols_list_root;

	while(initialized_symbols_sublist != NULL) {
		initialized_symbol *temp_symbol = initialized_symbols_sublist->data;
		while(temp_symbol != NULL) {
			if(strcmp(temp_symbol->string, "ROOT_INIT_SYMBOL") != 0) {
				printf("%*s %*d\n", 5, temp_symbol->string, 5, temp_symbol->initialized);
			}
			temp_symbol = temp_symbol->next;
		}
		initialized_symbols_sublist = initialized_symbols_sublist->next;
	}
}

/*************** TRANSLATE *****************/

int check_table(node *p) {
	if(is_numeric(p->children[0]->string) == 0) {
		//Sprawdzenie, czy lewy argument jest tablicą. Jesli tak, to zła konstrukcja
		if(strcmp(p->children[0]->string, "TABLE") == 0) {
			char *string = calloc(strlen("Niewłaściwe użycie zmiennej: ") + strlen(p->children[0]->children[0]->string), sizeof(char));
			strcat(string, "Niewłaściwe użycie zmiennej: ");
			strcat(string, p->children[0]->children[0]->string);
			add_error(create_error(string, p->children[0]->first_line, p->children[0]->first_column,
					p->children[0]->last_line, p->children[0]->last_column));
			return 0;
		}
		else {
			if(search_for_symbol(p->children[0]->string) == 0) {
				char *string = calloc(strlen("Nie zadeklarowano zmiennej: ") + strlen(p->children[0]->string), sizeof(char));
				strcat(string, "Nie zadeklarowano zmiennej: ");
				strcat(string, p->children[0]->string);
				add_error(create_error(string, p->children[0]->first_line, p->children[0]->first_column,
						p->children[0]->last_line, p->children[0]->last_column));
				return 0;
			}
			if((search_for_symbol(p->children[0]->string) == 1) &&
					(strcmp(find_symbol(p->children[0]->string)->type, "ID") == 0 ||
							strcmp(find_symbol(p->children[0]->string)->type, "ITERATOR") == 0	)) {
					char *string = calloc(strlen("Błędne użycie zmiennej: '") + strlen(p->children[0]->string) + strlen("' jako tablicy"), sizeof(char));
					strcat(string, "Próba użycia zmiennej: '");
					strcat(string, p->children[0]->string);
					strcat(string, "' jako tablicy");
					add_error(create_error(string, p->children[0]->first_line, p->children[0]->first_column,
					p->children[0]->last_line, p->children[0]->last_column));
					return 0;
			}
		}
	}
	if(is_numeric(p->children[1]->string) == 0) {
		if(search_for_symbol(p->children[1]->string) == 0) {
			char *string = calloc(strlen("Nie zadeklarowano zmiennej: ") + strlen(p->children[1]->string), sizeof(char));
			strcat(string, "Nie zadeklarowano zmiennej: ");
			strcat(string, p->children[1]->string);
			add_error(create_error(string, p->children[1]->first_line, p->children[1]->first_column,
					p->children[1]->last_line, p->children[1]->last_column));
			return 0;
		}
	}
	return 1;
}

void interpret(node *p) {

	if (strcmp(p->string, ":=") == 0) {
		if(is_numeric(p->children[0]->string) == 0) {
			//Sprawdzenie, czy lewy argument jest tablicą
			if(strcmp(p->children[0]->string, "TABLE") == 0) {
				check_table(p->children[0]);
				//Dodanie czwórki przypisania wartości do komórki w tablicy
				char *symbol_name = generate_new_table_address();
				symbol *new_symbol = create_symbol(symbol_name, "ADDRESS", address_counter, 1,
						p->first_line, p->first_column, p->last_line, p->last_column);
				address_counter++;
				add_symbol(new_symbol);
				p->children[0]->string = symbol_name;

				add_four("TAB", p->children[0]->children[0]->string, "", p->children[0]->children[1]->string, symbol_name);
			}
			else {
				if(search_for_symbol(p->children[0]->string) == 0) {
					char *string = calloc(strlen("Nie zadeklarowano zmiennej: ") + strlen(p->children[0]->string), sizeof(char));
					strcat(string, "Nie zadeklarowano zmiennej: ");
					strcat(string, p->children[0]->string);
					add_error(create_error(string, p->children[0]->first_line, p->children[0]->first_column,
							p->children[0]->last_line, p->children[0]->last_column));
				}
				else {
				// Sprawdzenie, czy nie bedziemy modyfikować iteratora petli
				symbol *found_symbol = find_symbol(p->children[0]->string);
				if(found_symbol != NULL && strcmp(found_symbol->type, "ITERATOR") == 0) {
					char *string = calloc(strlen("Próba zmiany iteratora '") + strlen(p->children[0]->string) +
							strlen("' wewnątrz petli"), sizeof(char));
					strcat(string, "Próba zmiany iteratora '");
					strcat(string, p->children[0]->string);
					strcat(string, "' wewnątrz petli");
					add_error(create_error(string, p->children[0]->first_line, p->children[0]->first_column,
							p->children[0]->last_line, p->children[0]->last_column));
				}
					// Sprawdzenie, czy zmienna tablicowa zostla poprawnie użyta
					if(strcmp(find_symbol(p->children[0]->string)->type, "TABLE") == 0) {
						char *string = calloc(strlen("Złe użycie zmiennej tablicowej: ") + strlen(p->children[0]->string), sizeof(char));
						strcat(string, "Złe użycie zmiennej tablicowej: ");
						strcat(string, p->children[0]->string);
						add_error(create_error(string, p->children[0]->first_line, p->children[0]->first_column,
								p->children[0]->last_line, p->children[0]->last_column));
					}
				}
			}
		}
		if(is_numeric(p->children[1]->string) == 0) {
			// Sprawdzenie, czy prawy argument przypisania jest tablicą
			if(strcmp(p->children[1]->string, "TABLE") == 0) {
				check_table(p->children[1]);
				//Dodanie czwórki przypisania wartości do komórki w tablicy
				char *symbol_name = generate_new_table_address();
				symbol *new_symbol = create_symbol(symbol_name, "ADDRESS", address_counter, 1,
						p->first_line, p->first_column, p->last_line, p->last_column);
				address_counter++;
				add_symbol(new_symbol);
				p->children[1]->string = symbol_name;

				add_four("TAB", p->children[1]->children[0]->string, "", p->children[1]->children[1]->string, symbol_name);
			}
			else {
				if(search_for_symbol(p->children[1]->string) == 0) {
					char *string = calloc(strlen("Nie zadeklarowano zmiennej: ") + strlen(p->children[1]->string), sizeof(char));
					strcat(string, "Nie zadeklarowano zmiennej: ");
					strcat(string, p->children[1]->string);
					add_error(create_error(string, p->children[1]->first_line, p->children[1]->first_column,
							p->children[1]->last_line, p->children[1]->last_column));
				}
				else {
					// Sprawdzenie, czy zmienna tablicowa zostla poprawnie użyta
					if(strcmp(find_symbol(p->children[1]->string)->type, "TABLE") == 0) {
						char *string = calloc(strlen("Złe użycie zmiennej tablicowej: ") + strlen(p->children[1]->string), sizeof(char));
						strcat(string, "Złe użycie zmiennej tablicowej: ");
						strcat(string, p->children[1]->string);
						add_error(create_error(string, p->children[1]->first_line, p->children[1]->first_column,
								p->children[1]->last_line, p->children[1]->last_column));
					}
					// Sprawdzenie, czy prawy argument zostal zainicjowany
					else if(strcmp(find_symbol(p->children[1]->string)->type, "ID") == 0 && check_if_symbol_was_initialized(p->children[1]->string) == 0){
						char *string = "Uzycie niezainicjowanej zmiennej";
						add_error(create_error(string, p->children[1]->first_line, p->children[1]->first_column,
								p->children[1]->last_line, p->children[1]->last_column));
					}
				}
			}
		}

		initialize(p->children[0]->string);
		add_four(":=", p->children[0]->string, "", p->children[1]->string, p->children[0]->string);
	}

	if (strcmp(p->string, "GET") == 0) {
		if(is_numeric(p->children[0]->string) == 0) {
			//Sprawdzenie, czy argument jest tablicą
			if(strcmp(p->children[0]->string, "TABLE") == 0) {
				check_table(p->children[0]);
				//Dodanie czwórki przypisania wartości do komórki w tablicy
				char *symbol_name = generate_new_table_address();
				symbol *new_symbol = create_symbol(symbol_name, "ADDRESS", address_counter, 1,
						p->first_line, p->first_column, p->last_line, p->last_column);
				address_counter++;
				add_symbol(new_symbol);
				p->children[0]->string = symbol_name;

				add_four("TAB", p->children[0]->children[0]->string, "", p->children[0]->children[1]->string, symbol_name);
			}
			else {
				if(search_for_symbol(p->children[0]->string) == 0) {
					char *string = calloc(strlen("Nie zadeklarowano zmiennej: ") + strlen(p->children[0]->string), sizeof(char));
					strcat(string, "Nie zadeklarowano zmiennej: ");
					strcat(string, p->children[0]->string);
					add_error(create_error(string, p->children[0]->first_line, p->children[0]->first_column,
							p->children[0]->last_line, p->children[0]->last_column));
				}
				else {
					// Sprawdzenie, czy zmienna tablicowa zostla poprawnie użyta
					if(strcmp(find_symbol(p->children[0]->string)->type, "TABLE") == 0) {
						char *string = calloc(strlen("Złe użycie zmiennej tablicowej: ") + strlen(p->children[0]->string), sizeof(char));
						strcat(string, "Złe użycie zmiennej tablicowej: ");
						strcat(string, p->children[0]->string);
						add_error(create_error(string, p->children[0]->first_line, p->children[0]->first_column,
								p->children[0]->last_line, p->children[0]->last_column));
					}
				}
			}
		}

		/*if(strcmp(find_symbol(p->children[0]->string)->type, "ID") == 0)*/
			initialize(p->children[0]->string);
		add_four("GET", p->children[0]->string, "", "", p->children[0]->string);
	}

	if (strcmp(p->string, "PUT") == 0) {
		if(is_numeric(p->children[0]->string) == 0) {
			//Sprawdzenie, czy argument jest tablicą
			if(strcmp(p->children[0]->string, "TABLE") == 0) {
				check_table(p->children[0]);
				//Dodanie czwórki przypisania wartości do komórki w tablicy
				char *symbol_name = generate_new_table_address();
				symbol *new_symbol = create_symbol(symbol_name, "ADDRESS", address_counter, 1,
						p->first_line, p->first_column, p->last_line, p->last_column);
				address_counter++;
				add_symbol(new_symbol);
				p->children[0]->string = symbol_name;

				add_four("TAB", p->children[0]->children[0]->string, "", p->children[0]->children[1]->string, symbol_name);
			}
			else {
				if(search_for_symbol(p->children[0]->string) == 0) {
					char *string = calloc(strlen("Nie zadeklarowano zmiennej: ") + strlen(p->children[0]->string), sizeof(char));
					strcat(string, "Nie zadeklarowano zmiennej: ");
					strcat(string, p->children[0]->string);
					add_error(create_error(string, p->children[0]->first_line, p->children[0]->first_column,
							p->children[0]->last_line, p->children[0]->last_column));
				}
				else {
					// Sprawdzenie, czy zmienna tablicowa zostala poprawnie użyta
					if(strcmp(find_symbol(p->children[0]->string)->type, "TABLE") == 0) {
						char *string = calloc(strlen("Złe użycie zmiennej tablicowej: ") + strlen(p->children[0]->string), sizeof(char));
						strcat(string, "Złe użycie zmiennej tablicowej: ");
						strcat(string, p->children[0]->string);
						add_error(create_error(string, p->children[0]->first_line, p->children[0]->first_column,
								p->children[0]->last_line, p->children[0]->last_column));
					}
					// Sprawdzenie, czy prawy argument zostal zainicjowany
					else if(strcmp(find_symbol(p->children[0]->string)->type, "ID") == 0 && check_if_symbol_was_initialized(p->children[0]->string) == 0){
						char *string = "Uzycie niezainicjowanej zmiennej";
						add_error(create_error(string, p->children[0]->first_line, p->children[0]->first_column,
								p->children[0]->last_line, p->children[0]->last_column));
					}
				}
			}
		}

		add_four("PUT", p->children[0]->string, "", "", p->children[0]->string);
	}

	if (strcmp(p->string, "+") == 0) {
		// Sprawdzenie, czy symbole, które nie są liczbami zostały wczesniej zadeklarowane
		if(is_numeric(p->children[0]->string) == 0) {
			//Sprawdzenie, czy lewy argument jest tablicą
			if(strcmp(p->children[0]->string, "TABLE") == 0) {
				check_table(p->children[0]);
				//Dodanie czwórki przypisania wartości do komórki w tablicy
				char *symbol_name = generate_new_table_address();
				symbol *new_symbol = create_symbol(symbol_name, "ADDRESS", address_counter, 1,
						p->first_line, p->first_column, p->last_line, p->last_column);
				address_counter++;
				add_symbol(new_symbol);
				p->children[0]->string = symbol_name;

				add_four("TAB", p->children[0]->children[0]->string, "", p->children[0]->children[1]->string, symbol_name);
			}
			else {
				if(search_for_symbol(p->children[0]->string) == 0) {
					char *string = calloc(strlen("Nie zadeklarowano zmiennej: ") + strlen(p->children[0]->string), sizeof(char));
					strcat(string, "Nie zadeklarowano zmiennej: ");
					strcat(string, p->children[0]->string);
					add_error(create_error(string, p->children[0]->first_line, p->children[0]->first_column,
							p->children[0]->last_line, p->children[0]->last_column));
				}
				else {
					// Sprawdzenie, czy zmienna tablicowa zostla poprawnie użyta
					if(strcmp(find_symbol(p->children[0]->string)->type, "TABLE") == 0) {
						char *string = calloc(strlen("Złe użycie zmiennej tablicowej: ") + strlen(p->children[0]->string), sizeof(char));
						strcat(string, "Złe użycie zmiennej tablicowej: ");
						strcat(string, p->children[0]->string);
						add_error(create_error(string, p->children[0]->first_line, p->children[0]->first_column,
								p->children[0]->last_line, p->children[0]->last_column));
					}
					// Sprawdzenie, czy lewy argument zostal zainicjowany
					else if(strcmp(find_symbol(p->children[0]->string)->type, "ID") == 0 && check_if_symbol_was_initialized(p->children[0]->string) == 0){
						char *string = "Uzycie niezainicjowanej zmiennej";
						add_error(create_error(string, p->children[0]->first_line, p->children[0]->first_column,
								p->children[0]->last_line, p->children[0]->last_column));
					}
				}
			}
		}
		if(is_numeric(p->children[1]->string) == 0) {
			// Sprawdzenie, czy prawy argument jest tablicą
			if(strcmp(p->children[1]->string, "TABLE") == 0) {
				check_table(p->children[1]);
				//Dodanie czwórki przypisania wartości do komórki w tablicy
				char *symbol_name = generate_new_table_address();
				symbol *new_symbol = create_symbol(symbol_name, "ADDRESS", address_counter, 1,
						p->first_line, p->first_column, p->last_line, p->last_column);
				address_counter++;
				add_symbol(new_symbol);
				p->children[1]->string = symbol_name;

				add_four("TAB", p->children[1]->children[0]->string, "", p->children[1]->children[1]->string, symbol_name);
			}
			else {
				if(search_for_symbol(p->children[1]->string) == 0) {
					char *string = calloc(strlen("Nie zadeklarowano zmiennej: ") + strlen(p->children[1]->string), sizeof(char));
					strcat(string, "Nie zadeklarowano zmiennej: ");
					strcat(string, p->children[1]->string);
					add_error(create_error(string, p->children[1]->first_line, p->children[1]->first_column,
							p->children[1]->last_line, p->children[1]->last_column));
				}
				else {
					// Sprawdzenie, czy zmienna tablicowa zostla poprawnie użyta
					if(strcmp(find_symbol(p->children[1]->string)->type, "TABLE") == 0) {
						char *string = calloc(strlen("Złe użycie zmiennej tablicowej: ") + strlen(p->children[1]->string), sizeof(char));
						strcat(string, "Złe użycie zmiennej tablicowej: ");
						strcat(string, p->children[1]->string);
						add_error(create_error(string, p->children[1]->first_line, p->children[1]->first_column,
								p->children[1]->last_line, p->children[1]->last_column));
					}
					// Sprawdzenie, czy prawy argument zostal zainicjowany
					else if(strcmp(find_symbol(p->children[1]->string)->type, "ID") == 0 && check_if_symbol_was_initialized(p->children[1]->string) == 0){
						char *string = "Uzycie niezainicjowanej zmiennej";
						add_error(create_error(string, p->children[1]->first_line, p->children[1]->first_column,
								p->children[1]->last_line, p->children[1]->last_column));
					}
				}
			}
		}


		char *symbol_name = generate_new_address();
		symbol *new_symbol = create_symbol(symbol_name, "-", address_counter, 1,
				p->first_line, p->first_column, p->last_line, p->last_column);
		address_counter++;
		add_symbol(new_symbol);
		add_four("+", p->children[0]->string, "", p->children[1]->string, symbol_name);

		// Bez dodania poniższego w tablicy czwórek pojawiałyby się np. takie wiersze ":= a - \ a"
		p->string = symbol_name;
	}

	if (strcmp(p->string, "-") == 0) {
		// Sprawdzenie, czy symbole, które nie są liczbami zostały wczesniej zadeklarowane
		if(is_numeric(p->children[0]->string) == 0) {
			//Sprawdzenie, czy lewy argument jest tablicą
			if(strcmp(p->children[0]->string, "TABLE") == 0) {
				check_table(p->children[0]);
				//Dodanie czwórki przypisania wartości do komórki w tablicy
				char *symbol_name = generate_new_table_address();
				symbol *new_symbol = create_symbol(symbol_name, "ADDRESS", address_counter, 1,
						p->first_line, p->first_column, p->last_line, p->last_column);
				address_counter++;
				add_symbol(new_symbol);
				p->children[0]->string = symbol_name;

				add_four("TAB", p->children[0]->children[0]->string, "", p->children[0]->children[1]->string, symbol_name);
			}
			else {
				if(search_for_symbol(p->children[0]->string) == 0) {
					char *string = calloc(strlen("Nie zadeklarowano zmiennej: ") + strlen(p->children[0]->string), sizeof(char));
					strcat(string, "Nie zadeklarowano zmiennej: ");
					strcat(string, p->children[0]->string);
					add_error(create_error(string, p->children[0]->first_line, p->children[0]->first_column,
							p->children[0]->last_line, p->children[0]->last_column));
				}
				else {
					// Sprawdzenie, czy zmienna tablicowa zostla poprawnie użyta
					if(strcmp(find_symbol(p->children[0]->string)->type, "TABLE") == 0) {
						char *string = calloc(strlen("Złe użycie zmiennej tablicowej: ") + strlen(p->children[0]->string), sizeof(char));
						strcat(string, "Złe użycie zmiennej tablicowej: ");
						strcat(string, p->children[0]->string);
						add_error(create_error(string, p->children[0]->first_line, p->children[0]->first_column,
								p->children[0]->last_line, p->children[0]->last_column));
					}
					// Sprawdzenie, czy lewy argument zostal zainicjowany
					else if(strcmp(find_symbol(p->children[0]->string)->type, "ID") == 0 && check_if_symbol_was_initialized(p->children[0]->string) == 0){
						char *string = "Uzycie niezainicjowanej zmiennej";
						add_error(create_error(string, p->children[0]->first_line, p->children[0]->first_column,
								p->children[0]->last_line, p->children[0]->last_column));
					}
				}
			}
		}
		if(is_numeric(p->children[1]->string) == 0) {
			// Sprawdzenie, czy prawy argument jest tablicą
			if(strcmp(p->children[1]->string, "TABLE") == 0) {
				check_table(p->children[1]);
				//Dodanie czwórki przypisania wartości do komórki w tablicy
				char *symbol_name = generate_new_table_address();
				symbol *new_symbol = create_symbol(symbol_name, "ADDRESS", address_counter, 1,
						p->first_line, p->first_column, p->last_line, p->last_column);
				address_counter++;
				add_symbol(new_symbol);
				p->children[1]->string = symbol_name;

				add_four("TAB", p->children[1]->children[0]->string, "", p->children[1]->children[1]->string, symbol_name);
			}
			else {
				if(search_for_symbol(p->children[1]->string) == 0) {
					char *string = calloc(strlen("Nie zadeklarowano zmiennej: ") + strlen(p->children[1]->string), sizeof(char));
					strcat(string, "Nie zadeklarowano zmiennej: ");
					strcat(string, p->children[1]->string);
					add_error(create_error(string, p->children[1]->first_line, p->children[1]->first_column,
							p->children[1]->last_line, p->children[1]->last_column));
				}
				else {
					// Sprawdzenie, czy zmienna tablicowa zostla poprawnie użyta
					if(strcmp(find_symbol(p->children[1]->string)->type, "TABLE") == 0) {
						char *string = calloc(strlen("Złe użycie zmiennej tablicowej: ") + strlen(p->children[1]->string), sizeof(char));
						strcat(string, "Złe użycie zmiennej tablicowej: ");
						strcat(string, p->children[1]->string);
						add_error(create_error(string, p->children[1]->first_line, p->children[1]->first_column,
								p->children[1]->last_line, p->children[1]->last_column));
					}
					// Sprawdzenie, czy prawy argument zostal zainicjowany
					else if(strcmp(find_symbol(p->children[1]->string)->type, "ID") == 0 && check_if_symbol_was_initialized(p->children[1]->string) == 0){
						char *string = "Uzycie niezainicjowanej zmiennej";
						add_error(create_error(string, p->children[1]->first_line, p->children[1]->first_column,
								p->children[1]->last_line, p->children[1]->last_column));
					}
				}
			}
		}


		char *symbol_name = generate_new_address();
		symbol *new_symbol = create_symbol(symbol_name, "-", address_counter, 1,
				p->first_line, p->first_column, p->last_line, p->last_column);
		address_counter++;
		add_symbol(new_symbol);
		add_four("-", p->children[0]->string, "", p->children[1]->string, symbol_name);

		p->string = symbol_name;
	}

	if (strcmp(p->string, "*") == 0) {
		// Sprawdzenie, czy symbole, które nie są liczbami zostały wczesniej zadeklarowane
		if(is_numeric(p->children[0]->string) == 0) {
			//Sprawdzenie, czy lewy argument jest tablicą
			if(strcmp(p->children[0]->string, "TABLE") == 0) {
				check_table(p->children[0]);
				//Dodanie czwórki przypisania wartości do komórki w tablicy
				char *symbol_name = generate_new_table_address();
				symbol *new_symbol = create_symbol(symbol_name, "ADDRESS", address_counter, 1,
						p->first_line, p->first_column, p->last_line, p->last_column);
				address_counter++;
				add_symbol(new_symbol);
				p->children[0]->string = symbol_name;

				add_four("TAB", p->children[0]->children[0]->string, "", p->children[0]->children[1]->string, symbol_name);
			}
			else {
				if(search_for_symbol(p->children[0]->string) == 0) {
					char *string = calloc(strlen("Nie zadeklarowano zmiennej: ") + strlen(p->children[0]->string), sizeof(char));
					strcat(string, "Nie zadeklarowano zmiennej: ");
					strcat(string, p->children[0]->string);
					add_error(create_error(string, p->children[0]->first_line, p->children[0]->first_column,
							p->children[0]->last_line, p->children[0]->last_column));
				}
				else {
					// Sprawdzenie, czy zmienna tablicowa zostla poprawnie użyta
					if(strcmp(find_symbol(p->children[0]->string)->type, "TABLE") == 0) {
						char *string = calloc(strlen("Złe użycie zmiennej tablicowej: ") + strlen(p->children[0]->string), sizeof(char));
						strcat(string, "Złe użycie zmiennej tablicowej: ");
						strcat(string, p->children[0]->string);
						add_error(create_error(string, p->children[0]->first_line, p->children[0]->first_column,
								p->children[0]->last_line, p->children[0]->last_column));
					}
					// Sprawdzenie, czy lewy argument zostal zainicjowany
					else if(strcmp(find_symbol(p->children[0]->string)->type, "ID") == 0 && check_if_symbol_was_initialized(p->children[0]->string) == 0){
						char *string = "Uzycie niezainicjowanej zmiennej";
						add_error(create_error(string, p->children[0]->first_line, p->children[0]->first_column,
								p->children[0]->last_line, p->children[0]->last_column));
					}
				}
			}
		}
		if(is_numeric(p->children[1]->string) == 0) {
			// Sprawdzenie, czy prawy argument jest tablicą
			if(strcmp(p->children[1]->string, "TABLE") == 0) {
				check_table(p->children[1]);
				//Dodanie czwórki przypisania wartości do komórki w tablicy
				char *symbol_name = generate_new_table_address();
				symbol *new_symbol = create_symbol(symbol_name, "ADDRESS", address_counter, 1,
						p->first_line, p->first_column, p->last_line, p->last_column);
				address_counter++;
				add_symbol(new_symbol);
				p->children[1]->string = symbol_name;

				add_four("TAB", p->children[1]->children[0]->string, "", p->children[1]->children[1]->string, symbol_name);
			}
			else {
				if(search_for_symbol(p->children[1]->string) == 0) {
					char *string = calloc(strlen("Nie zadeklarowano zmiennej: ") + strlen(p->children[1]->string), sizeof(char));
					strcat(string, "Nie zadeklarowano zmiennej: ");
					strcat(string, p->children[1]->string);
					add_error(create_error(string, p->children[1]->first_line, p->children[1]->first_column,
							p->children[1]->last_line, p->children[1]->last_column));
				}
				else {
					// Sprawdzenie, czy zmienna tablicowa zostla poprawnie użyta
					if(strcmp(find_symbol(p->children[1]->string)->type, "TABLE") == 0) {
						char *string = calloc(strlen("Złe użycie zmiennej tablicowej: ") + strlen(p->children[1]->string), sizeof(char));
						strcat(string, "Złe użycie zmiennej tablicowej: ");
						strcat(string, p->children[1]->string);
						add_error(create_error(string, p->children[1]->first_line, p->children[1]->first_column,
								p->children[1]->last_line, p->children[1]->last_column));
					}
					// Sprawdzenie, czy prawy argument zostal zainicjowany
					else if(strcmp(find_symbol(p->children[1]->string)->type, "ID") == 0 && check_if_symbol_was_initialized(p->children[1]->string) == 0){
						char *string = "Uzycie niezainicjowanej zmiennej";
						add_error(create_error(string, p->children[1]->first_line, p->children[1]->first_column,
								p->children[1]->last_line, p->children[1]->last_column));
					}
				}
			}
		}

		char *symbol_name = generate_new_address();
		symbol *new_symbol = create_symbol(symbol_name, "-", address_counter, 1,
				p->first_line, p->first_column, p->last_line, p->last_column);
		address_counter++;
		add_symbol(new_symbol);
		add_four("*", p->children[0]->string, "", p->children[1]->string, symbol_name);

		p->string = symbol_name;
	}

	if (strcmp(p->string, "/") == 0) {
		// Sprawdzenie, czy symbole, które nie są liczbami zostały wczesniej zadeklarowane
		if(is_numeric(p->children[0]->string) == 0) {
			//Sprawdzenie, czy lewy argument jest tablicą
			if(strcmp(p->children[0]->string, "TABLE") == 0) {
				check_table(p->children[0]);
				//Dodanie czwórki przypisania wartości do komórki w tablicy
				char *symbol_name = generate_new_table_address();
				symbol *new_symbol = create_symbol(symbol_name, "ADDRESS", address_counter, 1,
						p->first_line, p->first_column, p->last_line, p->last_column);
				address_counter++;
				add_symbol(new_symbol);
				p->children[0]->string = symbol_name;

				add_four("TAB", p->children[0]->children[0]->string, "", p->children[0]->children[1]->string, symbol_name);
			}
			else {
				if(search_for_symbol(p->children[0]->string) == 0) {
					char *string = calloc(strlen("Nie zadeklarowano zmiennej: ") + strlen(p->children[0]->string), sizeof(char));
					strcat(string, "Nie zadeklarowano zmiennej: ");
					strcat(string, p->children[0]->string);
					add_error(create_error(string, p->children[0]->first_line, p->children[0]->first_column,
							p->children[0]->last_line, p->children[0]->last_column));
				}
				else {
					// Sprawdzenie, czy zmienna tablicowa zostla poprawnie użyta
					if(strcmp(find_symbol(p->children[0]->string)->type, "TABLE") == 0) {
						char *string = calloc(strlen("Złe użycie zmiennej tablicowej: ") + strlen(p->children[0]->string), sizeof(char));
						strcat(string, "Złe użycie zmiennej tablicowej: ");
						strcat(string, p->children[0]->string);
						add_error(create_error(string, p->children[0]->first_line, p->children[0]->first_column,
								p->children[0]->last_line, p->children[0]->last_column));
					}
					// Sprawdzenie, czy lewy argument zostal zainicjowany
					else if(strcmp(find_symbol(p->children[0]->string)->type, "ID") == 0 && check_if_symbol_was_initialized(p->children[0]->string) == 0){
						char *string = "Uzycie niezainicjowanej zmiennej";
						add_error(create_error(string, p->children[0]->first_line, p->children[0]->first_column,
								p->children[0]->last_line, p->children[0]->last_column));
					}
				}
			}
		}
		if(is_numeric(p->children[1]->string) == 0) {
			// Sprawdzenie, czy prawy argument jest tablicą
			if(strcmp(p->children[1]->string, "TABLE") == 0) {
				check_table(p->children[1]);
				//Dodanie czwórki przypisania wartości do komórki w tablicy
				char *symbol_name = generate_new_table_address();
				symbol *new_symbol = create_symbol(symbol_name, "ADDRESS", address_counter, 1,
						p->first_line, p->first_column, p->last_line, p->last_column);
				address_counter++;
				add_symbol(new_symbol);
				p->children[1]->string = symbol_name;

				add_four("TAB", p->children[1]->children[0]->string, "", p->children[1]->children[1]->string, symbol_name);
			}
			else {
				if(search_for_symbol(p->children[1]->string) == 0) {
					char *string = calloc(strlen("Nie zadeklarowano zmiennej: ") + strlen(p->children[1]->string), sizeof(char));
					strcat(string, "Nie zadeklarowano zmiennej: ");
					strcat(string, p->children[1]->string);
					add_error(create_error(string, p->children[1]->first_line, p->children[1]->first_column,
							p->children[1]->last_line, p->children[1]->last_column));
				}
				else {
					// Sprawdzenie, czy zmienna tablicowa zostla poprawnie użyta
					if(strcmp(find_symbol(p->children[1]->string)->type, "TABLE") == 0) {
						char *string = calloc(strlen("Złe użycie zmiennej tablicowej: ") + strlen(p->children[1]->string), sizeof(char));
						strcat(string, "Złe użycie zmiennej tablicowej: ");
						strcat(string, p->children[1]->string);
						add_error(create_error(string, p->children[1]->first_line, p->children[1]->first_column,
								p->children[1]->last_line, p->children[1]->last_column));
					}
					// Sprawdzenie, czy prawy argument zostal zainicjowany
					else if(strcmp(find_symbol(p->children[1]->string)->type, "ID") == 0 && check_if_symbol_was_initialized(p->children[1]->string) == 0){
						char *string = "Uzycie niezainicjowanej zmiennej";
						add_error(create_error(string, p->children[1]->first_line, p->children[1]->first_column,
								p->children[1]->last_line, p->children[1]->last_column));
					}
				}
			}
		}


		char *symbol_name = generate_new_address();
		symbol *new_symbol = create_symbol(symbol_name, "-", address_counter, 1,
				p->first_line, p->first_column, p->last_line, p->last_column);
		address_counter++;
		add_symbol(new_symbol);
		add_four("/", p->children[0]->string, "", p->children[1]->string, symbol_name);

		p->string = symbol_name;
	}

	if (strcmp(p->string, "%") == 0) {
		// Sprawdzenie, czy symbole, które nie są liczbami zostały wczesniej zadeklarowane
		if(is_numeric(p->children[0]->string) == 0) {
			//Sprawdzenie, czy lewy argument jest tablicą
			if(strcmp(p->children[0]->string, "TABLE") == 0) {
				check_table(p->children[0]);
				//Dodanie czwórki przypisania wartości do komórki w tablicy
				char *symbol_name = generate_new_table_address();
				symbol *new_symbol = create_symbol(symbol_name, "ADDRESS", address_counter, 1,
						p->first_line, p->first_column, p->last_line, p->last_column);
				address_counter++;
				add_symbol(new_symbol);
				p->children[0]->string = symbol_name;

				add_four("TAB", p->children[0]->children[0]->string, "", p->children[0]->children[1]->string, symbol_name);
			}
			else {
				if(search_for_symbol(p->children[0]->string) == 0) {
					char *string = calloc(strlen("Nie zadeklarowano zmiennej: ") + strlen(p->children[0]->string), sizeof(char));
					strcat(string, "Nie zadeklarowano zmiennej: ");
					strcat(string, p->children[0]->string);
					add_error(create_error(string, p->children[0]->first_line, p->children[0]->first_column,
							p->children[0]->last_line, p->children[0]->last_column));
				}
				else {
					// Sprawdzenie, czy zmienna tablicowa zostla poprawnie użyta
					if(strcmp(find_symbol(p->children[0]->string)->type, "TABLE") == 0) {
						char *string = calloc(strlen("Złe użycie zmiennej tablicowej: ") + strlen(p->children[0]->string), sizeof(char));
						strcat(string, "Złe użycie zmiennej tablicowej: ");
						strcat(string, p->children[0]->string);
						add_error(create_error(string, p->children[0]->first_line, p->children[0]->first_column,
								p->children[0]->last_line, p->children[0]->last_column));
					}
					// Sprawdzenie, czy lewy argument zostal zainicjowany
					else if(strcmp(find_symbol(p->children[0]->string)->type, "ID") == 0 && check_if_symbol_was_initialized(p->children[0]->string) == 0){
						char *string = "Uzycie niezainicjowanej zmiennej";
						add_error(create_error(string, p->children[0]->first_line, p->children[0]->first_column,
								p->children[0]->last_line, p->children[0]->last_column));
					}
				}
			}
		}
		if(is_numeric(p->children[1]->string) == 0) {
			// Sprawdzenie, czy prawy argument jest tablicą
			if(strcmp(p->children[1]->string, "TABLE") == 0) {
				check_table(p->children[1]);
				//Dodanie czwórki przypisania wartości do komórki w tablicy
				char *symbol_name = generate_new_table_address();
				symbol *new_symbol = create_symbol(symbol_name, "ADDRESS", address_counter, 1,
						p->first_line, p->first_column, p->last_line, p->last_column);
				address_counter++;
				add_symbol(new_symbol);
				p->children[1]->string = symbol_name;

				add_four("TAB", p->children[1]->children[0]->string, "", p->children[1]->children[1]->string, symbol_name);
			}
			else {
				if(search_for_symbol(p->children[1]->string) == 0) {
					char *string = calloc(strlen("Nie zadeklarowano zmiennej: ") + strlen(p->children[1]->string), sizeof(char));
					strcat(string, "Nie zadeklarowano zmiennej: ");
					strcat(string, p->children[1]->string);
					add_error(create_error(string, p->children[1]->first_line, p->children[1]->first_column,
							p->children[1]->last_line, p->children[1]->last_column));
				}
				else {
					// Sprawdzenie, czy zmienna tablicowa zostla poprawnie użyta
					if(strcmp(find_symbol(p->children[1]->string)->type, "TABLE") == 0) {
						char *string = calloc(strlen("Złe użycie zmiennej tablicowej: ") + strlen(p->children[1]->string), sizeof(char));
						strcat(string, "Złe użycie zmiennej tablicowej: ");
						strcat(string, p->children[1]->string);
						add_error(create_error(string, p->children[1]->first_line, p->children[1]->first_column,
								p->children[1]->last_line, p->children[1]->last_column));
					}
					// Sprawdzenie, czy prawy argument zostal zainicjowany
					else if(strcmp(find_symbol(p->children[1]->string)->type, "ID") == 0 && check_if_symbol_was_initialized(p->children[1]->string) == 0){
						char *string = "Uzycie niezainicjowanej zmiennej";
						add_error(create_error(string, p->children[1]->first_line, p->children[1]->first_column,
								p->children[1]->last_line, p->children[1]->last_column));
					}
				}
			}
		}


		char *symbol_name = generate_new_address();
		symbol *new_symbol = create_symbol(symbol_name, "-", address_counter, 1,
				p->first_line, p->first_column, p->last_line, p->last_column);
		address_counter++;
		add_symbol(new_symbol);
		add_four("%", p->children[0]->string, "", p->children[1]->string, symbol_name);

		p->string = symbol_name;
	}

	if (strcmp(p->string, "=") == 0) {
		// Sprawdzenie, czy symbole, które nie są liczbami zostały wczesniej zadeklarowane
		if(is_numeric(p->children[0]->string) == 0) {
			//Sprawdzenie, czy lewy argument jest tablicą
			if(strcmp(p->children[0]->string, "TABLE") == 0) {
				check_table(p->children[0]);
				//Dodanie czwórki przypisania wartości do komórki w tablicy
				char *symbol_name = generate_new_table_address();
				symbol *new_symbol = create_symbol(symbol_name, "ADDRESS", address_counter, 1,
						p->first_line, p->first_column, p->last_line, p->last_column);
				address_counter++;
				add_symbol(new_symbol);
				p->children[0]->string = symbol_name;

				add_four("TAB", p->children[0]->children[0]->string, "", p->children[0]->children[1]->string, symbol_name);
			}
			else {
				if(search_for_symbol(p->children[0]->string) == 0) {
					char *string = calloc(strlen("Nie zadeklarowano zmiennej: ") + strlen(p->children[0]->string), sizeof(char));
					strcat(string, "Nie zadeklarowano zmiennej: ");
					strcat(string, p->children[0]->string);
					add_error(create_error(string, p->children[0]->first_line, p->children[0]->first_column,
							p->children[0]->last_line, p->children[0]->last_column));
				}
				else {
					// Sprawdzenie, czy zmienna tablicowa zostla poprawnie użyta
					if(strcmp(find_symbol(p->children[0]->string)->type, "TABLE") == 0) {
						char *string = calloc(strlen("Złe użycie zmiennej tablicowej: ") + strlen(p->children[0]->string), sizeof(char));
						strcat(string, "Złe użycie zmiennej tablicowej: ");
						strcat(string, p->children[0]->string);
						add_error(create_error(string, p->children[0]->first_line, p->children[0]->first_column,
								p->children[0]->last_line, p->children[0]->last_column));
					}
					// Sprawdzenie, czy lewy argument zostal zainicjowany
					else if(strcmp(find_symbol(p->children[0]->string)->type, "ID") == 0 && check_if_symbol_was_initialized(p->children[0]->string) == 0){
						char *string = "Uzycie niezainicjowanej zmiennej";
						add_error(create_error(string, p->children[0]->first_line, p->children[0]->first_column,
								p->children[0]->last_line, p->children[0]->last_column));
					}
				}
			}
		}
		if(is_numeric(p->children[1]->string) == 0) {
			// Sprawdzenie, czy prawy argument jest tablicą
			if(strcmp(p->children[1]->string, "TABLE") == 0) {
				check_table(p->children[1]);
				//Dodanie czwórki przypisania wartości do komórki w tablicy
				char *symbol_name = generate_new_table_address();
				symbol *new_symbol = create_symbol(symbol_name, "ADDRESS", address_counter, 1,
						p->first_line, p->first_column, p->last_line, p->last_column);
				address_counter++;
				add_symbol(new_symbol);
				p->children[1]->string = symbol_name;

				add_four("TAB", p->children[1]->children[0]->string, "", p->children[1]->children[1]->string, symbol_name);
			}
			else {
				if(search_for_symbol(p->children[1]->string) == 0) {
					char *string = calloc(strlen("Nie zadeklarowano zmiennej: ") + strlen(p->children[1]->string), sizeof(char));
					strcat(string, "Nie zadeklarowano zmiennej: ");
					strcat(string, p->children[1]->string);
					add_error(create_error(string, p->children[1]->first_line, p->children[1]->first_column,
							p->children[1]->last_line, p->children[1]->last_column));
				}
				else {
					// Sprawdzenie, czy zmienna tablicowa zostla poprawnie użyta
					if(strcmp(find_symbol(p->children[1]->string)->type, "TABLE") == 0) {
						char *string = calloc(strlen("Złe użycie zmiennej tablicowej: ") + strlen(p->children[1]->string), sizeof(char));
						strcat(string, "Złe użycie zmiennej tablicowej: ");
						strcat(string, p->children[1]->string);
						add_error(create_error(string, p->children[1]->first_line, p->children[1]->first_column,
								p->children[1]->last_line, p->children[1]->last_column));
					}
					// Sprawdzenie, czy prawy argument zostal zainicjowany
					else if(strcmp(find_symbol(p->children[1]->string)->type, "ID") == 0 && check_if_symbol_was_initialized(p->children[1]->string) == 0){
						char *string = "Uzycie niezainicjowanej zmiennej";
						add_error(create_error(string, p->children[1]->first_line, p->children[1]->first_column,
								p->children[1]->last_line, p->children[1]->last_column));
					}
				}
			}
		}


		char *symbol_name = generate_new_address();
		symbol *new_symbol = create_symbol(symbol_name, "-", address_counter, 1,
				p->first_line, p->first_column, p->last_line, p->last_column);
		address_counter++;
		add_symbol(new_symbol);
		add_four("=", p->children[0]->string, "", p->children[1]->string, symbol_name);

		p->string = symbol_name;
	}

	if (strcmp(p->string, "!=") == 0) {
		// Sprawdzenie, czy symbole, które nie są liczbami zostały wczesniej zadeklarowane
		if(is_numeric(p->children[0]->string) == 0) {
			//Sprawdzenie, czy lewy argument jest tablicą
			if(strcmp(p->children[0]->string, "TABLE") == 0) {
				check_table(p->children[0]);
				//Dodanie czwórki przypisania wartości do komórki w tablicy
				char *symbol_name = generate_new_table_address();
				symbol *new_symbol = create_symbol(symbol_name, "ADDRESS", address_counter, 1,
						p->first_line, p->first_column, p->last_line, p->last_column);
				address_counter++;
				add_symbol(new_symbol);
				p->children[0]->string = symbol_name;

				add_four("TAB", p->children[0]->children[0]->string, "", p->children[0]->children[1]->string, symbol_name);
			}
			else {
				if(search_for_symbol(p->children[0]->string) == 0) {
					char *string = calloc(strlen("Nie zadeklarowano zmiennej: ") + strlen(p->children[0]->string), sizeof(char));
					strcat(string, "Nie zadeklarowano zmiennej: ");
					strcat(string, p->children[0]->string);
					add_error(create_error(string, p->children[0]->first_line, p->children[0]->first_column,
							p->children[0]->last_line, p->children[0]->last_column));
				}
				else {
					// Sprawdzenie, czy zmienna tablicowa zostla poprawnie użyta
					if(strcmp(find_symbol(p->children[0]->string)->type, "TABLE") == 0) {
						char *string = calloc(strlen("Złe użycie zmiennej tablicowej: ") + strlen(p->children[0]->string), sizeof(char));
						strcat(string, "Złe użycie zmiennej tablicowej: ");
						strcat(string, p->children[0]->string);
						add_error(create_error(string, p->children[0]->first_line, p->children[0]->first_column,
								p->children[0]->last_line, p->children[0]->last_column));
					}
					// Sprawdzenie, czy lewy argument zostal zainicjowany
					else if(strcmp(find_symbol(p->children[0]->string)->type, "ID") == 0 && check_if_symbol_was_initialized(p->children[0]->string) == 0){
						char *string = "Uzycie niezainicjowanej zmiennej";
						add_error(create_error(string, p->children[0]->first_line, p->children[0]->first_column,
								p->children[0]->last_line, p->children[0]->last_column));
					}
				}
			}
		}
		if(is_numeric(p->children[1]->string) == 0) {
			// Sprawdzenie, czy prawy argument jest tablicą
			if(strcmp(p->children[1]->string, "TABLE") == 0) {
				check_table(p->children[1]);
				//Dodanie czwórki przypisania wartości do komórki w tablicy
				char *symbol_name = generate_new_table_address();
				symbol *new_symbol = create_symbol(symbol_name, "ADDRESS", address_counter, 1,
						p->first_line, p->first_column, p->last_line, p->last_column);
				address_counter++;
				add_symbol(new_symbol);
				p->children[1]->string = symbol_name;

				add_four("TAB", p->children[1]->children[0]->string, "", p->children[1]->children[1]->string, symbol_name);
			}
			else {
				if(search_for_symbol(p->children[1]->string) == 0) {
					char *string = calloc(strlen("Nie zadeklarowano zmiennej: ") + strlen(p->children[1]->string), sizeof(char));
					strcat(string, "Nie zadeklarowano zmiennej: ");
					strcat(string, p->children[1]->string);
					add_error(create_error(string, p->children[1]->first_line, p->children[1]->first_column,
							p->children[1]->last_line, p->children[1]->last_column));
				}
				else {
					// Sprawdzenie, czy zmienna tablicowa zostla poprawnie użyta
					if(strcmp(find_symbol(p->children[1]->string)->type, "TABLE") == 0) {
						char *string = calloc(strlen("Złe użycie zmiennej tablicowej: ") + strlen(p->children[1]->string), sizeof(char));
						strcat(string, "Złe użycie zmiennej tablicowej: ");
						strcat(string, p->children[1]->string);
						add_error(create_error(string, p->children[1]->first_line, p->children[1]->first_column,
								p->children[1]->last_line, p->children[1]->last_column));
					}
					// Sprawdzenie, czy prawy argument zostal zainicjowany
					else if(strcmp(find_symbol(p->children[1]->string)->type, "ID") == 0 && check_if_symbol_was_initialized(p->children[1]->string) == 0){
						char *string = "Uzycie niezainicjowanej zmiennej";
						add_error(create_error(string, p->children[1]->first_line, p->children[1]->first_column,
								p->children[1]->last_line, p->children[1]->last_column));
					}
				}
			}
		}


		char *symbol_name = generate_new_address();
		symbol *new_symbol = create_symbol(symbol_name, "-", address_counter, 1,
				p->first_line, p->first_column, p->last_line, p->last_column);
		address_counter++;
		add_symbol(new_symbol);
		add_four("!=", p->children[0]->string, "", p->children[1]->string, symbol_name);

		p->string = symbol_name;
	}

	if (strcmp(p->string, "<") == 0) {
		// Sprawdzenie, czy symbole, które nie są liczbami zostały wczesniej zadeklarowane
		if(is_numeric(p->children[0]->string) == 0) {
			//Sprawdzenie, czy lewy argument jest tablicą
			if(strcmp(p->children[0]->string, "TABLE") == 0) {
				check_table(p->children[0]);
				//Dodanie czwórki przypisania wartości do komórki w tablicy
				char *symbol_name = generate_new_table_address();
				symbol *new_symbol = create_symbol(symbol_name, "ADDRESS", address_counter, 1,
						p->first_line, p->first_column, p->last_line, p->last_column);
				address_counter++;
				add_symbol(new_symbol);
				p->children[0]->string = symbol_name;

				add_four("TAB", p->children[0]->children[0]->string, "", p->children[0]->children[1]->string, symbol_name);
			}
			else {
				if(search_for_symbol(p->children[0]->string) == 0) {
					char *string = calloc(strlen("Nie zadeklarowano zmiennej: ") + strlen(p->children[0]->string), sizeof(char));
					strcat(string, "Nie zadeklarowano zmiennej: ");
					strcat(string, p->children[0]->string);
					add_error(create_error(string, p->children[0]->first_line, p->children[0]->first_column,
							p->children[0]->last_line, p->children[0]->last_column));
				}
				else {
					// Sprawdzenie, czy zmienna tablicowa zostla poprawnie użyta
					if(strcmp(find_symbol(p->children[0]->string)->type, "TABLE") == 0) {
						char *string = calloc(strlen("Złe użycie zmiennej tablicowej: ") + strlen(p->children[0]->string), sizeof(char));
						strcat(string, "Złe użycie zmiennej tablicowej: ");
						strcat(string, p->children[0]->string);
						add_error(create_error(string, p->children[0]->first_line, p->children[0]->first_column,
								p->children[0]->last_line, p->children[0]->last_column));
					}
					// Sprawdzenie, czy lewy argument zostal zainicjowany
					else if(strcmp(find_symbol(p->children[0]->string)->type, "ID") == 0 && check_if_symbol_was_initialized(p->children[0]->string) == 0){
						char *string = "Uzycie niezainicjowanej zmiennej";
						add_error(create_error(string, p->children[0]->first_line, p->children[0]->first_column,
								p->children[0]->last_line, p->children[0]->last_column));
					}
				}
			}
		}
		if(is_numeric(p->children[1]->string) == 0) {
			// Sprawdzenie, czy prawy argument jest tablicą
			if(strcmp(p->children[1]->string, "TABLE") == 0) {
				check_table(p->children[1]);
				//Dodanie czwórki przypisania wartości do komórki w tablicy
				char *symbol_name = generate_new_table_address();
				symbol *new_symbol = create_symbol(symbol_name, "ADDRESS", address_counter, 1,
						p->first_line, p->first_column, p->last_line, p->last_column);
				address_counter++;
				add_symbol(new_symbol);
				p->children[1]->string = symbol_name;

				add_four("TAB", p->children[1]->children[0]->string, "", p->children[1]->children[1]->string, symbol_name);
			}
			else {
				if(search_for_symbol(p->children[1]->string) == 0) {
					char *string = calloc(strlen("Nie zadeklarowano zmiennej: ") + strlen(p->children[1]->string), sizeof(char));
					strcat(string, "Nie zadeklarowano zmiennej: ");
					strcat(string, p->children[1]->string);
					add_error(create_error(string, p->children[1]->first_line, p->children[1]->first_column,
							p->children[1]->last_line, p->children[1]->last_column));
				}
				else {
					// Sprawdzenie, czy zmienna tablicowa zostla poprawnie użyta
					if(strcmp(find_symbol(p->children[1]->string)->type, "TABLE") == 0) {
						char *string = calloc(strlen("Złe użycie zmiennej tablicowej: ") + strlen(p->children[1]->string), sizeof(char));
						strcat(string, "Złe użycie zmiennej tablicowej: ");
						strcat(string, p->children[1]->string);
						add_error(create_error(string, p->children[1]->first_line, p->children[1]->first_column,
								p->children[1]->last_line, p->children[1]->last_column));
					}
					// Sprawdzenie, czy prawy argument zostal zainicjowany
					else if(strcmp(find_symbol(p->children[1]->string)->type, "ID") == 0 && check_if_symbol_was_initialized(p->children[1]->string) == 0){
						char *string = "Uzycie niezainicjowanej zmiennej";
						add_error(create_error(string, p->children[1]->first_line, p->children[1]->first_column,
								p->children[1]->last_line, p->children[1]->last_column));
					}
				}
			}
		}


		char *symbol_name = generate_new_address();
		symbol *new_symbol = create_symbol(symbol_name, "-", address_counter, 1,
				p->first_line, p->first_column, p->last_line, p->last_column);
		address_counter++;
		add_symbol(new_symbol);
		add_four("<", p->children[0]->string, "", p->children[1]->string, symbol_name);

		p->string = symbol_name;
	}

	if (strcmp(p->string, ">") == 0) {
		// Sprawdzenie, czy symbole, które nie są liczbami zostały wczesniej zadeklarowane
		if(is_numeric(p->children[0]->string) == 0) {
			//Sprawdzenie, czy lewy argument jest tablicą
			if(strcmp(p->children[0]->string, "TABLE") == 0) {
				check_table(p->children[0]);
				//Dodanie czwórki przypisania wartości do komórki w tablicy
				char *symbol_name = generate_new_table_address();
				symbol *new_symbol = create_symbol(symbol_name, "ADDRESS", address_counter, 1,
						p->first_line, p->first_column, p->last_line, p->last_column);
				address_counter++;
				add_symbol(new_symbol);
				p->children[0]->string = symbol_name;

				add_four("TAB", p->children[0]->children[0]->string, "", p->children[0]->children[1]->string, symbol_name);
			}
			else {
				if(search_for_symbol(p->children[0]->string) == 0) {
					char *string = calloc(strlen("Nie zadeklarowano zmiennej: ") + strlen(p->children[0]->string), sizeof(char));
					strcat(string, "Nie zadeklarowano zmiennej: ");
					strcat(string, p->children[0]->string);
					add_error(create_error(string, p->children[0]->first_line, p->children[0]->first_column,
							p->children[0]->last_line, p->children[0]->last_column));
				}
				else {
					// Sprawdzenie, czy zmienna tablicowa zostla poprawnie użyta
					if(strcmp(find_symbol(p->children[0]->string)->type, "TABLE") == 0) {
						char *string = calloc(strlen("Złe użycie zmiennej tablicowej: ") + strlen(p->children[0]->string), sizeof(char));
						strcat(string, "Złe użycie zmiennej tablicowej: ");
						strcat(string, p->children[0]->string);
						add_error(create_error(string, p->children[0]->first_line, p->children[0]->first_column,
								p->children[0]->last_line, p->children[0]->last_column));
					}
					// Sprawdzenie, czy lewy argument zostal zainicjowany
					else if(strcmp(find_symbol(p->children[0]->string)->type, "ID") == 0 && check_if_symbol_was_initialized(p->children[0]->string) == 0){
						char *string = "Uzycie niezainicjowanej zmiennej";
						add_error(create_error(string, p->children[0]->first_line, p->children[0]->first_column,
								p->children[0]->last_line, p->children[0]->last_column));
					}
				}
			}
		}
		if(is_numeric(p->children[1]->string) == 0) {
			// Sprawdzenie, czy prawy argument jest tablicą
			if(strcmp(p->children[1]->string, "TABLE") == 0) {
				check_table(p->children[1]);
				//Dodanie czwórki przypisania wartości do komórki w tablicy
				char *symbol_name = generate_new_table_address();
				symbol *new_symbol = create_symbol(symbol_name, "ADDRESS", address_counter, 1,
						p->first_line, p->first_column, p->last_line, p->last_column);
				address_counter++;
				add_symbol(new_symbol);
				p->children[1]->string = symbol_name;

				add_four("TAB", p->children[1]->children[0]->string, "", p->children[1]->children[1]->string, symbol_name);
			}
			else {
				if(search_for_symbol(p->children[1]->string) == 0) {
					char *string = calloc(strlen("Nie zadeklarowano zmiennej: ") + strlen(p->children[1]->string), sizeof(char));
					strcat(string, "Nie zadeklarowano zmiennej: ");
					strcat(string, p->children[1]->string);
					add_error(create_error(string, p->children[1]->first_line, p->children[1]->first_column,
							p->children[1]->last_line, p->children[1]->last_column));
				}
				else {
					// Sprawdzenie, czy zmienna tablicowa zostla poprawnie użyta
					if(strcmp(find_symbol(p->children[1]->string)->type, "TABLE") == 0) {
						char *string = calloc(strlen("Złe użycie zmiennej tablicowej: ") + strlen(p->children[1]->string), sizeof(char));
						strcat(string, "Złe użycie zmiennej tablicowej: ");
						strcat(string, p->children[1]->string);
						add_error(create_error(string, p->children[1]->first_line, p->children[1]->first_column,
								p->children[1]->last_line, p->children[1]->last_column));
					}
					// Sprawdzenie, czy prawy argument zostal zainicjowany
					else if(strcmp(find_symbol(p->children[1]->string)->type, "ID") == 0 && check_if_symbol_was_initialized(p->children[1]->string) == 0){
						char *string = "Uzycie niezainicjowanej zmiennej";
						add_error(create_error(string, p->children[1]->first_line, p->children[1]->first_column,
								p->children[1]->last_line, p->children[1]->last_column));
					}
				}
			}
		}


		char *symbol_name = generate_new_address();
		symbol *new_symbol = create_symbol(symbol_name, "-", address_counter, 1,
				p->first_line, p->first_column, p->last_line, p->last_column);
		address_counter++;
		add_symbol(new_symbol);
		add_four(">", p->children[0]->string, "", p->children[1]->string, symbol_name);

		p->string = symbol_name;
	}

	if (strcmp(p->string, "<=") == 0) {
		// Sprawdzenie, czy symbole, które nie są liczbami zostały wczesniej zadeklarowane
		if(is_numeric(p->children[0]->string) == 0) {
			//Sprawdzenie, czy lewy argument jest tablicą
			if(strcmp(p->children[0]->string, "TABLE") == 0) {
				check_table(p->children[0]);
				//Dodanie czwórki przypisania wartości do komórki w tablicy
				char *symbol_name = generate_new_table_address();
				symbol *new_symbol = create_symbol(symbol_name, "ADDRESS", address_counter, 1,
						p->first_line, p->first_column, p->last_line, p->last_column);
				address_counter++;
				add_symbol(new_symbol);
				p->children[0]->string = symbol_name;

				add_four("TAB", p->children[0]->children[0]->string, "", p->children[0]->children[1]->string, symbol_name);
			}
			else {
				if(search_for_symbol(p->children[0]->string) == 0) {
					char *string = calloc(strlen("Nie zadeklarowano zmiennej: ") + strlen(p->children[0]->string), sizeof(char));
					strcat(string, "Nie zadeklarowano zmiennej: ");
					strcat(string, p->children[0]->string);
					add_error(create_error(string, p->children[0]->first_line, p->children[0]->first_column,
							p->children[0]->last_line, p->children[0]->last_column));
				}
				else {
					// Sprawdzenie, czy zmienna tablicowa zostla poprawnie użyta
					if(strcmp(find_symbol(p->children[0]->string)->type, "TABLE") == 0) {
						char *string = calloc(strlen("Złe użycie zmiennej tablicowej: ") + strlen(p->children[0]->string), sizeof(char));
						strcat(string, "Złe użycie zmiennej tablicowej: ");
						strcat(string, p->children[0]->string);
						add_error(create_error(string, p->children[0]->first_line, p->children[0]->first_column,
								p->children[0]->last_line, p->children[0]->last_column));
					}
					// Sprawdzenie, czy lewy argument zostal zainicjowany
					else if(strcmp(find_symbol(p->children[0]->string)->type, "ID") == 0 && check_if_symbol_was_initialized(p->children[0]->string) == 0){
						char *string = "Uzycie niezainicjowanej zmiennej";
						add_error(create_error(string, p->children[0]->first_line, p->children[0]->first_column,
								p->children[0]->last_line, p->children[0]->last_column));
					}
				}
			}
		}
		if(is_numeric(p->children[1]->string) == 0) {
			// Sprawdzenie, czy prawy argument jest tablicą
			if(strcmp(p->children[1]->string, "TABLE") == 0) {
				check_table(p->children[1]);
				//Dodanie czwórki przypisania wartości do komórki w tablicy
				char *symbol_name = generate_new_table_address();
				symbol *new_symbol = create_symbol(symbol_name, "ADDRESS", address_counter, 1,
						p->first_line, p->first_column, p->last_line, p->last_column);
				address_counter++;
				add_symbol(new_symbol);
				p->children[1]->string = symbol_name;

				add_four("TAB", p->children[1]->children[0]->string, "", p->children[1]->children[1]->string, symbol_name);
			}
			else {
				if(search_for_symbol(p->children[1]->string) == 0) {
					char *string = calloc(strlen("Nie zadeklarowano zmiennej: ") + strlen(p->children[1]->string), sizeof(char));
					strcat(string, "Nie zadeklarowano zmiennej: ");
					strcat(string, p->children[1]->string);
					add_error(create_error(string, p->children[1]->first_line, p->children[1]->first_column,
							p->children[1]->last_line, p->children[1]->last_column));
				}
				else {
					// Sprawdzenie, czy zmienna tablicowa zostla poprawnie użyta
					if(strcmp(find_symbol(p->children[1]->string)->type, "TABLE") == 0) {
						char *string = calloc(strlen("Złe użycie zmiennej tablicowej: ") + strlen(p->children[1]->string), sizeof(char));
						strcat(string, "Złe użycie zmiennej tablicowej: ");
						strcat(string, p->children[1]->string);
						add_error(create_error(string, p->children[1]->first_line, p->children[1]->first_column,
								p->children[1]->last_line, p->children[1]->last_column));
					}
					// Sprawdzenie, czy prawy argument zostal zainicjowany
					else if(strcmp(find_symbol(p->children[1]->string)->type, "ID") == 0 && check_if_symbol_was_initialized(p->children[1]->string) == 0){
						char *string = "Uzycie niezainicjowanej zmiennej";
						add_error(create_error(string, p->children[1]->first_line, p->children[1]->first_column,
								p->children[1]->last_line, p->children[1]->last_column));
					}
				}
			}
		}


		char *symbol_name = generate_new_address();
		symbol *new_symbol = create_symbol(symbol_name, "-", address_counter, 1,
				p->first_line, p->first_column, p->last_line, p->last_column);
		address_counter++;
		add_symbol(new_symbol);
		add_four("<=", p->children[0]->string, "", p->children[1]->string, symbol_name);

		p->string = symbol_name;
	}

	if (strcmp(p->string, ">=") == 0) {
		// Sprawdzenie, czy symbole, które nie są liczbami zostały wczesniej zadeklarowane
		if(is_numeric(p->children[0]->string) == 0) {
			//Sprawdzenie, czy lewy argument jest tablicą
			if(strcmp(p->children[0]->string, "TABLE") == 0) {
				check_table(p->children[0]);
				//Dodanie czwórki przypisania wartości do komórki w tablicy
				char *symbol_name = generate_new_table_address();
				symbol *new_symbol = create_symbol(symbol_name, "ADDRESS", address_counter, 1,
						p->first_line, p->first_column, p->last_line, p->last_column);
				address_counter++;
				add_symbol(new_symbol);
				p->children[0]->string = symbol_name;

				add_four("TAB", p->children[0]->children[0]->string, "", p->children[0]->children[1]->string, symbol_name);
			}
			else {
				if(search_for_symbol(p->children[0]->string) == 0) {
					char *string = calloc(strlen("Nie zadeklarowano zmiennej: ") + strlen(p->children[0]->string), sizeof(char));
					strcat(string, "Nie zadeklarowano zmiennej: ");
					strcat(string, p->children[0]->string);
					add_error(create_error(string, p->children[0]->first_line, p->children[0]->first_column,
							p->children[0]->last_line, p->children[0]->last_column));
				}
				else {
					// Sprawdzenie, czy zmienna tablicowa zostla poprawnie użyta
					if(strcmp(find_symbol(p->children[0]->string)->type, "TABLE") == 0) {
						char *string = calloc(strlen("Złe użycie zmiennej tablicowej: ") + strlen(p->children[0]->string), sizeof(char));
						strcat(string, "Złe użycie zmiennej tablicowej: ");
						strcat(string, p->children[0]->string);
						add_error(create_error(string, p->children[0]->first_line, p->children[0]->first_column,
								p->children[0]->last_line, p->children[0]->last_column));
					}
					// Sprawdzenie, czy lewy argument zostal zainicjowany
					else if(strcmp(find_symbol(p->children[0]->string)->type, "ID") == 0 && check_if_symbol_was_initialized(p->children[0]->string) == 0){
						char *string = "Uzycie niezainicjowanej zmiennej";
						add_error(create_error(string, p->children[0]->first_line, p->children[0]->first_column,
								p->children[0]->last_line, p->children[0]->last_column));
					}
				}
			}
		}
		if(is_numeric(p->children[1]->string) == 0) {
			// Sprawdzenie, czy prawy argument jest tablicą
			if(strcmp(p->children[1]->string, "TABLE") == 0) {
				check_table(p->children[1]);
				//Dodanie czwórki przypisania wartości do komórki w tablicy
				char *symbol_name = generate_new_table_address();
				symbol *new_symbol = create_symbol(symbol_name, "ADDRESS", address_counter, 1,
						p->first_line, p->first_column, p->last_line, p->last_column);
				address_counter++;
				add_symbol(new_symbol);
				p->children[1]->string = symbol_name;

				add_four("TAB", p->children[1]->children[0]->string, "", p->children[1]->children[1]->string, symbol_name);
			}
			else {
				if(search_for_symbol(p->children[1]->string) == 0) {
					char *string = calloc(strlen("Nie zadeklarowano zmiennej: ") + strlen(p->children[1]->string), sizeof(char));
					strcat(string, "Nie zadeklarowano zmiennej: ");
					strcat(string, p->children[1]->string);
					add_error(create_error(string, p->children[1]->first_line, p->children[1]->first_column,
							p->children[1]->last_line, p->children[1]->last_column));
				}
				else {
					// Sprawdzenie, czy zmienna tablicowa zostla poprawnie użyta
					if(strcmp(find_symbol(p->children[1]->string)->type, "TABLE") == 0) {
						char *string = calloc(strlen("Złe użycie zmiennej tablicowej: ") + strlen(p->children[1]->string), sizeof(char));
						strcat(string, "Złe użycie zmiennej tablicowej: ");
						strcat(string, p->children[1]->string);
						add_error(create_error(string, p->children[1]->first_line, p->children[1]->first_column,
								p->children[1]->last_line, p->children[1]->last_column));
					}
					// Sprawdzenie, czy prawy argument zostal zainicjowany
					else if(strcmp(find_symbol(p->children[1]->string)->type, "ID") == 0 && check_if_symbol_was_initialized(p->children[1]->string) == 0){
						char *string = "Uzycie niezainicjowanej zmiennej";
						add_error(create_error(string, p->children[1]->first_line, p->children[1]->first_column,
								p->children[1]->last_line, p->children[1]->last_column));
					}
				}
			}
		}


		char *symbol_name = generate_new_address();
		symbol *new_symbol = create_symbol(symbol_name, "-", address_counter, 1,
				p->first_line, p->first_column, p->last_line, p->last_column);
		address_counter++;
		add_symbol(new_symbol);
		add_four(">=", p->children[0]->string, "", p->children[1]->string, symbol_name);

		p->string = symbol_name;
	}

}

/* translate jednoczesnie dodaje symbole do tablicy symboli i generuje kod trójadresowy */
void translate(node *p) {
	int i = 0;
	// IF, WHILE ...
	if(strcmp(p->string, "VDEC") == 0) {
		add_symbols_sublist();
		add_initialized_symbols_sublist();
		while (i < p->number_of_children) {
			if(check_if_symbol_already_declared(p->children[i]->string) == 1) {
				char *string = calloc(strlen("Zmienna '") + strlen(p->children[i]->string) +
						strlen("' została już wczesniej zadeklarowana."), sizeof(char));
				strcat(string, "Zmienna '");
				strcat(string, p->children[i]->string);
				strcat(string, "' została już wczesniej zadeklarowana.");
				add_error(create_error(string, p->children[i]->first_line, p->children[i]->first_column,
						p->children[i]->last_line, p->children[i]->last_column));
			}
			else {
				if(strcmp(p->children[i]->string, "VDEC_TABLE") == 0) {
					symbol *new_symbol = create_symbol(p->children[i]->children[0]->string, "TABLE",
							address_counter, atoi(p->children[i]->children[1]->string),
							p->children[i]->first_line, p->children[i]->first_column,
							p->children[i]->last_line, p->children[i]->last_column);
					address_counter+= atoi(p->children[i]->children[1]->string);
					add_symbol(new_symbol);
				}
				else {
					symbol *new_symbol = create_symbol(p->children[i]->string, "ID", address_counter, 1,
							p->children[i]->first_line, p->children[i]->first_column,
							p->children[i]->last_line, p->children[i]->last_column);
					address_counter++;
					add_symbol(new_symbol);
					// Dodanie initialized_symbol
					add_initialized_symbol(p->children[i]->string);
				}
			}
			i++;
		}
	}

	else if (strcmp(p->string, "IF") == 0) {
		// Najpier dodaj warunek z wewnątrz IF
		interpret(p->children[0]);

		// Utwórz nowy symbol (t2) z przykładu "IFZ t1 GOTO L1 t2"
		char *symbol_name = generate_new_address();
		symbol *new_symbol = create_symbol(symbol_name, "-", address_counter, 1,
				p->first_line, p->first_column, p->last_line, p->last_column);
		address_counter++;
		add_symbol(new_symbol);

		// Utwórz etykietę L1 z powyzszego przykładu
		char *new_label = generate_new_label();
		p->string = symbol_name;

		add_four("IFZ", p->children[0]->string, "GOTO", new_label, symbol_name);

		translate(p->children[1]);

		add_four("", "", "", "", new_label);
	}

	else if (strcmp(p->string, "IF_ELSE") == 0) {
		interpret(p->children[0]);

		// Utwórz nowy symbol (t2) z przykładu "IFZ t1 GOTO L1 t2"
		char *symbol_name_1 = generate_new_address();
		symbol *new_symbol = create_symbol(symbol_name_1, "-", address_counter, 1,
				p->first_line, p->first_column, p->last_line, p->last_column);
		address_counter++;
		add_symbol(new_symbol);
		// Utwórz etykietę L1 z powyzszego przykładu
		char *new_label_1 = generate_new_label();
		p->string = symbol_name_1;

		add_four("IFZ", p->children[0]->string, "GOTO", new_label_1, symbol_name_1);

		// Komendy dla prawdziwego IF
		translate(p->children[1]);

		char *symbol_name_2 = generate_new_address();
		new_symbol = create_symbol(symbol_name_2, "-", address_counter, 1,
				p->first_line, p->first_column, p->last_line, p->last_column);
		address_counter++;
		add_symbol(new_symbol);
		char *new_label_2 = generate_new_label();
		p->string = symbol_name_2;

		add_four("GOTO", new_label_2, "", "", symbol_name_2);


		add_four("","","","", new_label_1);

		// Komendy dla fałszywego IF (za ELSE)
		translate(p->children[2]);

		add_four("", "", "", "", new_label_2);
	}

	else if (strcmp(p->string, "WHILE") == 0) {
		char *new_label_1 = generate_new_label();

		add_four("", "", "", "", new_label_1);

		translate(p->children[0]);

		// Utwórz nowy symbol (t2) z przykładu "IFZ t1 GOTO L1 t2"
		char *symbol_name_1 = generate_new_address();
		symbol *new_symbol = create_symbol(symbol_name_1, "-", address_counter, 1,
				p->first_line, p->first_column, p->last_line, p->last_column);
		address_counter++;
		add_symbol(new_symbol);
		// Utwórz etykietę L1 z powyzszego przykładu
		char *new_label_2 = generate_new_label();
		p->string = symbol_name_1;

		add_four("IFZ", p->children[0]->string, "GOTO", new_label_2, symbol_name_1);

		translate(p->children[1]);

		char *symbol_name_2 = generate_new_address();
		new_symbol = create_symbol(symbol_name_1, "-", address_counter, 1,
				p->first_line, p->first_column, p->last_line, p->last_column);
		address_counter++;
		add_symbol(new_symbol);
		p->string = symbol_name_2;

		add_four("GOTO", new_label_1, "", "", symbol_name_2);

		add_four("", "", "", "", new_label_2);
	}

	else if (strcmp(p->string, "FOR") == 0) {
		// Utworzenie nowej podlisty, w której zadeklarujemy iterator petli FOR
		add_symbols_sublist();
		//add_initialized_symbols_sublist();
		// Dodanie iteratora do nowej podlisty
		symbol *new_symbol = create_symbol(p->children[0]->string, "ITERATOR", address_counter, 1,
				p->first_line, p->first_column, p->last_line, p->last_column);
		address_counter++;
		add_symbol(new_symbol);

		// Dodanie wiersza przypisania wartości do iteratora dla pętli FOR np. ":= i - 0 i"
		add_four(":=", p->children[0]->string, "", p->children[1]->string, p->children[0]->string);

		// Utworzenie nowej etykiety
		char *new_label_1 = generate_new_label();
		add_four("", "", "", "", new_label_1);

		// Utworzenie warunku dla iteratora pętli FOR np. "< i - 10 t0"
		char *symbol_name_1 = generate_new_address();
		new_symbol = create_symbol(symbol_name_1, "-", address_counter, 1,
				p->first_line, p->first_column, p->last_line, p->last_column);
		address_counter++;
		add_symbol(new_symbol);
		p->string = symbol_name_1;

		add_four("<=", p->children[0]->string, "", p->children[2]->string, symbol_name_1);


		char *symbol_name_2 = generate_new_address();
		new_symbol = create_symbol(symbol_name_2, "-", address_counter, 1,
				p->first_line, p->first_column, p->last_line, p->last_column);
		address_counter++;
		add_symbol(new_symbol);
		p->string = symbol_name_2;
		char *new_label_2 = generate_new_label();
		p->string = symbol_name_2;

		add_four("IFZ", p->children[0]->string, "GOTO", new_label_2, symbol_name_2);

		// Zapamietaj iterator. Jesli poniżej zostalo zmienione, wyrzuć błąd [DO ZROBIENIA]
		translate(p->children[3]);

		add_four("++", p->children[0]->string, "", "", p->children[0]->string);

		char *symbol_name_3 = generate_new_address();
		new_symbol = create_symbol(symbol_name_3, "-", address_counter, 1,
				p->first_line, p->first_column, p->last_line, p->last_column);
		address_counter++;
		add_symbol(new_symbol);
		p->string = symbol_name_3;

		add_four("GOTO", new_label_1, "", "", symbol_name_3);

		add_four("", "", "", "", new_label_2);
		// Usunięcie ostatniej podlisty
		//remove_symbols_sublist();
	}

	else if (strcmp(p->string, "FOR_DOWN") == 0) {
		// Utworzenie nowej podlisty, w której zadeklarujemy iterator petli FOR
		add_symbols_sublist();
		//add_initialized_symbols_sublist();
		// Dodanie iteratora do nowej podlisty
		symbol *new_symbol = create_symbol(p->children[0]->string, "ITERATOR", address_counter, 1,
				p->first_line, p->first_column, p->last_line, p->last_column);
		address_counter++;
		add_symbol(new_symbol);

		// Dodanie wiersza przypisania wartości do iteratora dla pętli FOR np. ":= i - 0 i"
		add_four(":=", p->children[0]->string, "", p->children[1]->string, p->children[0]->string);

		// Utworzenie nowej etykiety
		char *new_label_1 = generate_new_label();
		add_four("", "", "", "", new_label_1);

		// Utworzenie warunku dla iteratora pętli FOR np. "< i - 10 t0"
		char *symbol_name_1 = generate_new_address();
		new_symbol = create_symbol(symbol_name_1, "-", address_counter, 1,
				p->first_line, p->first_column, p->last_line, p->last_column);
		address_counter++;
		add_symbol(new_symbol);
		p->string = symbol_name_1;

		add_four(">", p->children[0]->string, "", p->children[2]->string, symbol_name_1);


		char *symbol_name_2 = generate_new_address();
		new_symbol = create_symbol(symbol_name_2, "-", address_counter, 1,
				p->first_line, p->first_column, p->last_line, p->last_column);
		address_counter++;
		add_symbol(new_symbol);
		p->string = symbol_name_2;
		char *new_label_2 = generate_new_label();
		p->string = symbol_name_2;

		add_four("IFZ", p->children[0]->string, "GOTO", new_label_2, symbol_name_2);

		// Zapamietaj iterator. Jesli poniżej zostalo zmienione, wyrzuć błąd [DO ZROBIENIA]
		translate(p->children[3]);

		add_four("--", p->children[0]->string, "", "", p->children[0]->string);

		char *symbol_name_3 = generate_new_address();
		new_symbol = create_symbol(symbol_name_3, "-", address_counter, 1,
				p->first_line, p->first_column, p->last_line, p->last_column);
		address_counter++;
		add_symbol(new_symbol);
		p->string = symbol_name_3;

		add_four("GOTO", new_label_1, "", "", symbol_name_3);

		add_four("", "", "", "", new_label_2);

		//****Dodane sprawdzenie, czy arg2 = 0*******
		char *symbol_name_4 = generate_new_address();
		new_symbol = create_symbol(symbol_name_4, "-", address_counter, 1,
				p->first_line, p->first_column, p->last_line, p->last_column);
		address_counter++;
		add_symbol(new_symbol);
		p->string = symbol_name_4;

		add_four("=", p->children[0]->string, "", p->children[2]->string, symbol_name_4);

		char *symbol_name_5 = generate_new_address();
		new_symbol = create_symbol(symbol_name_5, "-", address_counter, 1,
				p->first_line, p->first_column, p->last_line, p->last_column);
		address_counter++;
		add_symbol(new_symbol);
		p->string = symbol_name_5;
		char *new_label_3 = generate_new_label();
		p->string = symbol_name_5;

		add_four("IFZ", symbol_name_4, "GOTO", new_label_3, symbol_name_5);

		translate(p->children[3]);

		add_four("", "", "", "", new_label_3);



		// Usunięcie ostatniej podlisty
		//remove_symbols_sublist();
	}

	else {
		while (i < p->number_of_children) {
			translate(p->children[i]);
			i++;
		}
		interpret(p);
	}
}
