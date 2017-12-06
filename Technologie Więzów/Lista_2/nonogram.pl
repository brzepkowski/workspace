:- use_module(library(clpfd)).

% Columns = [[2], [2], [1, 4], [4], [1], [4]].
% Rows = [[2], [2, 1], [4, 1], [4], [1, 1], [1, 1]].

rows_equal_columns([], []).
rows_equal_columns([X|Xs], [Y|Ys]) :-
  X #= Y,
  rows_equal_columns(Xs, Ys).

list_sum([Item], Item).
list_sum([Item1,Item2 | Tail], Total) :-
    list_sum([Item1+Item2|Tail], Total).

sublist(List, Offset, Length, Sublist) :-
  length(Prefix, Offset),
  append(Prefix, Rest, List),
  length(Sublist, Length),
  append(Sublist, _, Rest).

generate_column_vars(0, _, []).
generate_column_vars(M, N, Xs) :-
  length(X, N), % wygeneruj jedną kolumnę
  X ins 0..1,
  M1 is M - 1,
  generate_column_vars(M1, N, Xss),
  append([X], Xss, Xs).

generate_row_vars(_, 0, []).
generate_row_vars(M, N, Ys) :-
  length(Y, M), % wygeneruj jeden wiersz
  Y ins 0..1,
  N1 is N - 1,
  generate_row_vars(M, N1, Yss),
  append([Y], Yss, Ys).

get_range([], 0).
get_range([C|Cs], R) :-
  get_range(Cs, Rs),
  R is Rs + C + 1.

list_empty([], true).
list_empty([_|_], false).

cartprod([], L, L).
cartprod([X|Xs], Y, L) :-
  cartprod(Xs, Y, L1),
  partial_cartprod([X], Y, L2),
  append(L1, L2, L).

partial_cartprod(_, [], []).
partial_cartprod(X, [Y|Ys], L) :-
  partial_cartprod(X, Ys, L1),
  print("X: "), print(X), nl,
  print("Y: "), print(Y), nl,
  print("L1: "), print(L1), nl,
  append(X, Y, L2),
  print("L2: "), print(L2), nl,
  append(L1, [L2], L).

% C - blok, X - zmienne, R - zasięg (ile mamy możliwości dla tego bloku),
% B - początek
all_options_for_block(_, _, _, 0, _, []).
all_options_for_block(N, [C|Cs], X, R, B, Expr) :-
  B1 is B + 1,
  R1 is R - 1,
  sublist(X, B, C, X1),
  % Wygeneruj wszystkie pozostałe bloki dla X1
  BI is B + C + 1,
  one_column(N, BI, Cs, X, ExprInn),
  list_empty(ExprInn, S),
  (S -> append(ExprInn, [X1], CompleteCase); partial_cartprod([X1], ExprInn, CompleteCase)),
  % print("X1: "), print(X1), nl,
  % print("ExprInn: "), print(ExprInn), nl,
  % print("Complete: "), print(CompleteCase), nl,
  all_options_for_block(N, [C|Cs], X, R1, B1, ExprRest),
  append(CompleteCase, ExprRest, Expr).

% N - długość kolumny, [C|Cs] - lista bloków, X - lista zmiennych w kolumnie,
% B - początek
one_column(_, _, [], _, []).
one_column(N, B, [C|Cs], X, Expr) :-
  get_range(Cs, R1),
  R is N - R1 - C - B + 1,
  all_options_for_block(N, [C|Cs], X, R, B, Expr).

% [C|Cs] - lista list bloków
columns_constraints(_, _, [], [], _).
columns_constraints(N, B, [C|Cs], [X|Xs], Expr) :-
  one_column(N, B, C, X, Expr).
  % columns_constraints(N, Cs, Xs).

nonogram(M, N, Columns) :-
  generate_column_vars(M, N, X),
  generate_row_vars(M, N, Y),
  transpose(Y, YT),
  flatten(YT, YTF),
  flatten(X, XF),
  rows_equal_columns(XF, YTF),
  columns_constraints(N, 0, Columns, X, Expr),
  print(Expr), nl,
  append(XF, YTF, AllVars),
  label(AllVars),
  print(AllVars).
