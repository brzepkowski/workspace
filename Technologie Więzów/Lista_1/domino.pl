:- use_module(library(clpfd)).

domino(Vars) :-
	Board = [3,1,2,6,6,1,2,2,3,4,1,5,3,0,3,6,5,6,6,1,2,4,5,0,5,6,4,1,3,3,0,0,6,1,0,6,3,2,4,0,4,1,5,2,4,3,5,5,4,1,0,2,4,5,2,0],
	length(Vars, 195),
	Vars ins 0..6,
	set_board_values(Vars, Board, 0),
	%constraints1(Vars, 0),
	constraints2(Vars, 3),
	writeln("Przeslo"),
	%label(Vars),
	%print_(Vars, 0).



set_board_values(_, _, 194).
set_board_values([V|Vs], Board, I) :-
	Pass = [15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119,135,136,137,138,139,140,141,142,143,144,145,146,147,148,149,165,166,167,168,169,170,171,172,173,174,175,176,177,178,179],
	I1 is I + 1,
	[B|Bs] = Board,
	(memberchk(I, Pass) -> set_board_values(Vs, Board, I1); /* Jeśli należy do zbioru elementów, które pomijamy -> przejdź dalej */
		(mod(I, 2) =:= 0 -> V #= B, set_board_values(Vs, Bs, I1); 
			set_board_values(Vs, Board, I1))).
	



print_(_, 195).
print_(List, I) :-
	nth0(I, List, X),
	write(X), write(" "),
	(mod(I, 15) =:= 14 -> writeln(""); !),
	INext is I + 1,
	print_(List, INext).



constraints1(_, 196) :- !.
constraints1(List, I) :-
	%writeln(I),
	(I < 15 ->
		(mod(I, 15) =:= 0 -> IR is I + 1, ID is I + 15, nth0(IR, List, CR), nth0(ID, List, CD), CR + CD #= 1; /* Lewa sciana */
		(mod(I, 15) =:= 14 -> IL is I - 1, ID is I + 15, nth0(IL, List, CL), nth0(ID, List, CD), CL + CD #= 1; /* Prawa sciana */
		IL is I - 1, IR is I + 1, ID is I + 15, nth0(IL, List, CL), nth0(IR, List, CR), nth0(ID, List, CD), CL + CR + CD #= 1 )); 		     /* Miedzy scianami */
	(I > 179 ->
		(mod(I, 15) =:= 0 -> IR is I + 1, IU is I - 15, nth0(IR, List, CR), nth0(IU, List, CU), CR + CU #= 1; /* Lewa sciana */
		(mod(I, 15) =:= 14 -> IL is I - 1, IU is I - 15, nth0(IL, List, CL), nth0(IU, List, CU), CL + CU #= 1 /*, writeln("KONIEC!")*/ ; /* Prawa sciana */
		IL is I - 1, IR is I + 1, IU is I - 15, nth0(IL, List, CL), nth0(IR, List, CR), nth0(IU, List, CU), CL + CR + CU #= 1 )); 		     /* Miedzy scianami */
	(mod(I, 15) =:= 0 -> IR is I + 1, IU is I - 15, ID is I + 15, nth0(IR, List, CR), nth0(IU, List, CU), nth0(ID, List, CD), CR + CU + CD #= 1; /* Lewa sciana */
		(mod(I, 15) =:= 14 -> IL is I - 1, IU is I - 15, ID is I + 15, nth0(IL, List, CL), nth0(IU, List, CU), nth0(ID, List, CD), CL + CU + CD #= 1; /* Prawa sciana */
		IL is I - 1, IR is I + 1, IU is I - 15, ID is I + 15, nth0(IL, List, CL), nth0(IR, List, CR), nth0(IU, List, CU), nth0(ID, List, CD), CL + CR + CU + CD #= 1 )) /* Miedzy scianami */
	)),
	INext is I + 2,
	constraints1(List, INext).



list_sum([Item], Item).
list_sum([Item1,Item2 | Tail], Total) :-
    list_sum([Item1+Item2|Tail], Total).



constraints2(_, 195).
constraints2(Vars, I) :-
	Board1 = [3,-1,1,-1,2,-1,6,-1,6,-1,1,-1,2,-1,2,-1,3,-1,4,-1,1,-1,5,-1,3,-1,0,-1,3,-1,6,-1,5,-1,6,-1,6,-1,1,-1,2,-1,4,-1,5,-1,0,-1,5,-1,6,-1,4,-1,1,-1,3,-1,3,-1,0,-1,0,-1,6,-1,1,-1,0,-1,6,-1,3,-1,2,-1,4,-1,0,-1,4,-1,1,-1,5,-1,2,-1,4,-1,3,-1,5,-1,5,-1,4,-1,1,-1,0,-1,2,-1,4,-1,5,-1,2,-1,0],
	Board2 = [3,-1,3,-1,5,-1,5,-1,6,-1,4,-1,4,-1,1,-1,4,-1,6,-1,6,-1,1,-1,1,-1,1,2,-1,1,-1,6,-1,4,-1,0,-1,5,-1,0,-1,6,-1,5,-1,1,-1,1,-1,6,-1,2,-1,2,-1,6,-1,3,-1,2,-1,3,-1,3,-1,4,-1,4,-1,1,-1,0,-1,4,-1,3,-1,2,-1,3,-1,5,-1,2,-1,3,-1,5,-1,0,-1,4,-1,5,-1,2,-1,2,-1,6,-1,0,-1,0,-1,0,-1,5,-1,0],
	write(I),
	write(" --> "),
	(memberchk(I, Horizontal) -> 
		IU is I - 15, ID is I + 15, nth0(IU, Board, VU), nth0(ID, Board, VD), find_all_cases(VU, VD, Vars, Sublist), list_sum(Sublist, Final), Final #= 1; 
		IL is I - 1, IR is I + 1, nth0(IL, Board, VL), nth0(IR, Board, VR), find_all_cases(VL, VR, Vars, Sublist), list_sum(Sublist, Final), Final #= 1
	),
	I2 is I + 2,
	constraints2(Vars, I2).
	











