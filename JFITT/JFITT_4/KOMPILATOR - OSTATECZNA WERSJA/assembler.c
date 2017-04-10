#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>
#include "main.h"

char *to_char(unsigned long int k) {
	char *number = calloc(100000, sizeof(char));
	snprintf(number, 100000 *sizeof(char), "%lu", k);
	return number;
}

void add_row(char *order, char *arg1, char *arg2) {
	assembler_line *new_line = malloc(sizeof(assembler_line));
	new_line->row = k;
	new_line->order = order;
	new_line->arg1 = arg1;
	new_line->arg2 = arg2;

	struct assembler_line *temp = assembler_root;

	if(assembler_root == NULL) assembler_root = new_line;
	else {

		while(temp->next != NULL) {
			temp = temp->next;
		}

		new_line->next = temp->next;
		temp->next = new_line;
	}
	k++;
}

void print_assembler() {
	assembler_line *temp = assembler_root;

	while(temp != NULL) {
		printf("%*lu %*s %*s %*s\n", 4, temp->row, 5, temp->order, 5, temp->arg1, 5, temp->arg2);
		temp = temp->next;
	}
}

void generate_number(unsigned long int number, char *r) {
	char stack[100] = "";
	int i = 0;
	if (number > 0) {
		while (number > 0) {
			if (number & 1) { // Jest nieparzysta
				stack[i] = 'I';
				number--;
			}
			else { // Jest parzysta
				stack[i] = 'S';
				number = number / 2;
			}
			i++;
		}
		i--;
		add_row("RESET", r, "");
		while (i >= 0) {
			if (stack[i] == 'I') {
				add_row("INC", r, "");
			}
			else {
				add_row("SHL", r, "");
			}
			i--;
		}
	}
	else {
		add_row("RESET", r, "");
	}
}
/*
 * int backpatch() {
	assembler_line *temp = assembler_root;

	if(assembler_root == NULL) return 0;

	while(temp != NULL) {
		if(strcmp(temp->order, "") == 0) {
			char *row = to_char(temp->row);
			assembler_line *temp2 = assembler_root;
			while(temp2 != NULL) {
				if(strcmp(temp2->order, "") != 0) {
					if(strcmp(temp->arg2, temp2->arg1) == 0) {
						temp2->arg1 = row;
					}
					if(strcmp(temp->arg2, temp2->arg2) == 0) {
						temp2->arg2 = row;
					}
				}
				temp2 = temp2->next;
			}
			temp->arg2 = "";
		}
		temp = temp->next;
	}

	return 0;
}
 */

int backpatch() {
	assembler_line *temp = assembler_root;

	if(assembler_root == NULL) return 0;

	while(temp != NULL) {
		if(strcmp(temp->order, "") == 0) {
			char *row = to_char(temp->row);
			assembler_line *temp2 = assembler_root;
			while(temp2 != NULL) {
				if(strcmp(temp2->order, "") != 0) {
					if(strcmp(temp->arg2, temp2->arg1) == 0) {
						temp2->arg1 = row;
					}
					if(strcmp(temp->arg2, temp2->arg2) == 0) {
						temp2->arg2 = row;
					}
				}
				temp2 = temp2->next;
			}
			temp->arg2 = "";
		}
		temp = temp->next;
	}

	return 0;
}

int repair() {
	assembler_line *previous = assembler_root;
	assembler_line *temp = assembler_root;

	if(assembler_root == NULL) return 0;

	while(temp != NULL) {
			if(strcmp(temp->order, "") == 0 && strcmp(temp->arg1, "") == 0 && strcmp(temp->arg2, "") == 0) {
				unsigned long int row = temp->row;
				assembler_line *temp2 = assembler_root;

				while(temp2 != NULL) {
					if(temp2->row > row) temp2->row--;
					if(strcmp(temp2->order, "JUMP") == 0) {
						char *ptr;
						unsigned long int number;
						number = strtoul(temp2->arg1, &ptr, 10);
						if(number > row) {
							number--;
							temp2->arg1 = to_char(number);
						}
					}
					if(strcmp(temp2->order, "JZERO") == 0) {
						char *ptr;
						unsigned long int number;
						number = strtoul(temp2->arg2, &ptr, 10);
						if(number > row) {
							number--;
							temp2->arg2 = to_char(number);
						}
					}
					if(strcmp(temp2->order, "JODD") == 0) {
						char *ptr;
						unsigned long int number;
						number = strtoul(temp2->arg2, &ptr, 10);
						if(number > row) {
							number--;
							temp2->arg2 = to_char(number);
						}
					}
					temp2 = temp2->next;
				}
				temp->arg2 = "";
				//Usuniecie wiersza
				previous->next = temp->next;
			}


			previous = temp;
			temp = temp->next;
		}

		return 0;

}

