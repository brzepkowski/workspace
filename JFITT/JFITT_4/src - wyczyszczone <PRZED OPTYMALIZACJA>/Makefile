all: compiler

compiler.tab.c compiler.tab.h:	compiler.y
	bison -d compiler.y

lex.yy.c: compiler.l compiler.tab.h
	flex compiler.l

compiler: lex.yy.c compiler.tab.c compiler.tab.h main.c
	gcc -o compiler compiler.tab.c lex.yy.c main.h main.c -lfl
	rm compiler.tab.c lex.yy.c compiler.tab.h
