:- use_module(library(clpfd)).

% Przykładowe uruchomienie:
% nonogram(X, 6, 6, [[2],[2,1],[4,1],[4],[1,1],[1,1]], [[2],[2],[1,4],[4],[1],[4]]).

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
  max(Len1, 1, Length),
  sublist(L, Beginning, Length, V),
  inner_forbid_overlapping(Bs, T, Ls, Vs).

% Funkcja zwiekszająca każdy element listy o 1
increase_each_element_by_one([], []).
increase_each_element_by_one([L|Ls], [R|Rs]) :-
  R is L + 1,
  increase_each_element_by_one(Ls, Rs).

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

% [B|Bs] - lista z długościami bloków
% [L|Ls] - lista list zmiennych decyzyjnych przypisanych do bloków
% I - indeks Z-tej zmiennej decyzyjnej
% [R|Rs] - zwracana lista list ze zmiennymi do zsumowania
get_lists_for_sum_constraint([], _, _, []).
get_lists_for_sum_constraint([B|Bs], [L|Ls], I, [R|Rs]) :-
  max(0, I - B, Beginning),
  Length is I - Beginning,
  sublist(L, Beginning, Length, R),
  get_lists_for_sum_constraint(Bs, Ls, I, Rs).

% N - długość wiersza/kolumny, z którego będziemy pobierać zmienne decyzyjne (zmienne z planszy)
constrain_grid_variables(_, 0, _, _).
constrain_grid_variables(Vars, N, Blocks, BlocksLists) :-
  nth1(N, Vars, Z),
  get_lists_for_sum_constraint(Blocks, BlocksLists, N, SubLists),
  flatten(SubLists, SubList),
  sum(SubList, #=, Z),
  N1 is N - 1,
  constrain_grid_variables(Vars, N1, Blocks, BlocksLists).

% RowBlocks - tablica z długościami bloków
% RowVars - zmienne decyzyjne z tego wiersza
% N - długość wiersza
add_row_constraints(RowBlocks, RowVars, N) :-
  generate_lists_for_blocks(RowBlocks, N, BlocksLists),
  generate_beginnings(RowBlocks, 0, Beginnings),
  generate_time_horizon(N, TimeHorizonReversed),
  reverse(TimeHorizonReversed, TimeHorizon),
  constrain_one_block(BlocksLists, TimeHorizon, Beginnings),
  % --------Overlapping--------
  increase_each_element_by_one(RowBlocks, IncreasedRowBlocks),
  forbid_overlapping(IncreasedRowBlocks, TimeHorizon, BlocksLists),
  % ---------------------------
  proper_order(BlocksLists, TimeHorizon),
  limit_endings(RowBlocks, BlocksLists),
  constrain_grid_variables(RowVars, N, RowBlocks, BlocksLists).

% [L|Ls] - lista list przypisana do bloków (zadań)
% TimeHorizon - horyzont czasowy
% [B|Bs] - lsita momentów gotowości dla każdego bloku (zadania)
constrain_one_block([], _, []).
constrain_one_block([L|Ls], TimeHorizon, [B|Bs]) :-
  only_one_beginning(L),
  proper_beginning(L, TimeHorizon, B),
  constrain_one_block(Ls, TimeHorizon, Bs).

% Vars - lista wszystkich zmiennych decyzyjnych na planszy
% B - indeks początku wiersza
% M - początkowa liczba wierszy, funkcja kończy działanie gdy M = 0
% N - długość wiersza (liczba kolumn)
get_rows(_, 0, _, _, []).
get_rows(Vars, M, N, B, [R|Rs]) :-
  sublist(Vars, B, N, R),
  B1 is B + N,
  M1 is M - 1,
  get_rows(Vars, M1, N, B1, Rs).

% M - liczba wierszy (długość kolumny)
% N - liczba kolumn
% [R|Rs] - lista odpowiadająca zmiennym decyzyjnym z kolumny
get_one_column(_, 0, _, _, []).
get_one_column(Vars, M, N, B, [R|Rs]) :-
  nth1(B, Vars, R),
  B1 is B + N,
  M1 is M - 1,
  get_one_column(Vars, M1, N, B1, Rs).

% M - liczba wierszy
% N - liczba kolumn
% C - licznik kolumn (ile zostało do końca)
% B - początek kolumny (będziemy go zwiększać w trakcie działania funkcji)
% [R|Rs] - lista list ze zmiennymi w danych kolumnach (R jest listą zmiennych)
get_columns(_, _, _, 0, _, []).
get_columns(Vars, M, N, C, B, [R|Rs]) :-
  get_one_column(Vars, M, N, B, R),
  B1 is B + 1,
  C1 is C - 1,
  get_columns(Vars, M, N, C1, B1, Rs).

% [B|Bs] - lista list długości bloków (B jest listą z długościami bloków dla danego wiersza)
% [R|Rs] - lista list ze zmiennymi decyzyjnymi wierszy (R jest całym wierszem (listą))
add_constraints_to_all_rows([], [], _).
add_constraints_to_all_rows([B|Bs], [R|Rs], N) :-
  add_row_constraints(B, R, N),
  add_constraints_to_all_rows(Bs, Rs, N).

print_([], _, _).
print_([X|Xs], I, N) :-
  (0 is mod(I,N) -> write(" "), write(X), write(" "), nl; write(" "), write(X), write(" ")),
  I1 is I + 1,
  print_(Xs, I1, N).

% M - liczba wierszy, N - liczba kolumn
nonogram(Vars, M, N, RowsBlocks, ColumnsBlocks) :-
  MN is M*N,
  length(Vars, MN),
  Vars ins 0..1,
  get_rows(Vars, M, N, 0, RowsVars),
  get_columns(Vars, M, N, N, 1, ColumnsVars),
  add_constraints_to_all_rows(RowsBlocks, RowsVars, N),
  add_constraints_to_all_rows(ColumnsBlocks, ColumnsVars, M),
  label(Vars),
  print_(Vars, 1, N).
