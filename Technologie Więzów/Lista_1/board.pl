/* Żeby zobaczyc cala odpowiedz:
?- set_prolog_flag(toplevel_print_options,[quoted(true), portray(true), max_depth(0), spacing(next_argument)]).
 */
:- use_module(library(clpfd)).

/* Wersja 5x5 */
board5(Vars) :-
	length(Vars, 25),
	Vars ins 0..1,
	constraints5(Vars, 0),
	label(Vars),
	print_(Vars, 0).

print_(_, 25).
print_(List, I) :-
	nth0(I, List, X),
	write(X), write(" "),
	(mod(I, 5) =:= 4 -> writeln(""); !),
	INext is I + 1,
	print_(List, INext).

constraints5(_, 25).
constraints5(List, I) :-
	nth0(I, List, C),
	(I < 5 ->
		(mod(I, 5) =:= 0 -> IR is I + 1, ID is I + 5, nth0(IR, List, CR), nth0(ID, List, CD), mod(C + CR + CD, 2) #= 1; /* Lewa sciana */
		(mod(I, 5) =:= 4 -> IL is I - 1, ID is I + 5, nth0(IL, List, CL), nth0(ID, List, CD), mod(C + CL + CD, 2) #= 1; /* Prawa sciana */
		IL is I - 1, IR is I + 1, ID is I + 5, nth0(IL, List, CL), nth0(IR, List, CR), nth0(ID, List, CD), mod(C + CL + CR + CD, 2) #= 1 )); 		     /* Miedzy scianami */
	(I > 19 -> 
		(mod(I, 5) =:= 0 -> IR is I + 1, IU is I - 5, nth0(IR, List, CR), nth0(IU, List, CU), mod(C + CR + CU, 2) #= 1; /* Lewa sciana */
		(mod(I, 5) =:= 4 -> IL is I - 1, IU is I - 5, nth0(IL, List, CL), nth0(IU, List, CU), mod(C + CL + CU, 2) #= 1; /* Prawa sciana */
		IL is I - 1, IR is I + 1, IU is I - 5, nth0(IL, List, CL), nth0(IR, List, CR), nth0(IU, List, CU), mod(C + CL + CR + CU, 2) #= 1 )); 		     /* Miedzy scianami */
	(mod(I, 5) =:= 0 -> IR is I + 1, IU is I - 5, ID is I + 5, nth0(IR, List, CR), nth0(IU, List, CU), nth0(ID, List, CD), mod(C + CR + CU + CD, 2) #= 1; /* Lewa sciana */
	(mod(I, 5) =:= 4 -> IL is I - 1, IU is I - 5, ID is I + 5, nth0(IL, List, CL), nth0(IU, List, CU), nth0(ID, List, CD), mod(C + CL + CU + CD, 2) #= 1; /* Prawa sciana */
	IL is I - 1, IR is I + 1, IU is I - 5, ID is I + 5, nth0(IL, List, CL), nth0(IR, List, CR), nth0(IU, List, CU), nth0(ID, List, CD), mod(C + CL + CR + CU + CD, 2) #= 1 )) /* Miedzy scianami */
	)),
	INext is I + 1,
	constraints5(List, INext).














