%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%  Cartprod.pl:		Prolog source code implementing	     %
%			cartesian product.		     %
%  Versions:	 	1.0 and 2.0			     %
%  Implemented by:	Psallidas Fotis		 	     %
%  std code:		std06170			     %
%  A.M. code:		1115200600170			     %
%  Reason:		Logic Programming course 	     %
%  Semester:		Spring 2009			     %
%							     %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% Verison 1.0 for the cartesian product implementation based on
% double recursion ,one for all of the elements of the set and the other
% for all the elements-(lists) of the cartesian product.

cartprod([H], L) :-
   cartprod2([[]], H, L).
cartprod([H | T], L) :-
   cartprod(T, R),
   cartprod2(R, H, L).

cartprod2([], _, []).
cartprod2([H | IT], T, L) :-
   distribute(H, T, F),
   cartprod2(IT, T, R),
   append(F, R, L).

% distribute takes an element and a list of elements and combine
% the element with each element from the list creating a new list of lists.

distribute(_, [], []).
distribute(X, [H | T], [L | R]) :-
   append([H], X, L),
   distribute(X, T, R).



%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%							  %
% Verison 2.0 of the cartesian product implementation 	  %
% using findall						  %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

cartprod3(S, L) :-
   findall(R, cart(S, R), L).

cart([], []).
cart([[A | _] | T], [A | R]) :-
   cart(T, R).

cart([[_ | B] | T], R) :-
   cart([B | T], R).
