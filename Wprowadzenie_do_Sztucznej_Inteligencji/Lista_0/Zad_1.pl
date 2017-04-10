rodzice(uranus, gaia, rhea).
rodzice(uranus, gaia, cronus).
rodzice(cronus, rhea, zeus).
rodzice(cronus, rhea, hera).
rodzice(cronus, rhea, demeter).
rodzice(zeus, leto, artemis).
rodzice(zeus, leto, apollo).
rodzice(zeus, demeter, persephone).

ojciec(X, Y) :- rodzice(X, _, Y).
matka(X, Y)   :- rodzice(_, X, Y).

rodzic(X, Y) :- ojciec(X, Y).
rodzic(X, Y) :- matka(X, Y).

przodek(X, Y) :- rodzic(X, Y).
przodek(X, Y) :- rodzic(X, Z), przodek(Z, Y).

krewny(X, Y) :-
	X \= Y,
	przodek(Z1, X),
	przodek(Z2, Y),
	Z1 = Z2.
