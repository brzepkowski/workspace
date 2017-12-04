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
  print(Cs), nl,
  get_range(Cs, Rs),
  R is Rs + C + 1.

% C - blok, X - zmienne, R - zasięg (ile mamy możliwości dla tego bloku),
% B - początek
all_options_for_block(_, _, 0, _, []).
all_options_for_block(C, X, R, B, Expr) :-
  % sleep(1),
  % print("--------"), nl,
  % print("C: "), print(C), nl,
  % print("R: "), print(R), nl,
  % print("B: "), print(B), nl,
  B1 is B + 1,
  R1 is R - 1,
  sublist(X, B, C, X1),
  all_options_for_block(C, X, R1, B1, ExprInn),
  append([X1], ExprInn, Expr).

% N - długość kolumny, [C|Cs] - lista bloków, X - lista zmiennych w kolumnie,
% B - początek
inner_columns_constraints(N, B, [C|Cs], X) :-
  get_range(Cs, R1),
  R2 is N - R1,
  R is R2 - C + 1,
  all_options_for_block(C, X, R, B, Expr),
  print(Expr), nl.

columns_constraints(_, _, [], []).
columns_constraints(N, B, [C|Cs], [X|Xs]) :-
  inner_columns_constraints(N, B, C, X).
  % columns_constraints(N, Cs, Xs).

nonogram(M, N, Columns) :-
  generate_column_vars(M, N, X),
  generate_row_vars(M, N, Y),
  transpose(Y, YT),
  flatten(YT, YTF),
  flatten(X, XF),
  rows_equal_columns(XF, YTF),
  columns_constraints(N, 0, Columns, X),
  append(XF, YTF, AllVars),
  label(AllVars),
  print(AllVars).

% nonogram(Xs, Ys, M, N, Columns, Rows) :-
%   BoardSize is N*M,
%   length(Xs, BoardSize),
%   length(Ys, BoardSize),
%   Xs ins 0..1,
%   Ys ins 0..1,
%   rows_equal_columns(Xs, Ys),
%   columns_constraints(M, N, 0, Columns, Xs),
%   rows_constraints(M, N, 0, Rows, Ys),
%   append(Xs, Ys, AllVars),
%   label(AllVars),
%   print_(M, N, 0, 0, Ys).
