:- use_module(library(clpfd)).
:- use_module(matrix).

matrix_rotate(X, Z) :-
  transpose(X, Y),
  maplist(reverse, Y, Z).

list_sum([Item], Item).
list_sum([Item1,Item2 | Tail], Total) :-
    list_sum([Item1+Item2|Tail], Total).

flatten_([], []).
flatten_([H|T], L) :-
  flatten_(T, Ls),
  append(H, Ls, L).

% X - macierz, którą będziemy obracać, I - licznik, ile obrotów wykonaliśmy, L - lista ze wszsystkim rotacjami pierwotnej macierzy
generate_rotations(_, 4, []).
generate_rotations(X, I, [X|L]) :-
  matrix_rotate(X, Y),
  I1 is I + 1,
  generate_rotations(Y, I1, L).

cube(Vars) :-
  constraints1(X).

constraints1(Vars) :-
  %--------------------PIONOWO--------------------
  % Lewa strona
  generate_rotations([[[0,0,0],[0,0,0],[1,0,0]],[[0,0,0],[0,0,0],[1,0,0]],[[0,0,0],[1,0,0],[1,0,0]]], 0, L0),
  generate_rotations([[[0,0,0],[0,0,0],[1,0,0]],[[0,0,0],[0,0,0],[1,0,0]],[[0,0,0],[0,0,0],[1,1,0]]], 0, L1),
  generate_rotations([[[0,0,0],[1,0,0],[0,0,0]],[[0,0,0],[1,0,0],[0,0,0]],[[0,0,0],[1,0,0],[1,0,0]]], 0, L2),
  generate_rotations([[[0,0,0],[0,0,0],[0,1,0]],[[0,0,0],[0,0,0],[0,1,0]],[[0,0,0],[0,0,0],[1,1,0]]], 0, L3),
  generate_rotations([[[0,0,0],[1,0,0],[1,0,0]],[[0,0,0],[0,0,0],[1,0,0]],[[0,0,0],[0,0,0],[1,0,0]]], 0, L4),
  generate_rotations([[[0,0,0],[0,0,0],[1,1,0]],[[0,0,0],[0,0,0],[1,0,0]],[[0,0,0],[0,0,0],[1,0,0]]], 0, L5),
  generate_rotations([[[0,0,0],[1,0,0],[1,0,0]],[[0,0,0],[1,0,0],[0,0,0]],[[0,0,0],[1,0,0],[0,0,0]]], 0, L6),
  generate_rotations([[[0,0,0],[0,0,0],[1,1,0]],[[0,0,0],[0,0,0],[0,1,0]],[[0,0,0],[0,0,0],[0,1,0]]], 0, L7),
  % Środek
  generate_rotations([[[0,0,0],[0,0,0],[0,1,0]],[[0,0,0],[0,0,0],[0,1,0]],[[0,0,0],[0,1,0],[0,1,0]]], 0, L8),
  generate_rotations([[[0,0,0],[0,1,0],[0,0,0]],[[0,0,0],[0,1,0],[0,0,0]],[[0,0,0],[0,1,0],[0,1,0]]], 0, L9),
  generate_rotations([[[0,0,0],[0,1,0],[0,1,0]],[[0,0,0],[0,0,0],[0,1,0]],[[0,0,0],[0,0,0],[0,1,0]]], 0, L10),
  generate_rotations([[[0,0,0],[0,1,0],[0,1,0]],[[0,0,0],[0,1,0],[0,0,0]],[[0,0,0],[0,1,0],[0,0,0]]], 0, L11),
  % Prawa strona
  generate_rotations([[[0,0,0],[0,0,0],[0,0,1]],[[0,0,0],[0,0,0],[0,0,1]],[[0,0,0],[0,0,1],[0,0,1]]], 0, L12),
  generate_rotations([[[0,0,0],[0,0,0],[0,0,1]],[[0,0,0],[0,0,0],[0,0,1]],[[0,0,0],[0,0,0],[0,1,1]]], 0, L13),
  generate_rotations([[[0,0,0],[0,0,1],[0,0,0]],[[0,0,0],[0,0,1],[0,0,0]],[[0,0,0],[0,0,1],[0,0,1]]], 0, L14),
  generate_rotations([[[0,0,0],[0,0,0],[0,1,0]],[[0,0,0],[0,0,0],[0,1,0]],[[0,0,0],[0,0,0],[0,1,1]]], 0, L15),
  generate_rotations([[[0,0,0],[0,0,1],[0,0,1]],[[0,0,0],[0,0,0],[0,0,1]],[[0,0,0],[0,0,0],[0,0,1]]], 0, L16),
  generate_rotations([[[0,0,0],[0,0,0],[0,1,1]],[[0,0,0],[0,0,0],[0,0,1]],[[0,0,0],[0,0,0],[0,0,1]]], 0, L17),
  generate_rotations([[[0,0,0],[0,0,1],[0,0,1]],[[0,0,0],[0,0,1],[0,0,0]],[[0,0,0],[0,0,1],[0,0,0]]], 0, L18),
  generate_rotations([[[0,0,0],[0,0,0],[0,1,1]],[[0,0,0],[0,0,0],[0,1,0]],[[0,0,0],[0,0,0],[0,1,0]]], 0, L19), % <---
  %--------------------POZIOMO---------------------
  % Lewa strona
  generate_rotations([[[0,0,0],[0,0,0],[0,0,0]],[[0,0,0],[0,0,0],[0,0,0]],[[0,0,0],[0,0,0],[0,0,0]]], 0, L20),
  generate_rotations([[[0,0,0],[0,0,0],[0,0,0]],[[0,0,0],[0,0,0],[0,0,0]],[[0,0,0],[0,0,0],[0,0,0]]], 0, L21),
  generate_rotations([[[0,0,0],[0,0,0],[0,0,0]],[[0,0,0],[0,0,0],[0,0,0]],[[0,0,0],[0,0,0],[0,0,0]]], 0, L22),
  generate_rotations([[[0,0,0],[0,0,0],[0,0,0]],[[0,0,0],[0,0,0],[0,0,0]],[[0,0,0],[0,0,0],[0,0,0]]], 0, L23),
  generate_rotations([[[0,0,0],[0,0,0],[0,0,0]],[[0,0,0],[0,0,0],[0,0,0]],[[0,0,0],[0,0,0],[0,0,0]]], 0, L24),
  generate_rotations([[[0,0,0],[0,0,0],[0,0,0]],[[0,0,0],[0,0,0],[0,0,0]],[[0,0,0],[0,0,0],[0,0,0]]], 0, L25),
  generate_rotations([[[0,0,0],[0,0,0],[0,0,0]],[[0,0,0],[0,0,0],[0,0,0]],[[0,0,0],[0,0,0],[0,0,0]]], 0, L26),
  generate_rotations([[[0,0,0],[0,0,0],[0,0,0]],[[0,0,0],[0,0,0],[0,0,0]],[[0,0,0],[0,0,0],[0,0,0]]], 0, L27),
  generate_rotations([[[0,0,0],[0,0,0],[0,0,0]],[[0,0,0],[0,0,0],[0,0,0]],[[0,0,0],[0,0,0],[0,0,0]]], 0, L28),
  generate_rotations([[[0,0,0],[0,0,0],[0,0,0]],[[0,0,0],[0,0,0],[0,0,0]],[[0,0,0],[0,0,0],[0,0,0]]], 0, L29),
  % Prawa strona
  generate_rotations([[[0,0,0],[0,0,0],[0,0,0]],[[0,0,0],[0,0,0],[0,0,0]],[[0,0,0],[0,0,0],[0,0,0]]], 0, L30),
  generate_rotations([[[0,0,0],[0,0,0],[0,0,0]],[[0,0,0],[0,0,0],[0,0,0]],[[0,0,0],[0,0,0],[0,0,0]]], 0, L31),
  generate_rotations([[[0,0,0],[0,0,0],[0,0,0]],[[0,0,0],[0,0,0],[0,0,0]],[[0,0,0],[0,0,0],[0,0,0]]], 0, L32),
  generate_rotations([[[0,0,0],[0,0,0],[0,0,0]],[[0,0,0],[0,0,0],[0,0,0]],[[0,0,0],[0,0,0],[0,0,0]]], 0, L33),
  generate_rotations([[[0,0,0],[0,0,0],[0,0,0]],[[0,0,0],[0,0,0],[0,0,0]],[[0,0,0],[0,0,0],[0,0,0]]], 0, L34),
  generate_rotations([[[0,0,0],[0,0,0],[0,0,0]],[[0,0,0],[0,0,0],[0,0,0]],[[0,0,0],[0,0,0],[0,0,0]]], 0, L35),
  generate_rotations([[[0,0,0],[0,0,0],[0,0,0]],[[0,0,0],[0,0,0],[0,0,0]],[[0,0,0],[0,0,0],[0,0,0]]], 0, L36),
  generate_rotations([[[0,0,0],[0,0,0],[0,0,0]],[[0,0,0],[0,0,0],[0,0,0]],[[0,0,0],[0,0,0],[0,0,0]]], 0, L37),
  generate_rotations([[[0,0,0],[0,0,0],[0,0,0]],[[0,0,0],[0,0,0],[0,0,0]],[[0,0,0],[0,0,0],[0,0,0]]], 0, L38),
  generate_rotations([[[0,0,0],[0,0,0],[0,0,0]],[[0,0,0],[0,0,0],[0,0,0]],[[0,0,0],[0,0,0],[0,0,0]]], 0, L39),
  flatten_([L0, L1, L2, L3, L4, L5, L6, L7, L8, L9, L10, L11, L12, L13, L14, L15, L16, L17, L18, L19, L20, L21, L22, L23, L24, L25, L26, L27, L28, L29, L30, L31, L32, L33, L34, L35, L36, L37, L38, L39], L),
  writeln(L).
