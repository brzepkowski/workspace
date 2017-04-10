%{
#include <math.h>
#include <stdbool.h>
int stack[100];
int pointer = 0;
int solution = 0;
bool too_few_arguments = false;
bool bad_sign = false;
bool invalid_number = false;

void push(int number) {
	if (pointer < 100) {
		//printf("Umieszczono na stosie: %d pod pointer = %d\n", number, pointer);
		stack[pointer] = number;
		pointer++;
	}
	else printf("Przekroczono rozmiar stosu\n");
}

int pop() {
	if (pointer != 0) {
		pointer--;
		return stack[pointer];
	}
	else {
		too_few_arguments = true;
		return INFINITY;
	}
}	

int power(int x, int y) {
	int final = 1;
	if (y < 0) return INFINITY;
	else if (y == 0) return 1;
	else {
		while (y > 0) {
			final = final * x;
			y--;
		}
		return final;
	}
}
%}
DIGIT 		[0-9]
OPERATOR	"+"|"-"|"*"|"^"|"/"|"%"
%%
{DIGIT}*	{
		int x = atoi(yytext);
		push(x);
		}
"-"{DIGIT}+	{
		int x = atoi(yytext);
		push(x);
		}
"+"		{
		int x = pop();
		int y = pop();
		push(y+x);
		}
"-"		{
		int x = pop();
		int y = pop();
		push(y-x);
		}
"*"		{
		int x = pop();
		int y = pop();
		push(y*x);
		}
"/"		{
		int x = pop();
		int y = pop();
		if (x != 0) push(y/x);
		else {
			invalid_number = true;
		}
		}
"^"		{
		int x = pop();
		int y = pop();
		push(power(y,x));
		}
"%"		{
		int x = pop();
		int y = pop();
		if (x != 0) push(y/x);
		else {
			invalid_number = true;
		}
		}
\n		{
		if (pointer > 1) printf("Błąd: za mała liczba operatorów\n");
		else if (too_few_arguments == true) {
			printf("Błąd: za mała liczba argumentów\n");
			too_few_arguments = false;
		}
		else if (bad_sign == true) {
			bad_sign = false;
		}
		else if (invalid_number == true) {
			printf("Błąd: dzielenie przez 0\n");
			invalid_number = false;
		}
		else printf("= %d\n", stack[pointer-1]);
		pointer = 0;
		}
"-"\n		{
		int x = pop();
		int y = pop();
		push(y-x);
		if (pointer > 1) printf("Błąd: za mała liczba operatorów\n");
		else if (too_few_arguments == true) {
			printf("Błąd: za mała liczba argumentów\n");
			too_few_arguments = false;
		}
		else if (invalid_number == true) {
			printf("Błąd: dzielenie przez 0\n");
			invalid_number = false;
		}
		else printf("= %d\n", stack[pointer-1]);
		pointer = 0;
		}
[^{DIGIT}{OPERATOR} \n]    {printf("Błąd: zły znak: '%c'\n",yytext[0]);
				bad_sign = true;
		}

%%


int main() {
	yylex();
}
