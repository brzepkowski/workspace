// -------------------------------------------------------------- -*- C++ -*-
// File: ./examples/src/donald.cpp
// --------------------------------------------------------------------------
// Licensed Materials - Property of IBM
// 5724-Y47
// Copyright IBM Corporation 1990, 2009. All Rights Reserved.
//
// Note to U.S. Government Users Restricted Rights:
// Use, duplication or disclosure restricted by GSA ADP Schedule
// Contract with IBM Corp.
// --------------------------------------------------------------------------

/* ------------------------------------------------------------

Problem description
-------------------

Solve the cryptarithm:

     D O N A L D
   + G E R A L D
 ----------------
   = R O B E R T

where each letter represents a different digit.

------------------------------------------------------------ */

#include <ilsolver/ilosolverint.h>

ILOSTLBEGIN


//
// FIRST MODEL
//

void basicModel(IlcInt order) {
    IloEnv env;
    try {
        IloModel mdl(env);

        IloIntVar D(env, 1, 9),
        O(env, 0, 9),
        N(env, 0, 9),
        A(env, 0, 9),
        L(env, 0, 9),
        G(env, 1, 9),
        E(env, 0, 9),
        R(env, 1, 9),
        B(env, 0, 9),
        T(env, 0, 9);
        IloIntVarArray vars (env, 10, D, O, N, A, L, G, E, R, B, T);

        IloConstraint alldiff=IloAllDiff(env,vars);
        mdl.add(alldiff);
        mdl.add(100000L*D + 1000*N + 200*A + 20*L + 2*D
                + 100000L*G + 9900*E
                == 99010L*R + 1000*B + T);

        IloIntVar donald(env,0,IloIntMax);
        mdl.add(donald == 100000L*D + 10000*O + 1000*N + 100*A + 10*L + D);
        IloIntVar gerald(env,0,IloIntMax);
        mdl.add(gerald == 100000L*G + 10000*E + 1000*R + 100*A + 10*L + D);
        IloIntVar robert(env,0,IloIntMax);
        mdl.add(robert == 100000L*R + 10000*O + 1000*B + 100*E + 10*R + T);
        IloSolver solver(mdl);
        solver.setFilterLevel(alldiff,IlcExtended);
        if (order > 0)
            solver.startNewSearch(IloGenerate(env, vars, IlcChooseMinSizeInt));
        else
            solver.startNewSearch(IloGenerate(env, vars));

        solver.next();
        solver.endSearch();
        solver.printInformation();

        solver.out() << "   " << solver.getIntVar(donald).getValue() << endl;
        solver.out() << " + " << solver.getIntVar(gerald).getValue() << endl;
        solver.out() << " = " << solver.getIntVar(robert).getValue() << endl;

    }
    catch (IloException& ex) {
        cout << "Error: " << ex << endl;
    }
    env.end();
}


//
// MODEL WITH PREDICATES
//

//
// Cryptarithm Predicates
//

// x + x == y rightmost column
ILOINTBINARYPREDICATE0(CryptaPredicate1) {
    if (val1 == val2)
        return IloFalse;
    const IloNum tmp = 2*val1;
    return (tmp - 10. == val2
            || tmp == val2);
}

// x + y == x rightmost column
ILOINTBINARYPREDICATE0(CryptaPredicate2) {
    return (val1 != 0. && val2 == 0.);
}

// x + y == z rigthmost column
ILOINTTERNARYPREDICATE0(CryptaPredicate3) {
    if (val1 == val2 || val1 == val3 || val2 == val3)
        return IloFalse;
    const IloNum tmp = val1+val2;
    return (tmp - 10. == val3
            || tmp == val3);
}

// x + x == y middle
ILOINTBINARYPREDICATE0(CryptaPredicate4) {
    if (val1 == val2)
        return IloFalse;
    const IloNum tmp = 2*val1;
    return (tmp - 9. == val2
            || tmp - 10. == val2
            || tmp + 1. == val2
            || tmp == val2);
}

// x + y == x middle
ILOINTBINARYPREDICATE0(CryptaPredicate5) {
    if (val1 == val2)
        return IloFalse;
    return (9. == val2
            || 0. == val2);
}