int assembly() {
		if(fours_root == NULL) {
			add_row("HALT", "", "");
			return 0;
		}

		/* Jesli operator1 = ROW, to oznacza że mamy po prostu dodac to co jest w dalszej części
		 * wiersza do assemblera
		 */
		if(strcmp(fours_root->operator1, "ROW") == 0) {
			add_row(fours_root->arg1, fours_root->operator2, fours_root->arg2);
		}

		// Dodanie label
		if(strcmp(fours_root->operator1, "") == 0) {
			add_row("", "", fours_root->result);
		}

		if(strcmp(fours_root->operator1, "IFZ") == 0) {
			add_row("JZERO", "0", fours_root->arg2);
		}

		if(strcmp(fours_root->operator1, "GOTO") == 0) {
			add_row("JUMP", fours_root->arg1, "");
		}

		if(strcmp(fours_root->operator1, "TAB") == 0) {
			// Wygeneruj adres wyniku
			generate_number(find_symbol(fours_root->result)->address, "3");
			// Wygeneruj adres poczatku tablicy
			generate_number(find_symbol(fours_root->arg1)->address, "2");

			//Sprawdź, czy wskaźnik jest liczbą.
			if(is_numeric(fours_root->arg2) == 0) {
				// Wygeneruj adres zmiennej
				generate_number(find_symbol(fours_root->arg2)->address, "1");
				add_row("LOAD", "0", "1");
			}
			else {
				// Wygeneruj po prostu liczbę
				char *end;
				unsigned long int number = strtoul(fours_root->arg2, &end, 10);
				generate_number(number, "0");
			}

			// Dodaj wygenerowane wartości do adresu poczatku tablicy
			add_row("ADD", "2", "0");
			// Zapisz otrzymany adres w tablicy pod adresem result
			add_row("STORE", "2", "3");
		}


		if(strcmp(fours_root->operator1, "GET") == 0) {
			// Sprawdzenie, czy nie zapisujemy do tablicy
			if(strcmp(find_symbol(fours_root->arg1)->type, "ADDRESS") == 0) {
				generate_number(find_symbol(fours_root->arg1)->address, "1");
				add_row("LOAD", "0", "1");
			}
			else {
				generate_number(find_symbol(fours_root->arg1)->address, "0");
			}
			// W tym miejscu adres docelowy zapisu znajduje sie w rejestrze 0
			add_row("READ", "1", "");
			add_row("STORE", "1", "0");
		}

		if(strcmp(fours_root->operator1, "PUT") == 0) {
			if(is_numeric(fours_root->arg1) == 0) {
				// Sprawdzenie, czy czytamy z tablicy
				if(strcmp(find_symbol(fours_root->arg1)->type, "ADDRESS") == 0) {
					generate_number(find_symbol(fours_root->arg1)->address, "2");
					add_row("LOAD", "0", "2");
				}
				else {
					generate_number(find_symbol(fours_root->arg1)->address, "0");
				}
				add_row("LOAD", "1", "0");
			}
			else {
				// Wygeneruj po prostu liczbę
				char *end;
				unsigned long int number = strtoul(fours_root->arg1, &end, 10);
				generate_number(number, "1");
			}
			// W tym miejscu adres docelowy zapisu znajduje sie w rejestrze 0
			add_row("WRITE", "1", "");
		}

		if(strcmp(fours_root->operator1, "++") == 0) {
			// Wygeneruj adres iteratora
			generate_number(find_symbol(fours_root->arg1)->address, "1");
			// Załaduj wartośc iteratora do rejestru
			add_row("LOAD", "0", "1");
			// Zwieksz wartośc rejestru o 1
			add_row("INC", "0", "");
			// Zapisz wartośc pod adresem iteratora
			add_row("STORE", "0", "1");
		}

		if(strcmp(fours_root->operator1, "--") == 0) {
			// Wygeneruj adres iteratora
			generate_number(find_symbol(fours_root->arg1)->address, "1");
			// Załaduj wartośc iteratora do rejestru
			add_row("LOAD", "0", "1");
			// Zwieksz wartośc rejestru o 1
			add_row("DEC", "0", "");
			// Zapisz wartośc pod adresem iteratora
			add_row("STORE", "0", "1");
		}

		if(strcmp(fours_root->operator1, ":=") == 0) {
			// Sprawdzenie, czy zapisujemy do tablicy
			if(strcmp(find_symbol(fours_root->arg1)->type, "ADDRESS") == 0) {
				generate_number(find_symbol(fours_root->arg1)->address, "5");
				add_row("LOAD", "4", "5");
			}
			else {
				generate_number(find_symbol(fours_root->arg1)->address, "4");
			}

			//Sprawdzenie, czy drugi argument jest liczbą
			if(is_numeric(fours_root->arg2) == 0) {
				// Sprawdzenie, czy czytamy z tablicy
				if(strcmp(find_symbol(fours_root->arg2)->type, "ADDRESS") == 0) {
					generate_number(find_symbol(fours_root->arg2)->address, "3");
					add_row("LOAD", "2", "3");
					add_row("LOAD", "0", "2");
				}
				else {
				// Wygeneruj adres zmiennej i odczytaj z niej wartość
				generate_number(find_symbol(fours_root->arg2)->address, "1");
				add_row("LOAD", "0", "1");
				}
			}
			else {
				// Wygeneruj po prostu liczbę
				char *end;
				unsigned long int number = strtoul(fours_root->arg2, &end, 10);
				generate_number(number, "0");
			}

			add_row("STORE", "0", "4");
		}

		if(strcmp(fours_root->operator1, "+") == 0) {
			int forward = 0;
			int arg2_equals_1 = 0;


			//Sprawdzenie, czy pierwszy argument jest liczbą
			if(is_numeric(fours_root->arg1) == 0) {
				// Sprawdzenie, czy czytamy z tablicy
				if(strcmp(find_symbol(fours_root->arg1)->type, "ADDRESS") == 0) {
					generate_number(find_symbol(fours_root->arg1)->address, "5");
					add_row("LOAD", "4", "5");
					add_row("LOAD", "3", "4");
				}
				else {
				// Wygeneruj adres zmiennej i odczytaj z niej wartość
				generate_number(find_symbol(fours_root->arg1)->address, "4");
				add_row("LOAD", "3", "4");
				}
			}
			else {
				// Wygeneruj po prostu liczbę
				char *end;
				unsigned long int number = strtoul(fours_root->arg1, &end, 10);
				generate_number(number, "3");
			}


			//Sprawdzenie, czy pierwszy argument jest taki sam ja kdrugi
			if(strcmp(fours_root->arg1, fours_root->arg2) == 0) {
				add_row("COPY", "0", "3");
			}
			else {
				//Sprawdzenie, czy drugi argument jest liczbą
				if(is_numeric(fours_root->arg2) == 0) {
					// Sprawdzenie, czy czytamy z tablicy
					if(strcmp(find_symbol(fours_root->arg2)->type, "ADDRESS") == 0) {
						generate_number(find_symbol(fours_root->arg2)->address, "2");
						add_row("LOAD", "1", "2");
						add_row("LOAD", "0", "1");
					}
					else {
						// Wygeneruj adres zmiennej i odczytaj z niej wartość
						generate_number(find_symbol(fours_root->arg2)->address, "1");
						add_row("LOAD", "0", "1");
					}
				}
				else {
					// Sprawdzenie, czy drugi argument równa sie 1
					if(strcmp(fours_root->arg2, "1") == 0) {
						arg2_equals_1 = 1;
						add_row("INC", "3", "");
					}
					else {
						// Wygeneruj po prostu liczbę
						char *end;
						unsigned long int number = strtoul(fours_root->arg2, &end, 10);
						generate_number(number, "0");
					}
				}
			}

			/********Wygenerowanie adresu do zapisania********/

			/*Sprawdzenie, czy nastepna operacja jest operacja przypisania [DO POPRAWY]*/
			if(strcmp(fours_root->next->operator1, ":=") == 0) {
				/*Sprawdzenie, czy zmienna, do której bedziemy zapisywac nie została już wygenerowana\
				przy generowaniu adresów arg1 i arg2*/
				if(strcmp(fours_root->next->arg1,fours_root->arg1) == 0) {
					//Skopiuj arg1 do R6 (adresu zapisu)
					add_row("COPY", "6", "4");
				}
				else if(strcmp(fours_root->next->arg1,fours_root->arg2) == 0) {
					//Skopiuj arg1 do R6 (adresu zapisu)
					add_row("COPY", "6", "1");
				}
				else {
					// Sprawdzenie, czy zapisujemy do tablicy
					if(strcmp(find_symbol(fours_root->next->arg1)->type, "ADDRESS") == 0) {
						generate_number(find_symbol(fours_root->next->arg1)->address, "7");
						add_row("LOAD", "6", "7");
					}
					else {
						generate_number(find_symbol(fours_root->next->arg1)->address, "6");
					}
				}

				forward = 1;
			}
			else {

				// Sprawdzenie, czy zapisujemy do tablicy
				if(strcmp(find_symbol(fours_root->result)->type, "ADDRESS") == 0) {
					generate_number(find_symbol(fours_root->result)->address, "7");
					add_row("LOAD", "6", "7");
				}
				else {
					generate_number(find_symbol(fours_root->result)->address, "6");
				}
			}

			//Koncowe obliczenia

			if(arg2_equals_1 == 0) {
				add_row("ADD", "0", "3");
				add_row("STORE", "0", "6");
			}
			else {
				add_row("STORE", "3", "6");
			}




			if(forward == 1) {
				fours_root = fours_root->next;
			}

		}

		if(strcmp(fours_root->operator1, "-") == 0) {
			int forward = 0;

			//Sprawdzenie, czy pierwszy argument jest liczbą
			if(is_numeric(fours_root->arg1) == 0) {
				// Sprawdzenie, czy czytamy z tablicy
				if(strcmp(find_symbol(fours_root->arg1)->type, "ADDRESS") == 0) {
					generate_number(find_symbol(fours_root->arg1)->address, "2");
					add_row("LOAD", "1", "2");
					add_row("LOAD", "0", "1");
				}
				else {
				// Wygeneruj adres zmiennej i odczytaj z niej wartość
				generate_number(find_symbol(fours_root->arg1)->address, "1");
				add_row("LOAD", "0", "1");
				}
			}
			else {
				// Wygeneruj po prostu liczbę
				char *end;
				unsigned long int number = strtoul(fours_root->arg1, &end, 10);
				generate_number(number, "0");
			}


			//Sprawdzenie, czy drugi argument jest liczbą
			if(is_numeric(fours_root->arg2) == 0) {
				// Sprawdzenie, czy czytamy z tablicy
				if(strcmp(find_symbol(fours_root->arg2)->type, "ADDRESS") == 0) {
					generate_number(find_symbol(fours_root->arg2)->address, "5");
					add_row("LOAD", "4", "5");
					add_row("LOAD", "3", "4");
				}
				else {
					// Wygeneruj adres zmiennej i odczytaj z niej wartość
					generate_number(find_symbol(fours_root->arg2)->address, "4");
					add_row("LOAD", "3", "4");
				}
			}
			else {
				// Wygeneruj po prostu liczbę
				char *end;
				unsigned long int number = strtoul(fours_root->arg2, &end, 10);
				generate_number(number, "3");
			}


			/*Sprawdzenie, czy nastpena operacja jest operacja przypisania */
			if(strcmp(fours_root->next->operator1, ":=") == 0) {
					/*Sprawdzenie, czy zmienna, do której bedziemy zapisywac nie została już wygenerowana\
					przy generowaniu adresów arg1 i arg2*/
					if(strcmp(fours_root->next->arg1,fours_root->arg1) == 0) {
						//Skopiuj arg1 do R6 (adresu zapisu)
						add_row("COPY", "6", "1");
					}
					else if(strcmp(fours_root->next->arg1,fours_root->arg2) == 0) {
						//Skopiuj arg1 do R6 (adresu zapisu)
						add_row("COPY", "6", "4");
					}
					else {
						// Sprawdzenie, czy zapisujemy do tablicy
						if(strcmp(find_symbol(fours_root->next->arg1)->type, "ADDRESS") == 0) {
							generate_number(find_symbol(fours_root->next->arg1)->address, "7");
							add_row("LOAD", "6", "7");
						}
						else {
							generate_number(find_symbol(fours_root->next->arg1)->address, "6");
						}
					}

					forward = 1;
			}
			else {

				// Sprawdzenie, czy zapisujemy do tablicy
				if(strcmp(find_symbol(fours_root->result)->type, "ADDRESS") == 0) {
					generate_number(find_symbol(fours_root->result)->address, "7");
					add_row("LOAD", "6", "7");
				}
				else {
					generate_number(find_symbol(fours_root->result)->address, "6");
				}
			}

			add_row("SUB", "0", "3");

			add_row("STORE", "0", "6");


			if(forward == 1) {
				fours_root = fours_root->next;
			}

		}



		if(strcmp(fours_root->operator1, "*") == 0) {
			int forward = 0;

			//Sprawdzenie, czy pierwszy argument jest liczbą
			if(is_numeric(fours_root->arg1) == 0) {
				// Sprawdzenie, czy czytamy z tablicy
				if(strcmp(find_symbol(fours_root->arg1)->type, "ADDRESS") == 0) {
					generate_number(find_symbol(fours_root->arg1)->address, "5");
					add_row("LOAD", "4", "5");
					add_row("LOAD", "1", "4");
				}
				else {
				// Wygeneruj adres zmiennej i odczytaj z niej wartość
				generate_number(find_symbol(fours_root->arg1)->address, "4");
				add_row("LOAD", "1", "4");
				}
			}
			else {
				// Wygeneruj po prostu liczbę
				char *end;
				unsigned long int number = strtoul(fours_root->arg1, &end, 10);
				generate_number(number, "1");

			}


			//Sprawdzenie, czy pierwszy argument jest taki sam jak drugi
			if(strcmp(fours_root->arg1, fours_root->arg2) == 0) {
				add_row("COPY", "2", "1");
			}
			else {
				//Sprawdzenie, czy drugi argument jest liczbą
				if(is_numeric(fours_root->arg2) == 0) {
					// Sprawdzenie, czy czytamy z tablicy
					if(strcmp(find_symbol(fours_root->arg2)->type, "ADDRESS") == 0) {
						generate_number(find_symbol(fours_root->arg2)->address, "4");
						add_row("LOAD", "3", "4");
						add_row("LOAD", "2", "3");
					}
					else {
						// Wygeneruj adres zmiennej i odczytaj z niej wartość
						generate_number(find_symbol(fours_root->arg2)->address, "3");
						add_row("LOAD", "2", "3");
					}
				}
				else {
					// Wygeneruj po prostu liczbę
					char *end;
					unsigned long int number = strtoul(fours_root->arg2, &end, 10);
					generate_number(number, "2");
				}
			}

			/*Sprawdzenie, czy nastpena operacja jest operacja przypisania */
			if(strcmp(fours_root->next->operator1, ":=") == 0) {
				/*Sprawdzenie, czy zmienna, do której bedziemy zapisywac nie została już wygenerowana\
				przy generowaniu adresów arg1 i arg2*/
				if(strcmp(fours_root->next->arg1,fours_root->arg1) == 0) {
					//Skopiuj arg1 do R6 (adresu zapisu)
					add_row("COPY", "9", "4");
				}
				else if(strcmp(fours_root->next->arg1,fours_root->arg2) == 0) {
					//Skopiuj arg1 do R6 (adresu zapisu)
					add_row("COPY", "9", "3");
				}
				else {
					// Sprawdzenie, czy zapisujemy do tablicy
					if(strcmp(find_symbol(fours_root->next->arg1)->type, "ADDRESS") == 0) {
						generate_number(find_symbol(fours_root->next->arg1)->address, "7");
						add_row("LOAD", "9", "7");
					}
					else {
						generate_number(find_symbol(fours_root->next->arg1)->address, "9");
					}
				}

				forward = 1;
			}
			else {

				// Sprawdzenie, czy zapisujemy do tablicy
				if(strcmp(find_symbol(fours_root->result)->type, "ADDRESS") == 0) {
					generate_number(find_symbol(fours_root->result)->address, "7");
					add_row("LOAD", "9", "7");
				}
				else {
					generate_number(find_symbol(fours_root->result)->address, "9");
				}
			}

			//Wypisz pobrane liczby
			//add_row("WRITE", "1", "");
			//add_row("WRITE", "2", "");

			char *LZ = generate_new_label();
			char *LX = generate_new_label();
			char *LY = generate_new_label();
			//char *LF = generate_new_label();


			// RESULT bedzie przechowywany w R0
			// ARG1 będzie w R1
			// ARG2 będzie w R2
			add_row("JZERO", "2", to_char(k+2));
			add_row("JUMP", to_char(k+3), "");
			add_row("RESET", "0", "");
			add_row("JUMP", LY, "");


			add_row("RESET", "0", "");
			add_row("","", LX);
			/*add_row("WRITE", "0", "");
			add_row("WRITE", "1", "");
			add_row("WRITE", "2", "");*/
			//add_row("COPY", "8", "2");
			generate_number(1, "3");
			add_row("SUB", "3", "2");
			add_row("JZERO", "3", to_char(k+2)); // Mnożnik mniejszy niz jeden -> zakoncz
			add_row("JUMP", LY, "");
			//add_row("COPY", "2", "8"); // Przywróć mnozikowi stara wartość
			add_row("JODD", "2", to_char(k+2)); // Jeśli mnoznik nieparzysty result += mnozna
			add_row("JUMP", to_char(k+2), "");//---- Trzeba odjąc 2 !!!! ↓ dodane WRITe i COPY
			add_row("ADD", "0", "1");
			//add_row("WRITE", "0", "");
			// SHL(mnozna)
			add_row("SHL", "1", "");
			// SHR(mnoznik)
			add_row("SHR", "2", "");
			add_row("JUMP", LX, "");
			add_row("","",LY);

			// Zapisz wynik
			add_row("STORE", "0", "9");

			if(forward == 1) {
				fours_root = fours_root->next;
			}
		}


		if(strcmp(fours_root->operator1, "/") == 0) {
			int forward = 0;
			int skip_calculations = 0;

			//Sprawdzenie, czy pierwszy argument jest liczbą
			if(is_numeric(fours_root->arg1) == 0) {
				// Sprawdzenie, czy czytamy z tablicy
				if(strcmp(find_symbol(fours_root->arg1)->type, "ADDRESS") == 0) {
					generate_number(find_symbol(fours_root->arg1)->address, "5");
					add_row("LOAD", "4", "5");
					add_row("LOAD", "1", "4");
				}
				else {
				// Wygeneruj adres zmiennej i odczytaj z niej wartość
				generate_number(find_symbol(fours_root->arg1)->address, "4");
				add_row("LOAD", "1", "4");
				}
			}
			else {
				// Wygeneruj po prostu liczbę
				char *end;
				unsigned long int number = strtoul(fours_root->arg1, &end, 10);
				generate_number(number, "1");
			}

			//Sprawdzenie, czy pierwszy argument jest taki sam ja kdrugi
			if(strcmp(fours_root->arg1, fours_root->arg2) == 0) {
				add_row("COPY", "2", "1");
			}
			else {
				//Sprawdzenie, czy drugi argument jest liczbą
				if(is_numeric(fours_root->arg2) == 0) {
					// Sprawdzenie, czy czytamy z tablicy
					if(strcmp(find_symbol(fours_root->arg2)->type, "ADDRESS") == 0) {
						generate_number(find_symbol(fours_root->arg2)->address, "4");
						add_row("LOAD", "3", "4");
						add_row("LOAD", "2", "3");
					}
					else {
						// Wygeneruj adres zmiennej i odczytaj z niej wartość
						generate_number(find_symbol(fours_root->arg2)->address, "3");
						add_row("LOAD", "2", "3");
					}
				}
				else {
					// Wygeneruj po prostu liczbę
					char *end;
					unsigned long int number = strtoul(fours_root->arg2, &end, 10);
					if(number != 0) {
						generate_number(number, "2");
					}
					else {
						add_row("RESET", "0", "");
						skip_calculations = 1;
					}
				}
			}

			/*Sprawdzenie, czy nastpena operacja jest operacja przypisania */
			if(strcmp(fours_root->next->operator1, ":=") == 0) {
				/*Sprawdzenie, czy zmienna, do której bedziemy zapisywac nie została już wygenerowana\
				przy generowaniu adresów arg1 i arg2*/
				if(strcmp(fours_root->next->arg1,fours_root->arg1) == 0) {
					//Skopiuj arg1 do R6 (adresu zapisu)
					add_row("COPY", "9", "4");
				}
				else if(strcmp(fours_root->next->arg1,fours_root->arg2) == 0) {
					//Skopiuj arg1 do R6 (adresu zapisu)
					add_row("COPY", "9", "3");
				}
				else {
					// Sprawdzenie, czy zapisujemy do tablicy
					if(strcmp(find_symbol(fours_root->next->arg1)->type, "ADDRESS") == 0) {
						generate_number(find_symbol(fours_root->next->arg1)->address, "7");
						add_row("LOAD", "9", "7");
					}
					else {
						generate_number(find_symbol(fours_root->next->arg1)->address, "9");
					}
				}

				forward = 1;
			}
			else {

				// Sprawdzenie, czy zapisujemy do tablicy
				if(strcmp(find_symbol(fours_root->result)->type, "ADDRESS") == 0) {
					generate_number(find_symbol(fours_root->result)->address, "7");
					add_row("LOAD", "9", "7");
				}
				else {
					generate_number(find_symbol(fours_root->result)->address, "9");
				}
			}

			if(skip_calculations == 0) {

			char *LZ = generate_new_label();
			char *LX = generate_new_label();
			char *LY = generate_new_label();
			char *LF = generate_new_label();

			// RESULT bedzie przechowywany w R0
			// ARG1 będzie w R1
			// ARG2 będzie w R2
			add_row("JZERO", "2", to_char(k+2));
			add_row("JUMP", to_char(k+3), "");
			add_row("RESET", "0", "");
			add_row("JUMP", LZ, "");

			add_row("RESET", "0", ""); // Ustaw quotient na 0
			add_row("COPY", "3", "1"); // remainder = dividend
			add_row("COPY", "4", "2"); // multiple = divisor


			add_row("","", LX);
			add_row("COPY", "8", "4");
			add_row("SUB", "4", "3");
			add_row("JZERO", "4", to_char(k+2)); // while (multiiple <= remainder) {...
			add_row("JUMP", LY, "");
			add_row("COPY", "4", "8"); // Przywróć mnozikowi stara wartość
			add_row("SHL", "4", "");
			add_row("JUMP", LX, "");
			add_row("", "", LY);

			add_row("COPY", "4", "8");
			add_row("SHR", "4", "");
			//add_row("WRITE", "4", "");



			add_row("COPY", "8", "2");
			add_row("SUB", "2", "4");
			add_row("JZERO", "2", to_char(k+2)); // while (multiiple >= divisor) {...
			add_row("JUMP", LZ, "");
			add_row("COPY", "2", "8");
			add_row("SHL", "0", "");


			add_row("COPY", "8", "4");
			add_row("SUB", "4", "3");
			add_row("JZERO", "4", to_char(k+2)); // if (multiiple <= divisor) {...
			add_row("JUMP", LF, "");
			add_row("COPY", "4", "8");
			add_row("SUB", "3", "4"); // remainder = remainder - multiple
			add_row("INC", "0", "");

			add_row("", "", LF);
			add_row("SHR", "4", ""); //SHR(multiple)
			add_row("JUMP", LY, "");


			add_row("","",LZ);

		}

			// Zapisz wynik
			add_row("STORE", "0", "9");

			if(forward == 1) {
				fours_root = fours_root->next;
			}
		}


		if(strcmp(fours_root->operator1, "%") == 0) {
			int forward = 0;
			int skip_calculations = 0;

			//Sprawdzenie, czy pierwszy argument jest liczbą
			if(is_numeric(fours_root->arg1) == 0) {
				// Sprawdzenie, czy czytamy z tablicy
				if(strcmp(find_symbol(fours_root->arg1)->type, "ADDRESS") == 0) {
					generate_number(find_symbol(fours_root->arg1)->address, "5");
					add_row("LOAD", "4", "5");
					add_row("LOAD", "1", "4");
				}
				else {
				// Wygeneruj adres zmiennej i odczytaj z niej wartość
				generate_number(find_symbol(fours_root->arg1)->address, "4");
				add_row("LOAD", "1", "4");
				}
			}
			else {
				// Wygeneruj po prostu liczbę
				char *end;
				unsigned long int number = strtoul(fours_root->arg1, &end, 10);
				generate_number(number, "1");
			}



			//Sprawdzenie, czy pierwszy argument jest taki sam ja kdrugi
			if(strcmp(fours_root->arg1, fours_root->arg2) == 0) {
				add_row("COPY", "2", "1");
			}
			else {
				//Sprawdzenie, czy drugi argument jest liczbą
				if(is_numeric(fours_root->arg2) == 0) {
					// Sprawdzenie, czy czytamy z tablicy
					if(strcmp(find_symbol(fours_root->arg2)->type, "ADDRESS") == 0) {
						generate_number(find_symbol(fours_root->arg2)->address, "4");
						add_row("LOAD", "3", "4");
						add_row("LOAD", "2", "3");
					}
					else {
						// Wygeneruj adres zmiennej i odczytaj z niej wartość
						generate_number(find_symbol(fours_root->arg2)->address, "3");
						add_row("LOAD", "2", "3");
					}
				}
				else {
					// Wygeneruj po prostu liczbę
					char *end;
					unsigned long int number = strtoul(fours_root->arg2, &end, 10);
					if(number != 0) {
						generate_number(number, "2");
					}
					else {
						add_row("RESET", "0", "");
						skip_calculations = 1;
					}
				}
			}


			/*Sprawdzenie, czy nastpena operacja jest operacja przypisania */
			if(strcmp(fours_root->next->operator1, ":=") == 0) {
				/*Sprawdzenie, czy zmienna, do której bedziemy zapisywac nie została już wygenerowana\
				przy generowaniu adresów arg1 i arg2*/
				if(strcmp(fours_root->next->arg1,fours_root->arg1) == 0) {
					//Skopiuj arg1 do R6 (adresu zapisu)
					add_row("COPY", "9", "4");
				}
				else if(strcmp(fours_root->next->arg1,fours_root->arg2) == 0) {
					//Skopiuj arg1 do R6 (adresu zapisu)
					add_row("COPY", "9", "3");
				}
				else {
					// Sprawdzenie, czy zapisujemy do tablicy
					if(strcmp(find_symbol(fours_root->next->arg1)->type, "ADDRESS") == 0) {
						generate_number(find_symbol(fours_root->next->arg1)->address, "7");
						add_row("LOAD", "9", "7");
					}
					else {
						generate_number(find_symbol(fours_root->next->arg1)->address, "9");
					}
				}

				forward = 1;
			}
			else {

				// Sprawdzenie, czy zapisujemy do tablicy
				if(strcmp(find_symbol(fours_root->result)->type, "ADDRESS") == 0) {
					generate_number(find_symbol(fours_root->result)->address, "7");
					add_row("LOAD", "9", "7");
				}
				else {
					generate_number(find_symbol(fours_root->result)->address, "9");
				}
			}

			if(skip_calculations == 0) {

			char *LZ = generate_new_label();
			char *LX = generate_new_label();
			char *LY = generate_new_label();
			char *LF = generate_new_label();

			// RESULT bedzie przechowywany w R0
			// ARG1 będzie w R1
			// ARG2 będzie w R2
			add_row("JZERO", "2", to_char(k+2));
			add_row("JUMP", to_char(k+3), "");
			add_row("RESET", "0", "");
			add_row("JUMP", LZ, "");

			add_row("RESET", "0", ""); // Ustaw quotient na 0
			add_row("COPY", "3", "1"); // remainder = dividend
			add_row("COPY", "4", "2"); // multiple = divisor


			add_row("","", LX);
			add_row("COPY", "8", "4");
			add_row("SUB", "4", "3");
			add_row("JZERO", "4", to_char(k+2)); // while (multiiple <= remainder) {...
			add_row("JUMP", LY, "");
			add_row("COPY", "4", "8"); // Przywróć mnozikowi stara wartość
			add_row("SHL", "4", "");
			add_row("JUMP", LX, "");
			add_row("", "", LY);

			add_row("COPY", "4", "8");
			add_row("SHR", "4", "");
			//add_row("WRITE", "4", "");



			add_row("COPY", "8", "2");
			add_row("SUB", "2", "4");
			add_row("JZERO", "2", to_char(k+2)); // while (multiiple >= divisor) {...
			add_row("JUMP", LZ, "");
			add_row("COPY", "2", "8");
			add_row("SHL", "0", "");


			add_row("COPY", "8", "4");
			add_row("SUB", "4", "3");
			add_row("JZERO", "4", to_char(k+2)); // if (multiiple <= divisor) {...
			add_row("JUMP", LF, "");
			add_row("COPY", "4", "8");
			add_row("SUB", "3", "4"); // remainder = remainder - multiple
			add_row("INC", "0", "");

			add_row("", "", LF);
			add_row("SHR", "4", ""); //SHR(multiple)
			add_row("JUMP", LY, "");


			add_row("","",LZ);
			add_row("COPY", "0", "3");

			}
			// Zapisz wynik
			add_row("STORE", "0", "9");

			if(forward == 1) {
				fours_root = fours_root->next;
			}
		}


		if(strcmp(fours_root->operator1, "=") == 0) {
			//Sprawdzenie, czy pierwszy argument jest liczbą
			if(is_numeric(fours_root->arg1) == 0) {
				// Sprawdzenie, czy czytamy z tablicy
				if(strcmp(find_symbol(fours_root->arg1)->type, "ADDRESS") == 0) {
					generate_number(find_symbol(fours_root->arg1)->address, "2");
					add_row("LOAD", "1", "2");
					add_row("LOAD", "0", "1");
				}
				else {
				// Wygeneruj adres zmiennej i odczytaj z niej wartość
				generate_number(find_symbol(fours_root->arg1)->address, "1");
				add_row("LOAD", "0", "1");
				}
			}
			else {
				// Wygeneruj po prostu liczbę
				char *end;
				unsigned long int number = strtoul(fours_root->arg1, &end, 10);
				generate_number(number, "0");
			}
			add_row("COPY", "6","0");

			//Sprawdzenie, czy drugi argument jest liczbą
			if(is_numeric(fours_root->arg2) == 0) {
				// Sprawdzenie, czy czytamy z tablicy
				if(strcmp(find_symbol(fours_root->arg2)->type, "ADDRESS") == 0) {
					generate_number(find_symbol(fours_root->arg2)->address, "5");
					add_row("LOAD", "4", "5");
					add_row("LOAD", "3", "4");
				}
				else {
				// Wygeneruj adres zmiennej i odczytaj z niej wartość
				generate_number(find_symbol(fours_root->arg2)->address, "4");
				add_row("LOAD", "3", "4");
				}
			}
			else {
				// Wygeneruj po prostu liczbę
				char *end;
				unsigned long int number = strtoul(fours_root->arg2, &end, 10);
				generate_number(number, "3");
			}

			add_row("SUB", "0", "3");
			// Jesli R0 = 0, to skocz do LO
			add_row("JZERO", "0", to_char(k+3));
			add_row("RESET", "0", "");

			add_row("JUMP", to_char(k+6), ""); // Przeskocz na sam koniec do L2
			// LO    - odejmij B - A
			add_row("SUB", "3", "6");
			// Jeśli wynik równy zero, to skocz do L1 i zwiększ R0
			add_row("JZERO", "3", to_char(k+3));
			add_row("RESET", "0", "");

			add_row("JUMP" , to_char(k+2), ""); // Przeskocz do L2 (omiń zwiększanie R0)
			// L1
			add_row("INC", "0", "");
			// L2
		}

		if(strcmp(fours_root->operator1, "!=") == 0) {
			//Sprawdzenie, czy pierwszy argument jest liczbą
			if(is_numeric(fours_root->arg1) == 0) {
				// Sprawdzenie, czy czytamy z tablicy
				if(strcmp(find_symbol(fours_root->arg1)->type, "ADDRESS") == 0) {
					generate_number(find_symbol(fours_root->arg1)->address, "2");
					add_row("LOAD", "1", "2");
					add_row("LOAD", "0", "1");
				}
				else {
				// Wygeneruj adres zmiennej i odczytaj z niej wartość
				generate_number(find_symbol(fours_root->arg1)->address, "1");
				add_row("LOAD", "0", "1");
				}
			}
			else {
				// Wygeneruj po prostu liczbę
				char *end;
				unsigned long int number = strtoul(fours_root->arg1, &end, 10);
				generate_number(number, "0");
			}
			add_row("COPY", "6","0");

			//Sprawdzenie, czy drugi argument jest liczbą
			if(is_numeric(fours_root->arg2) == 0) {
				// Sprawdzenie, czy czytamy z tablicy
				if(strcmp(find_symbol(fours_root->arg2)->type, "ADDRESS") == 0) {
					generate_number(find_symbol(fours_root->arg2)->address, "5");
					add_row("LOAD", "4", "5");
					add_row("LOAD", "3", "4");
				}
				else {
				// Wygeneruj adres zmiennej i odczytaj z niej wartość
				generate_number(find_symbol(fours_root->arg2)->address, "4");
				add_row("LOAD", "3", "4");
				}
			}
			else {
				// Wygeneruj po prostu liczbę
				char *end;
				unsigned long int number = strtoul(fours_root->arg2, &end, 10);
				generate_number(number, "3");
			}

			add_row("SUB", "0", "3");
			// Jesli R0 = 0, to skocz do LO
			add_row("JZERO", "0", to_char(k+3));
			add_row("INC", "0", "");

			add_row("JUMP", to_char(k+6), ""); // Przeskocz na sam koniec do L2
			// LO    - odejmij B - A
			add_row("SUB", "3", "6");
			// Jeśli wynik równy zero, to skocz do L1 i resetuj R0
			add_row("JZERO", "3", to_char(k+3));
			add_row("INC", "0", "");

			add_row("JUMP" , to_char(k+2), ""); // Przeskocz do L2 (omiń resetowanie R0)
			// L1
			add_row("RESET", "0", "");
			// L2
		}


		if(strcmp(fours_root->operator1, "<") == 0) {
			//Sprawdzenie, czy pierwszy argument jest liczbą
			if(is_numeric(fours_root->arg1) == 0) {
				// Sprawdzenie, czy czytamy z tablicy
				if(strcmp(find_symbol(fours_root->arg1)->type, "ADDRESS") == 0) {
					generate_number(find_symbol(fours_root->arg1)->address, "2");
					add_row("LOAD", "1", "2");
					add_row("LOAD", "0", "1");
				}
				else {
				// Wygeneruj adres zmiennej i odczytaj z niej wartość
				generate_number(find_symbol(fours_root->arg1)->address, "1");
				add_row("LOAD", "0", "1");
				}
			}
			else {
				// Wygeneruj po prostu liczbę
				char *end;
				unsigned long int number = strtoul(fours_root->arg1, &end, 10);
				generate_number(number, "0");
			}

			//Sprawdzenie, czy drugi argument jest liczbą
			if(is_numeric(fours_root->arg2) == 0) {
				// Sprawdzenie, czy czytamy z tablicy
				if(strcmp(find_symbol(fours_root->arg2)->type, "ADDRESS") == 0) {
					generate_number(find_symbol(fours_root->arg2)->address, "5");
					add_row("LOAD", "4", "5");
					add_row("LOAD", "3", "4");
				}
				else {
				// Wygeneruj adres zmiennej i odczytaj z niej wartość
				generate_number(find_symbol(fours_root->arg2)->address, "4");
				add_row("LOAD", "3", "4");
				}
			}
			else {
				// Wygeneruj po prostu liczbę
				char *end;
				unsigned long int number = strtoul(fours_root->arg2, &end, 10);
				generate_number(number, "3");
			}

			add_row("SUB", "3", "0");
			// Jesli R0 = 0, to skocz do LO
			add_row("JZERO", "3", to_char(k+3));
			add_row("INC", "0", "");

			add_row("JUMP", to_char(k+2), ""); // Przeskocz na sam koniec do L2
			// LO    - odejmij B - A
			add_row("RESET", "0", "");
			// L2
		}



		if(strcmp(fours_root->operator1, ">") == 0) {
			//Sprawdzenie, czy pierwszy argument jest liczbą
			if(is_numeric(fours_root->arg1) == 0) {
				// Sprawdzenie, czy czytamy z tablicy
				if(strcmp(find_symbol(fours_root->arg1)->type, "ADDRESS") == 0) {
					generate_number(find_symbol(fours_root->arg1)->address, "2");
					add_row("LOAD", "1", "2");
					add_row("LOAD", "0", "1");
				}
				else {
				// Wygeneruj adres zmiennej i odczytaj z niej wartość
				generate_number(find_symbol(fours_root->arg1)->address, "1");
				add_row("LOAD", "0", "1");
				}
			}
			else {
				// Wygeneruj po prostu liczbę
				char *end;
				unsigned long int number = strtoul(fours_root->arg1, &end, 10);
				generate_number(number, "0");
			}

			//Sprawdzenie, czy drugi argument jest liczbą
			if(is_numeric(fours_root->arg2) == 0) {
				// Sprawdzenie, czy czytamy z tablicy
				if(strcmp(find_symbol(fours_root->arg2)->type, "ADDRESS") == 0) {
					generate_number(find_symbol(fours_root->arg2)->address, "5");
					add_row("LOAD", "4", "5");
					add_row("LOAD", "3", "4");
				}
				else {
				// Wygeneruj adres zmiennej i odczytaj z niej wartość
				generate_number(find_symbol(fours_root->arg2)->address, "4");
				add_row("LOAD", "3", "4");
				}
			}
			else {
				// Wygeneruj po prostu liczbę
				char *end;
				unsigned long int number = strtoul(fours_root->arg2, &end, 10);
				generate_number(number, "3");
			}

			add_row("SUB", "0", "3");
			// Jesli R0 = 0, to skocz do LO
			add_row("JZERO", "0", to_char(k+3));
			add_row("INC", "0", "");

			add_row("JUMP", to_char(k+2), ""); // Przeskocz na sam koniec do L2
			// LO    - odejmij B - A
			add_row("RESET", "0", "");
			// L2
		}

		if(strcmp(fours_root->operator1, "<=") == 0) {
			//Sprawdzenie, czy pierwszy argument jest liczbą
			if(is_numeric(fours_root->arg1) == 0) {
				// Sprawdzenie, czy czytamy z tablicy
				if(strcmp(find_symbol(fours_root->arg1)->type, "ADDRESS") == 0) {
					generate_number(find_symbol(fours_root->arg1)->address, "2");
					add_row("LOAD", "1", "2");
					add_row("LOAD", "0", "1");
				}
				else {
				// Wygeneruj adres zmiennej i odczytaj z niej wartość
				generate_number(find_symbol(fours_root->arg1)->address, "1");
				add_row("LOAD", "0", "1");
				}
			}
			else {
				// Wygeneruj po prostu liczbę
				char *end;
				unsigned long int number = strtoul(fours_root->arg1, &end, 10);
				generate_number(number, "0");
			}

			//Sprawdzenie, czy drugi argument jest liczbą
			if(is_numeric(fours_root->arg2) == 0) {
				// Sprawdzenie, czy czytamy z tablicy
				if(strcmp(find_symbol(fours_root->arg2)->type, "ADDRESS") == 0) {
					generate_number(find_symbol(fours_root->arg2)->address, "5");
					add_row("LOAD", "4", "5");
					add_row("LOAD", "3", "4");
				}
				else {
				// Wygeneruj adres zmiennej i odczytaj z niej wartość
				generate_number(find_symbol(fours_root->arg2)->address, "4");
				add_row("LOAD", "3", "4");
				}
			}
			else {
				// Wygeneruj po prostu liczbę
				char *end;
				unsigned long int number = strtoul(fours_root->arg2, &end, 10);
				generate_number(number, "3");
			}

			add_row("SUB", "0", "3");
			// Jesli R0 = 0, to skocz do LO
			add_row("JZERO", "0", to_char(k+3));
			add_row("RESET", "0", "");

			add_row("JUMP", to_char(k+2), ""); // Przeskocz na sam koniec do L2
			// LO    - odejmij B - A
			add_row("INC", "0", "");
			// L2
		}

		if(strcmp(fours_root->operator1, ">=") == 0) {
			//Sprawdzenie, czy pierwszy argument jest liczbą
			if(is_numeric(fours_root->arg1) == 0) {
				// Sprawdzenie, czy czytamy z tablicy
				if(strcmp(find_symbol(fours_root->arg1)->type, "ADDRESS") == 0) {
					generate_number(find_symbol(fours_root->arg1)->address, "2");
					add_row("LOAD", "1", "2");
					add_row("LOAD", "0", "1");
				}
				else {
				// Wygeneruj adres zmiennej i odczytaj z niej wartość
				generate_number(find_symbol(fours_root->arg1)->address, "1");
				add_row("LOAD", "0", "1");
				}
			}
			else {
				// Wygeneruj po prostu liczbę
				char *end;
				unsigned long int number = strtoul(fours_root->arg1, &end, 10);
				generate_number(number, "0");
			}

			//Sprawdzenie, czy drugi argument jest liczbą
			if(is_numeric(fours_root->arg2) == 0) {
				// Sprawdzenie, czy czytamy z tablicy
				if(strcmp(find_symbol(fours_root->arg2)->type, "ADDRESS") == 0) {
					generate_number(find_symbol(fours_root->arg2)->address, "5");
					add_row("LOAD", "4", "5");
					add_row("LOAD", "3", "4");
				}
				else {
				// Wygeneruj adres zmiennej i odczytaj z niej wartość
				generate_number(find_symbol(fours_root->arg2)->address, "4");
				add_row("LOAD", "3", "4");
				}
			}
			else {
				// Wygeneruj po prostu liczbę
				char *end;
				unsigned long int number = strtoul(fours_root->arg2, &end, 10);
				generate_number(number, "3");
			}

			add_row("SUB", "3", "0");
			// Jesli R0 = 0, to skocz do LO
			add_row("JZERO", "3", to_char(k+3));
			add_row("RESET", "0", "");

			add_row("JUMP", to_char(k+2), ""); // Przeskocz na sam koniec do L2
			// LO    - odejmij B - A
			add_row("INC", "0", "");
			// L2
		}





		fours_root = fours_root->next;
		assembly();


		return 0;
}

