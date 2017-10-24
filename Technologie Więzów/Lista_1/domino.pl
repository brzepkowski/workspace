:- use_module(library(clpfd)).

domino(Vars) :-
	Board = [3,1,2,6,6,1,2,2,3,4,1,5,3,0,3,6,5,6,6,1,2,4,5,0,5,6,4,1,3,3,0,0,6,1,0,6,3,2,4,0,4,1,5,2,4,3,5,5,4,1,0,2,4,5,2,0],
	length(Vars, 195),
	Vars ins 0..6,
	set_board_values(Vars, Board, 0),
	set_corner_values(Vars, 16), % Możemy zacząć od pierwszego narożnika
	constraints1(Vars, 0),
	constraints2(Vars, 0),
	writeln("Skonczylo wszystko--------------"),
	label(Vars),
	print_(Vars, 0).



set_board_values(_, _, 195) :- !.
set_board_values([V|Vs], Board, I) :-
	writeln(I),
	Pass = [15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119,135,136,137,138,139,140,141,142,143,144,145,146,147,148,149,165,166,167,168,169,170,171,172,173,174,175,176,177,178,179],
	I1 is I + 1,
	[B|Bs] = Board,
	(memberchk(I, Pass) -> set_board_values(Vs, Board, I1); /* Jeśli należy do zbioru elementów, które pomijamy -> przejdź dalej */
		(mod(I, 2) =:= 0 -> V #= B, set_board_values(Vs, Bs, I1); 
			set_board_values(Vs, Board, I1))).