// x + y == z middle
ILOINTTERNARYPREDICATE0(CryptaPredicate6) {
    if (val1 == val2 || val1 == val3 || val2 == val3)
        return IloFalse;
    const IloNum tmp = val1+val2;
    return (tmp - 9. == val3
            || tmp - 10. == val3
            || tmp + 1. == val3
            || tmp == val3);
}

// x + x == y leftmost column
ILOINTBINARYPREDICATE0(CryptaPredicate7) {
    if (val1 == val2)
        return IloFalse;
    const IloNum tmp = 2*val1;
    return (tmp + 1. == val2
            || tmp == val2);
}

// x + y == x leftmost column
ILOINTTERNARYPREDICATE0(CryptaPredicate8) {
    return (val1 != val2 && val2 == 0);
}

// x + y == z leftmost column
ILOINTTERNARYPREDICATE0(CryptaPredicate9) {
    if (val1 == val2 || val1 == val3 || val2 == val3)
        return IloFalse;
    const IloNum tmp = val1+val2;
    return (tmp + 1. == val3
            || tmp == val3);
}

void modelWithPredicates(IlcInt order) {
    IloEnv env;
    try {
        IloModel mdl(env);

        IloIntVar D(env,1, 9),
        O(env,0, 9),
        N(env,0, 9),
        A(env,0, 9),
        L(env,0, 9),
        G(env,1, 9),
        E(env,0, 9),
        R(env,1, 9),
        B(env,0, 9),
        T(env,0, 9);

        IloIntVarArray vars (env, 10, D, O, N, A, L, G, E, R, B, T);

        IloIntVarArray col1(env,2, D, T);
        mdl.add(IloTableConstraint(env,col1,CryptaPredicate1(env)));

        IloIntVarArray col2(env,2, L, R);
        mdl.add(IloTableConstraint(env,col2,CryptaPredicate4(env)));

        IloIntVarArray col3(env,2, A, E);
        mdl.add(IloTableConstraint(env,col3,CryptaPredicate4(env)));

        IloIntVarArray col4(env,3, N, R, B);
        mdl.add(IloTableConstraint(env,col4,CryptaPredicate6(env)));

        IloIntVarArray col5(env,2, O, E);
        mdl.add(IloTableConstraint(env,col5,CryptaPredicate5(env)));


        IloIntVarArray col6(env,3, D, G, R);
        mdl.add(IloTableConstraint(env,col6,CryptaPredicate9(env)));

        IloConstraint alldiff=IloAllDiff(env,vars);
        mdl.add(alldiff);

        mdl.add(100000L*D + 1000*N + 200*A + 20*L + 2*D
                + 100000L*G + 9900*E
                == 99010L*R + 1000*B + T);


        IloIntVar donald(env,0,IloIntMax);
        mdl.add(donald == 100000L*D + 10000*O + 1000*N + 100*A + 10*L + D);
        IloIntVar gerald(env,0,IloIntMax);
        mdl.add(gerald == 100000L*G + 10000*E + 1000*R + 100*A + 10*L + D);
        IloIntVar robert(env,0,IloIntMax);
        mdl.add(robert == 100000L*R + 10000*O + 1000*B + 100*E + 10*R + T);

        IloSolver solver(mdl);
        solver.setFilterLevel(alldiff,IlcExtended);
        if (order > 0)
            solver.startNewSearch(IloGenerate(env, vars, IlcChooseMinSizeInt));
        else
            solver.startNewSearch(IloGenerate(env, vars));

        solver.next();
        solver.endSearch();
        solver.printInformation();

        solver.out() << "   " << solver.getIntVar(donald).getValue() << endl;
        solver.out() << " + " << solver.getIntVar(gerald).getValue() << endl;
        solver.out() << " = " << solver.getIntVar(robert).getValue() << endl;

    }
    catch (IloException& ex) {
        cout << "Error: " << ex << endl;
    }
    env.end();
}

//
// MODEL WITH SET OF ALLOWED TUPLES
//


IloIntTupleSet computeBinaryTuples(IloEnv env, IloIntBinaryPredicate p) {
    IloInt val1, val2;
    IloIntTupleSet set(env,2);

    for(IloInt i=0;i<10;i++) {
        val1=i;
        for(IloInt j=0;j<10;j++) {
            val2=j;
            if (p.isTrue(val1,val2)) {
                IloIntArray array(env,2,val1,val2);
                set.add(array);
            }
        }
    }
    return set;
}

