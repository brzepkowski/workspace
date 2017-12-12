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

list_sum([Item], Item).
list_sum([Item1,Item2 | Tail], Total) :-
   list_sum([Item1+Item2|Tail], Total).

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
  T1 is T - B,
  max(0, T1, Beginning),
  Len1 is T - Beginning,
  % write("Begin: "), write(Beginning), nl,
  % write("T: "), write(T), nl,
  % write("L: "), write(L), nl,
  max(Len1, 1, Length),
  sublist(L, Beginning, Length, V),
  inner_forbid_overlapping(Bs, T, Ls, Vs).

% [L|Ls] - lista lsit przypisanych do każdego bloku
% TimeHorizon - momenty z horyzontu czasowy
% [S|Ss] - lista list z sumami wartości Xᵢ*Tᵢ
generate_vars_times_time_lists([], _, []).
generate_vars_times_time_lists([L|Ls], TimeHorizon, [S|Ss]) :-
  inner_proper_beginning(L, TimeHorizon, S1),
  list_sum(S1, S),
  generate_vars_times_time_lists(Ls, TimeHorizon, Ss).

% Ta funkcja tylko ustala porządek między elementami listy
inner_proper_order([_]).
inner_proper_order([Item1,Item2|Tail]) :-
  Item1 #< Item2,
  inner_proper_order([Item2|Tail]).

% BlocksLists - lista list ze zmiennymi decyzyjnymi, przypisane do bloków (zadań)
% B - lista sum elementów postaci Xᵢ*Tᵢ
proper_order(BlocksLists, TimeHorizon) :-
  generate_vars_times_time_lists(BlocksLists, TimeHorizon, B),
  inner_proper_order(B).

% [T|Ts] - lista z momentami z całego horyzontu czasowego
% BlocksLists - listy przypisane do bloków (zadań)
forbid_overlapping(_, [], _).
forbid_overlapping(Blocks, [T|Ts], BlocksLists) :-
  inner_forbid_overlapping(Blocks, T, BlocksLists, SelectedVars),
  flatten(SelectedVars, FlatSelectedVars),
  % write("SelectedVars: "), write(SelectedVars), nl,
  % write("FlatVars: "), write(FlatSelectedVars), nl,
  sum(FlatSelectedVars, #=<, 1),
  forbid_overlapping(Blocks, Ts, BlocksLists).

% Ta funkcja ustala jako zera ostatnie miejsca w wierszu, w którym nie może znaleźć się blok
% [B|Bs] - lista z długościami bloków
% [L|Ls] - lista list zmiennych przypisanych do bloków (zadań)
limit_endings([], []).
limit_endings([B|Bs], [L|Ls]) :-
  reverse(L, LR),
  B1 is B - 1,
  inner_limit_ending(LR, B1),
  limit_endings(Bs, Ls).

inner_limit_ending(_, 0).
inner_limit_ending([L|Ls], B) :-
  L #= 0,
  B1 is B - 1,
  inner_limit_ending(Ls, B1).

% RowBlocks - tablica z długościami bloków
% RowVars - zmienne decyzyjne z tego wiersza
% N - długość wiersza
add_row_constraints(RowBlocks, BlocksLists, N) :-
  generate_lists_for_blocks(RowBlocks, N, BlocksLists),
  generate_beginnings(RowBlocks, 0, Beginnings),
  generate_time_horizon(N, TimeHorizonReversed),
  reverse(TimeHorizonReversed, TimeHorizon),
  % write("TimeHorizon: "), write(TimeHorizon), nl,
  % write("Beginnings: "), write(Beginnings), nl,
  constrain_one_block(BlocksLists, TimeHorizon, Beginnings),
  forbid_overlapping(RowBlocks, TimeHorizon, BlocksLists),
  proper_order(BlocksLists, TimeHorizon),
  limit_endings(RowBlocks, BlocksLists),
  flatten(BlocksLists, FlatBlocksLists),
  label(FlatBlocksLists).

% [L|Ls] - lista list przypisana do bloków (zadań)
% TimeHorizon - horyzont czasowy
% [B|Bs] - lsita momentów gotowości dla każdego bloku (zadania)
constrain_one_block([], _, []).
constrain_one_block([L|Ls], TimeHorizon, [B|Bs]) :-
  only_one_beginning(L),
  proper_beginning(L, TimeHorizon, B),
  constrain_one_block(Ls, TimeHorizon, Bs).

% M - liczba wierszy, N - liczba kolumn
nonogram(Vars, M, N, Row) :-
  % MN is M*N,
  % length(Vars, MN),
  % Vars ins 0..1,
  add_row_constraints(Row, Vars, N).
  % label(Vars).
