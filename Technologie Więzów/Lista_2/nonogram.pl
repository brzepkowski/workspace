:- use_module(library(clpfd)).

% Columns = [[2], [2], [1, 4], [4], [1], [4]].
% Rows = [[2], [2, 1], [4, 1], [4], [1, 1], [1, 1]].

max(X, Y, Z) :-
  X =< Y -> Z is Y; Z is X.

sublist(List, Offset, Length, Sublist) :-
   length(Prefix, Offset),
   append(Prefix, Rest, List),
   length(Sublist, Length),
   append(Sublist, _, Rest).

% V - długośc bloku, S - dotychczasowy najdalszy punkt startowy,
% [B|Bs] - wygenerowana lista punktów startowych
generate_beginnings([], _, []).
generate_beginnings([V|Vs], B, [B|Bs]) :-
  B1 is B + V + 1,
  generate_beginnings(Vs, B1, Bs).

% [V|Vs] - tablica z długościami bloków, N - ługość wiersza/kolumny
generate_lists_for_blocks([], _, []).
generate_lists_for_blocks([_|Vs], N, [L|Ls]) :-
  length(L, N),
  L ins 0..1,
  generate_lists_for_blocks(Vs, N, Ls).

generate_time_horizon(1, [1]).
generate_time_horizon(T, [T|Ls]) :-
  T1 is T - 1,
  generate_time_horizon(T1, Ls).

% L - lista zmiennych decyzyjnych
only_one_beginning(L) :-
  sum(L, #=, 1).

% [X|Xs] - lista zmiennych decyzyjnych,
% [T|Ts] - dyskretny czas (lista momentów)
% [B|Bs] - tablica, w której będą zapisane wartości Xᵢ* Tᵢ
inner_proper_beginning([], [], []).
inner_proper_beginning([X|Xs], [T|Ts], [B|Bs]) :-
  T1 is T - 1,
  X*T1 #= B,
  inner_proper_beginning(Xs, Ts, Bs).

% X - lista zmiennych decyzyjnych
% T - horyzont czasowy
% B - moment gotowości
proper_beginning(X, T, B) :-
  inner_proper_beginning(X, T, Bs),
  sum(Bs, #>=, B).

% [B|Bs] - lista z długościami bloków
% [L|Ls] - lista lsit przypisanych do każdego bloku
% T - rozpatrywany moment
% [V|Vs] - tablica z wybranymi konkretnymi zmiennymi decyzyjnymi, które będziemy sumować
inner_forbid_overlapping([], _, [], []).
inner_forbid_overlapping([B|Bs], T, [L|Ls], [V|Vs]) :-
  T1 is T - B + 1,
  max(1, T1, Beginning),
  Len1 is T - Beginning + 1,
  % write("T: "), write(T), nl,
  % write("B: "), write(B), nl,
  % write("Len1: "), write(Len1), nl,
  max(Len1, 0, Length),
  sublist(L, Beginning, Length, V),
  % write("V: "), write(V), nl,
  inner_forbid_overlapping(Bs, T, Ls, Vs).

% [T|Ts] - lista z momentami z całego horyzontu czasowego
% BlocksLists - listy przypisane do bloków (zadań)
forbid_overlapping(_, [], _).
forbid_overlapping(Blocks, [T|Ts], BlocksLists) :-
  inner_forbid_overlapping(Blocks, T, BlocksLists, SelectedVars),
  flatten(SelectedVars, FlatSelectedVars),
  % write("FlatVars: "), write(FlatSelectedVars), nl,
  sum(FlatSelectedVars, #=<, 1),
  forbid_overlapping(Blocks, Ts, BlocksLists).

% RowBlocks - tablica z długościami bloków
% RowVars - zmienne decyzyjne z tego wiersza
% N - długość wiersza
add_row_constraints(RowBlocks, BlocksLists, N) :-
  generate_lists_for_blocks(RowBlocks, N, BlocksLists),
  generate_beginnings(RowBlocks, 0, Beginnings),
  generate_time_horizon(N, TimeHorizonReversed),
  reverse(TimeHorizonReversed, TimeHorizon),
  write("TimeHorizon: "), write(TimeHorizon), nl,
  write("Beginnings: "), write(Beginnings), nl,
  constrain_row_block(BlocksLists, TimeHorizon, Beginnings),
  % forbid_overlapping(RowBlocks, TimeHorizon, BlocksLists),
  flatten(BlocksLists, FlatBlocksLists),
  label(FlatBlocksLists).
  % write("Beginnings: "), write(Beginnings), nl,
  % write("RowBlocks: "), write(RowBlocks), nl,
  % write("BlockLists: "), write(BlocksLists), nl.

% [L|Ls] - lista list przypisana do bloków (zadań)
% TimeHorizon - horyzont czasowy
% [B|Bs] - lsita momentów gotowości dla każdego bloku (zadania)
constrain_row_block([], _, []).
constrain_row_block([L|Ls], TimeHorizon, [B|Bs]) :-
  only_one_beginning(L),
  % write("L: "), write(L), nl,
  % write("B: "), write(B), nl,
  proper_beginning(L, TimeHorizon, B),
  constrain_row_block(Ls, TimeHorizon, Bs).

% M - liczba wierszy, N - liczba kolumn
nonogram(Vars, M, N, Row) :-
  % MN is M*N,
  % length(Vars, MN),
  % Vars ins 0..1,
  add_row_constraints(Row, Vars, N).
  % label(Vars).