IloIntTupleSet computeTernaryTuples(IloEnv env,IloIntTernaryPredicate p) {
    IloInt val1,val2,val3;
    IloIntTupleSet set(env,3);

    for(IloInt i=0;i<10;i++) {
        val1=i;
        for(IloInt j=0;j<10;j++) {
            val2=j;
            for(IloInt k=0;k<10;k++) {
                val3=k;
                if (p.isTrue(val1,val2,val3)) {
                    IloIntArray array(env,3,val1,val2,val3);
                    set.add(array);
                }
            }
        }
    }
    return set;
}

void modelWithSetOfAllowedTuples(IlcInt order) {
    IloEnv env;
    try {
        IloModel mdl(env);

        IloIntVar D(env,1, 9),
        O(env,0, 9),
        N(env,0, 9),
        A(env,0, 9),
        L(env,0, 9),
        G(env,1, 9),
        E(env,0, 9),
        R(env,1, 9),
        B(env,0, 9),
        T(env,0, 9);

        IloIntVarArray vars (env, 10, D, O, N, A, L, G, E, R, B, T);

        IloIntVarArray col1(env,2, D, T);
        mdl.add(IloTableConstraint
                (env,
                 col1,
                 computeBinaryTuples(env,CryptaPredicate1(env)),
                 IloTrue));

        IloIntTupleSet set=computeBinaryTuples(env,CryptaPredicate4(env))
                           ;
        IloIntVarArray col2(env,2, L, R);
        mdl.add(IloTableConstraint(env,col2,set,IloTrue));

        IloIntVarArray col3(env,2, A, E);
        mdl.add(IloTableConstraint(env,col3,set,IloTrue));

        IloIntVarArray col4(env,3, N, R, B);
        mdl.add(IloTableConstraint
                (env,
                 col4,
                 computeTernaryTuples(env,CryptaPredicate6(env)),
                 IloTrue));

        IloIntVarArray col5(env,2, O, E);
        mdl.add(IloTableConstraint
                (env,
                 col5,
                 computeBinaryTuples(env,CryptaPredicate5(env)),
                 IloTrue));

        IloIntVarArray col6(env,3, D, G, R);
        mdl.add(IloTableConstraint
                (env,
                 col6,
                 computeTernaryTuples(env,CryptaPredicate9(env)),
                 IloTrue));

        IloConstraint alldiff=IloAllDiff(env,vars);
        mdl.add(alldiff);

        mdl.add(100000L*D + 1000*N + 200*A + 20*L + 2*D
                + 100000L*G + 9900*E
                == 99010L*R + 1000*B + T);


        IloIntVar donald(env,0,IloIntMax);
        mdl.add(donald == 100000L*D + 10000*O + 1000*N + 100*A + 10*L + D);
        IloIntVar gerald(env,0,IloIntMax);
        mdl.add(gerald == 100000L*G + 10000*E + 1000*R + 100*A + 10*L + D);
        IloIntVar robert(env,0,IloIntMax);
        mdl.add(robert == 100000L*R + 10000*O + 1000*B + 100*E + 10*R + T);

        IloSolver solver(mdl);
        solver.setFilterLevel(alldiff,IlcExtended);
        if (order > 0)
            solver.startNewSearch(IloGenerate(env, vars, IlcChooseMinSizeInt));
        else
            solver.startNewSearch(IloGenerate(env, vars));

        solver.next();
        solver.endSearch();
        solver.printInformation();

        solver.out() << "   " << solver.getIntVar(donald).getValue() << endl;
        solver.out() << " + " << solver.getIntVar(gerald).getValue() << endl;
        solver.out() << " = " << solver.getIntVar(robert).getValue() << endl;

    }
    catch (IloException& ex) {
        cout << "Error: " << ex << endl;
    }
    env.end();
}

//
// MODEL WITH SET OF FORBIDDEN TUPLES
//

IloIntTupleSet computeForbiddenBinaryTuples(IloEnv env,IloIntBinaryPredicate p) {
    IloInt val1, val2;
    IloIntTupleSet set(env,2);

    for(IloInt i=0;i<10;i++) {
        val1=i;
        for(IloInt j=0;j<10;j++) {
            val2=j;
            if (!p.isTrue(val1,val2)) {
                IloIntArray array(env,2,val1,val2);
                set.add(array);
            }
        }
    }
    return set;
}

