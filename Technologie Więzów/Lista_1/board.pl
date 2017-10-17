:- use_module(library(clpfd)).

/*click(Vars) :-
	Vars = [W,X,Y,Z],
	Vars ins 0..1,
	Board = [0,0,0,0],
	mult_one_list([1,1,1,0], 1, R1),
	mult_one_list([1,1,0,1], 1, R2),
	mult_one_list([1,0,1,1], 1, R3),
	mult_one_list([0,1,1,1], Z, R4),
	list_sum(Board, R1, L1),
	list_sum(L1, R2, L2),
	list_sum(L2, R3, L3),
	list_sum(L3, R4, L4),
	L4 #= [3,3,3,3],
	nth0(0,R4,X2),
	X2 #= 0,
	label(Vars).*/
	
click(X) :-
	X in 0..1,
	0 * X #= 0,
	label([X]).

list_sum([],[],[]).
list_sum([H1|T1],[H2|T2],[X|L3]):-list_sum(T1,T2,L3), X is H1+H2.

mult_one_list(L1, Elem, R) :-
    maplist(mult_2_numbers(Elem), L1, R).

mult_2_numbers(V1, V2, R) :-
    R is V1 * V2.

