:- use_module(library(clpfd)).

% Columns = [[2], [2], [1, 4], [4], [1], [4]].
% Rows = [[2], [2, 1], [4, 1], [4], [1, 1], [1, 1]].

% V - długośc bloku, S - dotychczasowy najdalszy punkt startowy,
% [B|Bs] - wygenerowana lista punktów startowych
generate_beginnings([], _, []).
generate_beginnings([V|Vs], B, [B|Bs]) :-
  B1 is B + V + 1,
  generate_beginnings(Vs, B1, Bs).

% M - liczba wierszy, N - liczba kolumn
nonogram(Vars, M, N, Row) :-
  length(Vars, 6),
  Vars ins 0..1,
  generate_beginnings(Row, 0, B),
  print(B).
  % add_constraints(Vars, Row).