IloIntTupleSet computeForbiddenTernaryTuples(IloEnv env,IloIntTernaryPredicate p) {
    IloInt val1,val2,val3;
    IloIntTupleSet set(env,3);

    for(IloInt i=0;i<10;i++) {
        val1=i;
        for(IloInt j=0;j<10;j++) {
            val2=j;
            for(IloInt k=0;k<10;k++) {
                val3=k;
                if (!p.isTrue(val1,val2,val3)) {
                    IloIntArray array(env,3,val1,val2,val3);
                    set.add(array);
                }
            }
        }
    }
    return set;
}

void modelWithSetOfForbiddenTuples(IlcInt order) {
    IloEnv env;
    try {
        IloModel mdl(env);

        IloIntVar D(env,1, 9),
        O(env,0, 9),
        N(env,0, 9),
        A(env,0, 9),
        L(env,0, 9),
        G(env,1, 9),
        E(env,0, 9),
        R(env,1, 9),
        B(env,0, 9),
        T(env,0, 9);

        IloIntVarArray vars (env, 10, D, O, N, A, L, G, E, R, B, T);

        IloIntVarArray col1(env,2, D, T);
        mdl.add(IloTableConstraint
                (env,
                 col1,
                 computeForbiddenBinaryTuples(env,CryptaPredicate1(env)),
                 IloFalse));

        IloIntTupleSet set=computeForbiddenBinaryTuples(env,CryptaPredicate4(env))
                           ;
        IloIntVarArray col2(env,2, L, R);
        mdl.add(IloTableConstraint(env,col2,set,IloFalse))
        ;

        IloIntVarArray col3(env,2, A, E);
        mdl.add(IloTableConstraint(env,col3,set,IloFalse))
        ;

        IloIntVarArray col4(env,3, N, R, B);
        mdl.add(IloTableConstraint
                (env,
                 col4,
                 computeForbiddenTernaryTuples(env,CryptaPredicate6(env)),
                 IloFalse));

        IloIntVarArray col5(env,2, O, E);
        mdl.add(IloTableConstraint
                (env,
                 col5,
                 computeForbiddenBinaryTuples(env,CryptaPredicate5(env)),
                 IloFalse));

        IloIntVarArray col6(env,3, D, G, R);
        mdl.add(IloTableConstraint
                (env,
                 col6,
                 computeForbiddenTernaryTuples(env,CryptaPredicate9(env)),
                 IloFalse));

        IloConstraint alldiff=IloAllDiff(env,vars);
        mdl.add(alldiff);

        mdl.add(100000L*D + 1000*N + 200*A + 20*L + 2*D
                + 100000L*G + 9900*E
                == 99010L*R + 1000*B + T);


        IloIntVar donald(env,0,IloIntMax);
        mdl.add(donald == 100000L*D + 10000*O + 1000*N + 100*A + 10*L + D);
        IloIntVar gerald(env,0,IloIntMax);
        mdl.add(gerald == 100000L*G + 10000*E + 1000*R + 100*A + 10*L + D);
        IloIntVar robert(env,0,IloIntMax);
        mdl.add(robert == 100000L*R + 10000*O + 1000*B + 100*E + 10*R + T);

        IloSolver solver(mdl);
        solver.setFilterLevel(alldiff,IlcExtended);
        if (order > 0)
            solver.startNewSearch(IloGenerate(env, vars, IlcChooseMinSizeInt));
        else
            solver.startNewSearch(IloGenerate(env, vars));

        solver.next();
        solver.endSearch();
        solver.printInformation();

        solver.out() << "   " << solver.getIntVar(donald).getValue() << endl;
        solver.out() << " + " << solver.getIntVar(gerald).getValue() << endl;
        solver.out() << " = " << solver.getIntVar(robert).getValue() << endl;

    }
    catch (IloException& ex) {
        cout << "Error: " << ex << endl;
    }
    env.end();
}


int main(int argc, char** argv) {
    IlcInt pb=(argc>1)?atoi(argv[1]):1;
    if (pb < 1 || pb > 4)
        pb=1;
    IlcInt order=(argc>2)?atoi(argv[2]):0;

    switch(pb) {
        case 1 : {
            basicModel(order);
        }
        break;
        case 2 : {
            modelWithPredicates(order);
        }
        break;
        case 3 : {
            modelWithSetOfAllowedTuples(order);
        }
        break;
        case 4 : {
            modelWithSetOfForbiddenTuples(order);
        }
        break;
    }

    return 0;
}

