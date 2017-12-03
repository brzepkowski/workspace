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

% początkowo len = length(Column_blocks), potem jest to iterator
% x - ile "kolumn" musimy przeskoczyc (potrzebne, ponieważ zmienne decyzyjne są w liście jednowymiarowej)
inner_columns_constraints(0, N, _, X, Xs, Total) :-
  sublist(Xs, X, N, ColumnXs),
  list_sum(ColumnXs, SumOfXs),
  SumOfXs #= Total.
inner_columns_constraints(Len, N, Column_blocks, X, Xs, Previous) :-
  nth1(Len, Column_blocks, Block),
  Total is Previous + Block,
  Length is Total + 1,
  Beginning is N - Length,
  SummedBeginning is X + Beginning,
  sublist(Xs, SummedBeginning, Length, ColumnXs),
  list_sum(ColumnXs, SumOfXs),
  SumOfXs #>= Total,
  Len1 is Len - 1,
  inner_columns_constraints(Len1, N, Column_blocks, X, Xs, Total).

columns_constraints(0, _, _, _, _).
columns_constraints(M, N, X, Columns, Xs) :-
  nth1(M, Columns, Column_blocks),
  length(Column_blocks, Len),
  inner_columns_constraints(Len, N, Column_blocks, X, Xs, 0),
  X1 is X + N,
  M1 is M - 1,
  columns_constraints(M1, N, X1, Columns, Xs).

% początkowo len = length(Column_blocks), potem jest to iterator
% x - ile "kolumn" musimy przeskoczyc (potrzebne, ponieważ zmienne decyzyjne są w liście jednowymiarowej)
inner_rows_constraints(0, N, _, Y, Ys, Total) :-
  sublist(Ys, Y, N, RowYs),
  list_sum(RowYs, SumOfYs),
  SumOfYs #= Total.
inner_rows_constraints(Len, M, Row_blocks, Y, Ys, Previous) :-
  nth1(Len, Row_blocks, Block),
  Total is Previous + Block,
  Length is Total + 1,
  Beginning is M - Length,
  SummedBeginning is Y + Beginning,
  sublist(Ys, SummedBeginning, Length, RowYs),
  list_sum(RowYs, SumOfYs),
  SumOfYs #>= Total,
  Len1 is Len - 1,
  inner_rows_constraints(Len1, M, Row_blocks, Y, Ys, Total).

rows_constraints(_, 0, _, _, _).
rows_constraints(M, N, Y, Rows, Ys) :-
  nth1(N, Rows, Row_blocks),
  length(Row_blocks, Len),
  inner_rows_constraints(Len, M, Row_blocks, Y, Ys, 0),
  Y1 is Y + M,
  N1 is N - 1,
  rows_constraints(M, N1, Y1, Rows, Ys).

print_(_, N, N, _, _).
print_(M, N, I, X, AllVars) :- % I - aktualnie drukowany wiersz
  sublist(AllVars, X, M, Sublist),
  print(Sublist), nl,
  I1 is I + 1,
  X1 is X + M,
  print_(M, N, I1, X1, AllVars).

nonogram(Xs, Ys, M, N, Columns, Rows) :-
  BoardSize is N*M,
  length(Xs, BoardSize),
  length(Ys, BoardSize),
  Xs ins 0..1,
  Ys ins 0..1,
  rows_equal_columns(Xs, Ys),
  columns_constraints(M, N, 0, Columns, Xs),
  rows_constraints(M, N, 0, Rows, Ys),
  append(Xs, Ys, AllVars),
  label(AllVars),
  print_(M, N, 0, 0, Ys).
