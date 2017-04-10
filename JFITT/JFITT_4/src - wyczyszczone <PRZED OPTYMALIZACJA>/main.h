#ifndef MAIN_H
#define MAIN_H

typedef enum { false, true } bool;

/****** POMOCNICZE ZMIENNE ********/
int new_address_counter; // Pozwala na tworzenie nowych adresów to tablicy symboli (np. t1, t2...)
int new_label_counter; // Pozwala na tworzenie nowych etykiet do kodu trójadresowego (np. L0 ...)
int new_table_address_counter; // Pozwala na tworzenie nowych adresów, w których beda zapamiętywane \
										adresy z tablic
unsigned long int address_counter; // Pozwala na przypisywanie miejsca w pamięci nowo utworzonym zmiennym

int number_of_programs; // Pozwala na rozpoznanie, ile jest zagnieżdżonych programow

unsigned long int k;

/****** DRZEWO AST ****************/

// Struktura węzła w drzewie AST
typedef struct node {
	char *string;
	char *type;
	int first_line;
	int last_line;
	int first_column;
	int last_column;

	int number_of_children;
	struct node *children[];
} node;

// Korzen drzewa AST
static node *AST_root;


/******* LISTA BŁĘDÓW *******/

// Struktura błędu
typedef struct error {
	char *string;
	int first_line;
	int last_line;
	int first_column;
	int last_column;

	struct error *next;
} error;

/******* Korzen listy błędów *******/
static error *errors_list_root;

/****** Contains_errors ***********/
int contains_errors; // Ustawione na 1, jesli program zawiera błędy i 0, jesli nie zawiera

/****** LISTA SYMBOLI *************/

/*	Struktura symbolu, który będzie przechowywany w liście symboli.
 * 	address - miejsce w pamięci, w którym znajduje się identyfikator,
 * 	string - nazwa identyfikatora.
 */
typedef struct symbol {
	char *string;
	char *type;
	unsigned long int address;
	unsigned long int size;

	int first_line;
	int first_column;
	int last_line;
	int last_column;

	struct symbol *next;
} symbol;


typedef struct symbols_list_element {
	symbol *data;
	struct symbols_list_element *next;

} symbols_list;

/******** Korzeń listy symboli *******/
static symbols_list *symbols_list_root;

/* Struktura zainicjowanych symboli */
typedef struct initialized_symbol {
	char *string;
	bool initialized;
	struct initialized_symbol *next;
} initialized_symbol;

typedef struct initialized_symbols_list_element {
	initialized_symbol *data;
	struct initialized_symbols_list_element *next;
} initialized_symbols_list;

/******** Korzen listy zainicjowanych symboli****/
static initialized_symbols_list *initialized_symbols_list_root;


/************* CZWÓRKI ***************/

/*	Struktura jednej czwórki, która zostanie użyta w tablicy czwórek.
 * 	operator - operator w działaniu,
 * 	arg1, arg2, result - nazwa zmiennej w tablicy symboli, w której będzie się znajdował argument/wynik.
*/
typedef struct four {
	char *operator1;
	char *arg1;
	char *operator2;
	char *arg2;
	char *result;

	struct four *next;
} four;

/******** Korzeń listy czwórek ********/
static four *fours_root;



/************* ASSEMBLER **************/

typedef struct assembler_line {
	unsigned long int row;
	char *order;
	char *arg1;
	char *arg2;

	struct assembler_line *next;
} assembler_line;

/*** Korzeń listy poleceń assemblera ****/
static assembler_line *assembler_root;


/************ FUNKCJE *****************/
// Drzewo AST
node *add_node(int number_of_children, char *string, node *children[]);
void add_child(char *operation, node *parent, node *child);
void free_node(node *p);
void post_order(node *p);

// Błędy
error *create_error(char *string, int first_line, int first_column, int last_line, int last_column);
void add_error(error *x);
void remove_error(error *x);
int find_error(char *searched_string, int first_line);
void print_errors();

// Lista symboli
symbol *create_symbol(char *string, char *type, int address, int size, int first_line,
		int first_column, int last_line, int last_column);
void add_symbol(symbol *symbol);
void add_symbols_sublist();
void remove_symbols_sublist();
int search_for_symbol(char *symbol_name);
int check_if_symbol_already_declared(char *symbol_name);
symbol *find_symbol(char *symbol_name);
void print_symbols();

// Lista zainicjowanych symboli
void initialize(char *symbol_name);
void add_initialized_symbols_sublist();
void remove_initialized_symbols_sublist();
int check_if_symbol_was_initialized(char *symbol_name);
void print_initialized_symbols();

// Kod trójadresowy
char *generate_new_label();
void add_four(char *operator1, char *arg1, char *operator2, char *arg2, char *result);
void print_fours();
void translate(node *p);

// Assembler
int backpatch();
void print_assembler();

#endif
