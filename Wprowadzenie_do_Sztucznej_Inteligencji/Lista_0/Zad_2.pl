is_a(fig1, square).
is_a(fig2, circle).
a_kind_of(square, figure).
a_kind_of(circle, figure).
a_kind_of(smallCircle, circle).
a_kind_of(bigCircle, circle).
has(square, size).
has(circle, radius).
has(figure, color).

subclass(SubClass, Class) :-
	a_kind_of(SubClass, Class);	
	(a_kind_of(SubClass, X), subclass(X, Class)).

super_has(Object, Property) :-
	is_a(Object, X),
	(has(X, Property);
	(subclass(X, Y),
	has(Y, Property))).