set_corner_values(_, 179) :- !. % Możemy skończyć przed końcem całej macierzy, po dodaniu wartości ostatniego narożnika
set_corner_values(Vars, I) :-
	write("COR: "), writeln(I),
	Corners = [16, 18, 20, 22, 24, 26, 28, 46, 48, 50, 52, 54, 56, 58, 76, 78, 80, 82, 84, 86, 88, 106, 108, 110, 112, 114, 116, 118, 136, 138, 140, 142, 144, 146, 148, 166, 168, 170, 172, 174, 176, 178],
	I1 is I + 1,
	(memberchk(I, Corners) -> nth0(I, Vars, V), V #= 0; true),
	set_corner_values(Vars, I1).
	



print_(_, 195).
print_(List, I) :-
	nth0(I, List, X),
	write(X), write("     "),
	(mod(I, 15) =:= 14 -> writeln(""); !),
	INext is I + 1,
	print_(List, INext).


constraints1(_, 196) :- !.
constraints1(List, I) :-
	%writeln(I),
	Pass = [15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119,135,136,137,138,139,140,141,142,143,144,145,146,147,148,149,165,166,167,168,169,170,171,172,173,174,175,176,177,178,179],
	(memberchk(I, Pass) -> true ;
	(I < 15 ->
		(mod(I, 15) =:= 0 -> IR is I + 1, ID is I + 15, nth0(IR, List, CR), nth0(ID, List, CD), CR + CD #= 1; /* Lewa sciana */
		(mod(I, 15) =:= 14 -> IL is I - 1, ID is I + 15, nth0(IL, List, CL), nth0(ID, List, CD), CL + CD #= 1; /* Prawa sciana */
		IL is I - 1, IR is I + 1, ID is I + 15, nth0(IL, List, CL), nth0(IR, List, CR), nth0(ID, List, CD), CL + CR + CD #= 1 )); /* Miedzy scianami */
	(I > 179 ->
		(mod(I, 15) =:= 0 -> IR is I + 1, IU is I - 15, nth0(IR, List, CR), nth0(IU, List, CU), CR + CU #= 1; /* Lewa sciana */
		(mod(I, 15) =:= 14 -> IL is I - 1, IU is I - 15, nth0(IL, List, CL), nth0(IU, List, CU), CL + CU #= 1; /* Prawa sciana */
		IL is I - 1, IR is I + 1, IU is I - 15, nth0(IL, List, CL), nth0(IR, List, CR), nth0(IU, List, CU), CL + CR + CU #= 1 )); /* Miedzy scianami */
	(mod(I, 15) =:= 0 -> IR is I + 1, IU is I - 15, ID is I + 15, nth0(IR, List, CR), nth0(IU, List, CU), nth0(ID, List, CD), CR + CU + CD #= 1; /* Lewa sciana */
		(mod(I, 15) =:= 14 -> IL is I - 1, IU is I - 15, ID is I + 15, nth0(IL, List, CL), nth0(IU, List, CU), nth0(ID, List, CD), CL + CU + CD #= 1; /* Prawa sciana */
		IL is I - 1, IR is I + 1, IU is I - 15, ID is I + 15, nth0(IL, List, CL), nth0(IR, List, CR), nth0(IU, List, CU), nth0(ID, List, CD), CL + CR + CU + CD #= 1 )) /* Miedzy scianami */
	))),
	INext is I + 2,	
	constraints1(List, INext).


constraints2(_, 56) :- !, writeln("KONIEC CONSTRAINTS 2").
constraints2(Vars, I) :-
	Board = [3,1,2,6,6,1,2,2,3,4,1,5,3,0,3,6,5,6,6,1,2,4,5,0,5,6,4,1,3,3,0,0,6,1,0,6,3,2,4,0,4,1,5,2,4,3,5,5,4,1,0,2,4,5,2,0],
	writeln("Constraints2: "),	
	writeln(I),
	/*sleep(0.25),*/
	nth0(I, Board, V),
	(I < 8 ->
		(mod(I, 8) =:= 0 -> IR is I + 1, ID is I + 8, nth0(IR, Board, VR), nth0(ID, Board, VD), constraints_for_pairs(V, [VR, VD], Vars); /* Lewa sciana */
		(mod(I, 8) =:= 7 -> IL is I - 1, ID is I + 8, nth0(IL, Board, VL), nth0(ID, Board, VD), constraints_for_pairs(V, [VL, VD], Vars); /* Prawa sciana */
		IL is I - 1, IR is I + 1, ID is I + 8, nth0(IL, Board, VL), nth0(IR, Board, VR), nth0(ID, Board, VD), constraints_for_pairs(V, [VL, VR, VD], Vars) )); 		     /* Miedzy scianami */
	(I > 47 -> 
		(mod(I, 8) =:= 0 -> IR is I + 1, IU is I - 8, nth0(IR, Board, VR), nth0(IU, Board, VU), constraints_for_pairs(V, [VR, VU], Vars); /* Lewa sciana */
		(mod(I, 8) =:= 7 -> IL is I - 1, IU is I - 8, nth0(IL, Board, VL), nth0(IU, Board, VU), constraints_for_pairs(V, [VL, VU], Vars); /* Prawa sciana */
		IL is I - 1, IR is I + 1, IU is I - 8, nth0(IL, Board, VL), nth0(IR, Board, VR), nth0(IU, Board, VU), constraints_for_pairs(V, [VL, VR, VU], Vars) )); 		     /* Miedzy scianami */
	(mod(I, 8) =:= 0 -> IR is I + 1, IU is I - 8, ID is I + 8, nth0(IR, Board, VR), nth0(IU, Board, VU), nth0(ID, Board, VD), constraints_for_pairs(V, [VR, VU, VD], Vars); /* Lewa sciana */
	(mod(I, 8) =:= 7 -> IL is I - 1, IU is I - 8, ID is I + 8, nth0(IL, Board, VL), nth0(IU, Board, VU), nth0(ID, Board, VD), constraints_for_pairs(V, [VL, VU, VD], Vars); /* Prawa sciana */
	IL is I - 1, IR is I + 1, IU is I - 8, ID is I + 8, nth0(IL, Board, VL), nth0(IR, Board, VR), nth0(IU, Board, VU), nth0(ID, Board, VD), constraints_for_pairs(V, [VL, VR, VU, VD], Vars) )) 		     /* Miedzy scianami */
	)),
	INext is I + 1,
	constraints2(Vars, INext).



/* Dodanie sumowania poszczególnych wartości do jedynki --> find_all_cases(VL, VR, Vars, Sublist), list_sum(Sublist, Final), Final #= 1   */
constraints_for_pairs(_, [], _) :- !, writeln("Wszystkie pary!")/*, sleep(3)*/.
constraints_for_pairs(V, [X|Xs], Vars) :-
	write("Weszlo: "), write(V), write(" / "), writeln([X|Xs]),
	find_all_cases(V, X, Vars, []),
	constraints_for_pairs(V, Xs, Vars).


list_sum([Item], Item).
list_sum([Item1,Item2 | Tail], Total) :-
    list_sum([Item1+Item2|Tail], Total).




find_all_cases(V1, V2, Vars, List) :- find_horizontal_cases_(1, V1, V2, Vars, List).




find_horizontal_cases_(111, V1, V2, Vars, List) :- !, writeln("Koniec1 -> przechodze do vertical"), find_vertical_cases_(1, V1, V2, Vars, List).
find_horizontal_cases_(I, V1, V2, Vars, List) :-
	write("Hor: "), writeln(I),
	Ommited = [15,31,47,63,79,95],
	Board1 = [3,-1,1,-1,2,-1,6,-1,6,-1,1,-1,2,-1,2,-1,3,-1,4,-1,1,-1,5,-1,3,-1,0,-1,3,-1,6,-1,5,-1,6,-1,6,-1,1,-1,2,-1,4,-1,5,-1,0,-1,5,-1,6,-1,4,-1,1,-1,3,-1,3,-1,0,-1,0,-1,6,-1,1,-1,0,-1,6,-1,3,-1,2,-1,4,-1,0,-1,4,-1,1,-1,5,-1,2,-1,4,-1,3,-1,5,-1,5,-1,4,-1,1,-1,0,-1,2,-1,4,-1,5,-1,2,-1,0],
	nth0(I, Vars, V),
	I2 is I + 2,	
	(memberchk(I, Ommited) -> 
		find_horizontal_cases_(I2, V1, V2, Vars, List);
		IL is I - 1, IR is I + 1, nth0(IL, Board1, VL), nth0(IR, Board1, VR), (((VL =:= V1, VR =:= V2);(VR =:= V1, VL =:= V2)) ->  writeln("Pasuje!"), append(List, [V], AppendedList), find_horizontal_cases_(I2, V1, V2, Vars, AppendedList); find_horizontal_cases_(I2, V1, V2, Vars, List))
	).




find_vertical_cases_(111, _, _, _, List) :- !, writeln("Koniec!"), writeln(List), list_sum(List, Final), write("Final: "), writeln(Final), Final #= 1.
find_vertical_cases_(I, V1, V2, Vars, List) :-
	write("VER: "), writeln(I),
	Ommited = [13,27,41,55,69,83,97],
	Board2 = [3,-1,3,-1,5,-1,5,-1,6,-1,4,-1,4,-1,1,-1,4,-1,6,-1,6,-1,1,-1,1,-1,1,-1,2,-1,1,-1,6,-1,4,-1,0,-1,5,-1,0,-1,6,-1,5,-1,1,-1,1,-1,6,-1,2,-1,2,-1,6,-1,3,-1,2,-1,3,-1,3,-1,4,-1,4,-1,1,-1,0,-1,4,-1,3,-1,2,-1,3,-1,5,-1,2,-1,3,-1,5,-1,0,-1,4,-1,5,-1,2,-1,2,-1,6,-1,0,-1,0,-1,0,-1,5,-1,0],
	nth0(I, Vars, V),
	I2 is I + 2,	
	(memberchk(I, Ommited) -> 
		find_vertical_cases_(I2, V1, V2, Vars, List);
		IL is I - 1, IR is I + 1, nth0(IL, Board2, VL), nth0(IR, Board2, VR), (((VL =:= V1, VR =:= V2);(VR =:= V1, VL =:= V2)) ->  writeln("Pasuje!"), append(List, [V], AppendedList), find_vertical_cases_(I2, V1, V2, Vars, AppendedList); find_vertical_cases_(I2, V1, V2, Vars, List))
	).

