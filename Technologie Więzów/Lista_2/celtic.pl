:- use_module(library(clpfd)).

% Tiles = [[0, 0, 0, 1],[1, 0, 0, 0],[0, 1, 0, 0],[0, 0, 1, 0],[0, 1, 0, 1],[1, 0, 1, 0],[1, 0, 0, 1],[1, 1, 0, 0],[0, 1, 1, 0],[0, 0, 1, 1],[1, 0, 1, 1],[1, 1, 0, 1],[1, 1, 1, 0],[0, 1, 1, 1],[1, 1, 1, 1]],

vs_0(V, Num) :-
  maplist(eq_4(0), V, Bs),
  sum(Bs, #=, Num).

vs_1(V, Num) :-
  maplist(eq_4(1), V, Bs),
  sum(Bs, #=, Num).

vs_2(V, Num) :-
  maplist(eq_2(2), V, Bs),
  sum(Bs, #=, Num).

vs_3(V, Num) :-
  maplist(eq_3(3), V, Bs),
  sum(Bs, #=, Num).

vs_4(V, Num) :-
  maplist(eq_4(4), V, Bs),
  sum(Bs, #=, Num).

eq_2(Y, [X1,X2,X3,X4], B) :-
  (X1 + X3 #= Y #/\ X2 + X4 #= 0) #\/ (X2 + X4 #= Y #/\ X1 + X3 #= 0) #<==> B.

eq_3(Y, [X1,X2,X3,X4], B) :-
  (X1 + X2 + X3 #= Y #/\ X4 #= 0) #\/ (X2 + X3 + X4 #= Y #/\ X1 #= 0) #\/ (X3 + X4 + X1 #= Y #/\ X2 #= 0) #\/ (X4 + X1 + X2 #= Y #/\ X3 #= 0) #<==> B.

% Poniższy predykat będzie wykorzystywany do 2 rodzajów klocków (z przepływami
% na każdej ścianie i z przepływem tylko na jednej z nich).
eq_4(Y, [X1,X2,X3,X4], B) :-
  X1 + X2 + X3 + X4 #= Y #<==> B.

add_equality_constraints(X) :-
  nth1(1, X, X1), nth1(2, X, X2), nth1(3, X, X3), nth1(4, X, X4), nth1(5, X, X5),
  nth1(6, X, X6), nth1(7, X, X7), nth1(8, X, X8), nth1(9, X, X9), nth1(10, X, X10),
  nth1(11, X, X11), nth1(12, X, X12), nth1(13, X, X13), nth1(14, X, X14), nth1(15, X, X15),
  nth1(16, X, X16), nth1(17, X, X17), nth1(18, X, X18), nth1(19, X, X19), nth1(20, X, X20),
  nth1(21, X, X21), nth1(22, X, X22), nth1(23, X, X23), nth1(24, X, X24), nth1(25, X, X25),
  % Pierwszy wiersz
  nth1(1, X1, X1_1), nth1(2, X1, X1_2), nth1(3, X1, X1_3), nth1(4, X1, X1_4),
  nth1(1, X2, X2_1), nth1(2, X2, X2_2), nth1(3, X2, X2_3), nth1(4, X2, X2_4),
  nth1(1, X3, X3_1), nth1(2, X3, X3_2), nth1(3, X3, X3_3), nth1(4, X3, X3_4),
  nth1(1, X4, X4_1), nth1(2, X4, X4_2), nth1(3, X4, X4_3), nth1(4, X4, X4_4),
  nth1(1, X5, X5_1), nth1(2, X5, X5_2), nth1(3, X5, X5_3), nth1(4, X5, X5_4),
  nth1(1, X6, X6_1), nth1(2, X6, X6_2), nth1(3, X6, X6_3), nth1(4, X6, X6_4),
  nth1(1, X7, X7_1), nth1(2, X7, X7_2), nth1(3, X7, X7_3), nth1(4, X7, X7_4),
  nth1(1, X8, X8_1), nth1(2, X8, X8_2), nth1(3, X8, X8_3), nth1(4, X8, X8_4),
  nth1(1, X9, X9_1), nth1(2, X9, X9_2), nth1(3, X9, X9_3), nth1(4, X9, X9_4),
  nth1(1, X10, X10_1), nth1(2, X10, X10_2), nth1(3, X10, X10_3), nth1(4, X10, X10_4),
  nth1(1, X11, X11_1), nth1(2, X11, X11_2), nth1(3, X11, X11_3), nth1(4, X11, X11_4),
  nth1(1, X12, X12_1), nth1(2, X12, X12_2), nth1(3, X12, X12_3), nth1(4, X12, X12_4),
  nth1(1, X13, X13_1), nth1(2, X13, X13_2), nth1(3, X13, X13_3), nth1(4, X13, X13_4),
  nth1(1, X14, X14_1), nth1(2, X14, X14_2), nth1(3, X14, X14_3), nth1(4, X14, X14_4),
  nth1(1, X15, X15_1), nth1(2, X15, X15_2), nth1(3, X15, X15_3), nth1(4, X15, X15_4),
  nth1(1, X16, X16_1), nth1(2, X16, X16_2), nth1(3, X16, X16_3), nth1(4, X16, X16_4),
  nth1(1, X17, X17_1), nth1(2, X17, X17_2), nth1(3, X17, X17_3), nth1(4, X17, X17_4),
  nth1(1, X18, X18_1), nth1(2, X18, X18_2), nth1(3, X18, X18_3), nth1(4, X18, X18_4),
  nth1(1, X19, X19_1), nth1(2, X19, X19_2), nth1(3, X19, X19_3), nth1(4, X19, X19_4),
  nth1(1, X20, X20_1), nth1(2, X20, X20_2), nth1(3, X20, X20_3), nth1(4, X20, X20_4),
  nth1(1, X21, X21_1), nth1(2, X21, X21_2), nth1(3, X21, X21_3), nth1(4, X21, X21_4),
  nth1(1, X22, X22_1), nth1(2, X22, X22_2), nth1(3, X22, X22_3), nth1(4, X22, X22_4),
  nth1(1, X23, X23_1), nth1(2, X23, X23_2), nth1(3, X23, X23_3), nth1(4, X23, X23_4),
  nth1(1, X24, X24_1), nth1(2, X24, X24_2), nth1(3, X24, X24_3), nth1(4, X24, X24_4),
  nth1(1, X25, X25_1), nth1(2, X25, X25_2), nth1(3, X25, X25_3), nth1(4, X25, X25_4),
  % -------------------Ograniczenia brzegowe------------
  % Pierwszy wiersz
  X1_1 #= 0, X1_2 #= 0, X2_2 #= 0, X3_2 #= 0, X4_2 #= 0, X5_2 #= 0, X5_3 #= 0,
  % Drugi wiersz
  X6_1 #= 0, X10_3 #= 0,
  % Trzeci wiersz
  X11_1 #= 0, X15_3 #= 0,
  % Czwarty wiersz
  X16_1 #= 0, X20_3 #= 0,
  % Piąty wiersz
  X21_1 #= 0, X21_4 #= 0, X22_4 #= 0, X23_4 #= 0, X24_4 #= 0, X25_3 #= 0, X25_4 #= 0,
  % ----------Równość między krawędziami klocków---------
  % Pierwszy wiersz
  X1_3 #= X2_1, X1_4 #= X6_2,
  X2_3 #= X3_1, X2_4 #= X7_2,
  X3_3 #= X4_1, X3_4 #= X8_2,
  X4_3 #= X5_1, X4_4 #= X9_2,
  X5_4 #= X10_2,
  % Drugi wiersz
  X6_3 #= X7_1, X6_4 #= X11_2,
  X7_3 #= X8_1, X7_4 #= X12_2,
  X8_3 #= X9_1, X8_4 #= X13_2,
  X9_3 #= X10_1, X9_4 #= X14_2,
  X10_4 #= X15_2,
  % Trzeci wiersz
  X11_3 #= X12_1, X11_4 #= X16_2,
  X12_3 #= X13_1, X12_4 #= X17_2,
  X13_3 #= X14_1, X13_4 #= X18_2,
  X14_3 #= X15_1, X14_4 #= X19_2,
  X15_4 #= X20_2,
  % Czwarty wiersz
  X16_3 #= X17_1, X16_4 #= X21_2,
  X17_3 #= X18_1, X17_4 #= X22_2,
  X18_3 #= X19_1, X18_4 #= X23_2,
  X19_3 #= X20_1, X19_4 #= X24_2,
  X20_4 #= X25_2,
  % Piąty wiersz
  X21_3 #= X22_1,
  X22_3 #= X23_1,
  X23_3 #= X24_1,
  X24_3 #= X25_1.

limit_number_of_puzzles(X) :-
  vs_0(X, 0),
  vs_1(X, 5),
  vs_2(X, 5),
  vs_3(X, 5),
  vs_4(X, 5).

generate_vars(0, []).
generate_vars(I, Z) :-
  I1 is I - 1,
  generate_vars(I1, Y),
  length(X, 4),
  X ins 0..1,
  append([X], Y, Z).

print_([], _).
print_([V|Vs], I) :-
  Change is I mod 5,
  (Change =:= 0 -> write(V), nl; write(V)),
  I1 is I + 1,
  print_(Vs, I1).

celtic(Vars) :-
  generate_vars(25, Vars),
  add_equality_constraints(Vars),
  limit_number_of_puzzles(Vars),
  flatten(Vars, FlatVars),
  label(FlatVars),
  print_(Vars, 1).