/*
donald 1 0

Number of fails               : 4612
Number of choice points       : 4618
Number of variables           : 11
Number of constraints         : 3
Reversible stack (bytes)      : 4044
Solver heap (bytes)           : 12084
Solver global heap (bytes)    : 4044
And stack (bytes)             : 4044
Or stack (bytes)              : 4044
Search Stack (bytes)          : 4044
Constraint queue (bytes)      : 4104
Total memory used (bytes)     : 36408
Elapsed time since creation   : 0.797
   526485
 + 197485
 = 723970


donald 2 0

Number of fails               : 138
Number of choice points       : 140
Number of variables           : 11
Number of constraints         : 9
Reversible stack (bytes)      : 4044
Solver heap (bytes)           : 20124
Solver global heap (bytes)    : 24144
And stack (bytes)             : 4044
Or stack (bytes)              : 4044
Search Stack (bytes)          : 4044
Constraint queue (bytes)      : 4104
Total memory used (bytes)     : 64548
Elapsed time since creation   : 0.109
   526485
 + 197485
 = 723970


donald 3 0

Number of fails               : 138
Number of choice points       : 140
Number of variables           : 11
Number of constraints         : 9
Reversible stack (bytes)      : 4044
Solver heap (bytes)           : 40224
Solver global heap (bytes)    : 24144
And stack (bytes)             : 4044
Or stack (bytes)              : 4044
Search Stack (bytes)          : 4044
Constraint queue (bytes)      : 4104
Total memory used (bytes)     : 84648
Elapsed time since creation   : 0.078
   526485
 + 197485
 = 723970


donald 4 0

Number of fails               : 138
Number of choice points       : 140
Number of variables           : 11
Number of constraints         : 9
Reversible stack (bytes)      : 4044
Solver heap (bytes)           : 168864
Solver global heap (bytes)    : 24144
And stack (bytes)             : 4044
Or stack (bytes)              : 4044
Search Stack (bytes)          : 4044
Constraint queue (bytes)      : 4104
Total memory used (bytes)     : 213288
Elapsed time since creation   : 0.329
   526485
 + 197485
 = 723970


donald 1 1

Number of fails               : 61
Number of choice points       : 63
Number of variables           : 11
Number of constraints         : 3
Reversible stack (bytes)      : 4044
Solver heap (bytes)           : 8064
Solver global heap (bytes)    : 4044
And stack (bytes)             : 4044
Or stack (bytes)              : 4044
Search Stack (bytes)          : 4044
Constraint queue (bytes)      : 4104
Total memory used (bytes)     : 32388
Elapsed time since creation   : 0.016
   526485
 + 197485
 = 723970


donald 2 1

Number of fails               : 1
Number of choice points       : 4
Number of variables           : 11
Number of constraints         : 9
Reversible stack (bytes)      : 4044
Solver heap (bytes)           : 20124
Solver global heap (bytes)    : 16104
And stack (bytes)             : 4044
Or stack (bytes)              : 4044
Search Stack (bytes)          : 4044
Constraint queue (bytes)      : 4104
Total memory used (bytes)     : 56508
Elapsed time since creation   : 0.016
   526485
 + 197485
 = 723970


donald 3 1

Number of fails               : 1
Number of choice points       : 4
Number of variables           : 11
Number of constraints         : 9
Reversible stack (bytes)      : 4044
Solver heap (bytes)           : 40224
Solver global heap (bytes)    : 16104
And stack (bytes)             : 4044
Or stack (bytes)              : 4044
Search Stack (bytes)          : 4044
Constraint queue (bytes)      : 4104
Total memory used (bytes)     : 76608
Elapsed time since creation   : 0.016
   526485
 + 197485
 = 723970


donald 4 1

Number of fails               : 1
Number of choice points       : 4
Number of variables           : 11
Number of constraints         : 9
Reversible stack (bytes)      : 4044
Solver heap (bytes)           : 168864
Solver global heap (bytes)    : 16104
And stack (bytes)             : 4044
Or stack (bytes)              : 4044
Search Stack (bytes)          : 4044
Constraint queue (bytes)      : 4104
Total memory used (bytes)     : 205248
Elapsed time since creation   : 0.328
   526485
 + 197485
 = 723970
*/

