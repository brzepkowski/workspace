OPERATOR	"*"
%%	
"//".*		/* Ignore one-line comments */
"//"(.*\\\n)*.*	/*Ignore "multi-line one-line comments" */


"/**/"
"/*"[^{OPERATOR}]+[.\n]*"*/"
%%

main(int argc,char **argv ) {
	       ++argv, --argc;	/* skip over program name */
	       if ( argc > 0 )
		       yyin = fopen( argv[0], "r+" );
	       else {
		       yyin = stdin;
		}
		yyout = fopen("temporary", "w+");
	       yylex();	
}
