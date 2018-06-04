
/******************************************/
/**    Your solution goes in this file    **/ 
/*******************************************/

/* Helper Functions */
intersects([], _) :- fail.
intersects([H|T], L2) :- member(H, L2); intersects(T, L2).

intersection([], _, []).
intersection([Head|L1tail], L2, L3) :-
        memberchk(Head, L2),
        !,
        L3 = [Head|L3tail],
        intersection(L1tail, L2, L3tail).
intersection([_|L1tail], L2, L3) :-
        intersection(L1tail, L2, L3).

toSet(List, Set) :- toSetHelper(List, [], Set), !.

toSetHelper([], Acc, Acc).
toSetHelper([H|T], Acc, Set) :- member(H, Acc), toSetHelper(T, Acc, Set).
toSetHelper([H|T], Acc, Set) :- toSetHelper(T, [H|Acc], Set).

myappend([],L,L).
myappend([X|L1],L2,[X|L3]) :- myappend(L1,L2,L3).

delete(X, [X|Tail], Tail).
delete(Elem, [Head|Tail], [Head|Rest]) :- delete(Elem, Tail, Rest).

permutation([], [], []).
permutation([X|Xs], Ys1, [_|Bound]) :-
	permutation(Xs, Ys, Bound),
	select(X, Ys1, Ys).

/* Part One */

year_1953_1996_novels(N) :- novel(N, Y), (Y=1953; Y=1996).  

period_1800_1900_novels(N) :- novel(N, Y), (Y >= 1800, Y =< 1900).

lotr_fans(F) :- fan(F, B), member(the_lord_of_the_rings, B). 

author_names(A) :- setof(W, (fan(chandler, X), author(W, B), intersects(B, X)), Authors), member(A, Authors).

fans_names(F) :- fan(F, B), author(brandon_sanderson, X), intersects(B, X).

mutual_novels(N) :- fan(phoebe, B1), fan(ross, B2), fan(monica, B3), 
	intersection(B1, B2, Intersects1), intersection(B1, B3, Intersects2), intersection(B2, B3, Intersects3),
	append(Intersects1, Intersects2, INew), append(Intersects3, INew, ITotal), member(N, ITotal).

/* Part Two */

isMember(_, []) :- fail.
isMember(E, [H|T]) :- E = H, true; isMember(E, T). 

isUnion(L1, L2, Union) :- myappend(L1, L2, L3), toSet(L3, Union).

isIntersection([Head|Tail],Y,[Head|Z]) :- isMember(Head,Y), isIntersection(Tail,Y,Z).
isIntersection([Head|Tail],Y,Z) :- \+(isMember(Head,Y)), isIntersection(Tail,Y,Z).
isIntersection([],_,[]).

isEqual(Xs, Ys) :- permutation(Xs, Ys, Ys).

powerset([], []).
powerset([_|T], P) :- powerset(T,P).
powerset([H|T], [H|P]) :- powerset(T,P).

powerSet(L, Res) :- findall(L2, powerset(L,L2), Res).

/* Part Three */

%Take and state are data (compount terms)
%state(F,W,G,C). Denotes which Bank each obj is on (left, right)
%take(X,A,B). Takes obj X (none, wolf, goat, cabbage) from Bank A to B

%Opposite and safe predicates
opposite(left, right).
opposite(right, left).
oneEq(X,X,_).
oneEq(X,_,X).

safe(state(F,W,G,C)) :- oneEq(F,G,W), oneEq(F,G,C). 

unsafe(A) :- \+safe(A).

printLst([]).
printLst([H|T]) :- write(H), nl, printLst(T).

% Takes a take and two states and returns true if the take takes one state to the other.
% Returns true if banks are opposite, only move the desired object and farmer, and lead to a safe state.
arc(take(cabbage, Bank1, Bank2),state(Bank1,W,G,Bank1), state(Bank2,W,G,Bank2)) :- opposite(Bank1, Bank2), safe(state(Bank2,W,G,Bank2)). 
arc(take(goat, Bank1, Bank2),state(Bank1,W,Bank1,C), state(Bank2,W,Bank2,C)) :- opposite(Bank1, Bank2), safe(state(Bank2,W,Bank2,C)). 
arc(take(wolf, Bank1, Bank2),state(Bank1,Bank1,G,C),state(Bank2,Bank2,G,C)) :- opposite(Bank1, Bank2), safe(state(Bank2,Bank2,G,C)). 
arc(take(none, Bank1, Bank2),state(Bank1,W,G,C),state(Bank2,W,G,C)) :- opposite(Bank1, Bank2), safe(state(Bank2,W,G,C)). 

arc(take(_,_,_),state(F,W,G,C),state(F,W,G,C)) :- fail. %Same state so backtrack.

% Recursive function that keeps track of path to avoid looping
solvePath(F,W,G,C,F,W,G,C,_,TakeList) :- printLst(TakeList), !. %Base case: Reached the correct state
solvePath(F1,W1,G1,C1,F2,W2,G2,C2,List,TakeList) :- safe(state(F1,W1,G1,C1)), arc(take(Obj,Bank1,Bank2), state(F1,W1,G1,C1),state(NF,NW,NG,NC)), \+member(state(NF,NW,NG,NC),List), 
	solvePath(NF,NW,NG,NC,F2,W2,G2,C2, [state(NF,NW,NG,NC)|List], [take(Obj,Bank1,Bank2)|TakeList]). %Recursively call solve starting from new state that arc generates

solve(F1,W1,G1,C1,F2,W2,G2,C2) :- solvePath(F1,W1,G1,C1,F2,W2,G2,C2, [], []), !. %Initial call to recursive function

