:- dynamic a/3.
a(0, N, X) :- 
	N >= 0,
	X is N + 1,
	asserta(a(0, N, X)).
a(M, 0, X) :-
	M > 0, 
	M1 is M-1,
	a(M1, 1, X),
	asserta(a(M, 0, X)).
a(M, N, X) :- 
	M > 0,
	N > 0,
	N1 is N-1,
	M1 is M-1,	
	a(M, N1, Y),
	a(M1, Y, X),
	asserta(a(M, N, X)).
