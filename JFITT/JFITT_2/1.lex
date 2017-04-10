	int num_lines = 0, num_words = 0;
%%	
\n		++num_lines, fprintf(yyout, "\n");
^[ \t]+		/* ignore this token */
[ \t]+$       	/* ignore this token */
\n[ \t]+$	/* ignore this token */
[ \t]+        	fprintf(yyout, "%c", ' ');
[\n]{2,}	fprintf(yyout, "%c", '\n'), ++num_lines;

[^ \t\n]+	fprintf(yyout, "%s",yytext);++num_words;
%%

main( argc, argv )
	   int argc;
	   char **argv;
	       {
	       ++argv, --argc;	/* skip over program name */
	       if ( argc > 0 )
		       yyin = fopen( argv[0], "r+" );
	       else {
		       yyin = stdin;
		}
		yyout = fopen("temporary", "w+");
	       yylex();
		fprintf(yyout, "Number of lines: %d, number of words: %d\n", num_lines, num_words);
		remove(argv[0]);
		rename("temporary", argv[0]);			
	       }
