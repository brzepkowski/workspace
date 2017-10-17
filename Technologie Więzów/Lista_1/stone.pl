:- use_module(library(clpfd)).

stones(Vars) :-
	Vars = [W, X, Y, Z],
	Vars ins 1..40,
	all_different(Vars),
	weighting(W, X, Y, Z, 40),
	label([W,X,Y,Z]).


weighting(W, X, Y, Z, 0).
weighting(W, X, Y, Z, N) :-
	N1 is N-1,
	COEFFICIENTS = [A, B, C, D],
	COEFFICIENTS ins -1..1,
	W*A + X*B + Y*C + Z*D #= N,
	weighting(W, X, Y, Z, N1).

/* Uruchomienie:
?- stones([W,X,Y,Z]).
*/

