// -------------------------------------------------------------- -*- C++ -*-
// File: ./examples/src/nqueen.cpp
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

Problem Description
-------------------

The eight-queens problem is a well known problem that involves
placing eight queens on a chess board in such a way that none
of them can capture any other using the conventional moves
allowed to a queen.  In other words, the problem is to select
eight squares on a chess board so that any pair of selected
squares is never aligned vertically, horizontally, nor
diagonally.

Of course, the problem can be generalized to a board of any
size.  In general terms, then, we have to select N squares on a
board with N squares on each side, still respecting the
constraints of non-alignment.

------------------------------------------------------------ */

#include <ilsolver/ilosolverint.h>

ILOSTLBEGIN

IlcChooseIndex2(IlcChooseMinSizeMin,
		var.getSize(),
		var.getMin(),
		IlcIntVar)

ILOCPGOALWRAPPER1(MyGenerate, solver, IloIntVarArray, vars) {
  return IlcGenerate(solver.getIntVarArray(vars), IlcChooseMinSizeMin);
}

int main(int argc, char** argv) {
  IloEnv env;
  try {
    IloModel mdl(env);

    IloInt nqueen = (argc > 1) ? atoi(argv[1]) : 100;
    IloIntVarArray x(env, nqueen, 0, nqueen-1);

    IloIntVarArray x1(env, nqueen, -2*nqueen, 2*nqueen);
    IloIntVarArray x2(env, nqueen, -2*nqueen, 2*nqueen);

    IloInt i;
    for (i = 0; i < nqueen; i++) {
      mdl.add(x1[i] == x[i]+i);
      mdl.add(x2[i] == x[i]-i);
    }

    mdl.add(IloAllDiff(env, x));
    mdl.add(IloAllDiff(env, x1));
    mdl.add(IloAllDiff(env, x2));

    IloSolver solver(mdl);

    if (solver.solve(MyGenerate(env, x))) {
      for (i = 0; i < nqueen && i <= 100; i++)
	solver.out() << solver.getValue(x[i]) << " ";
      if (nqueen > 100)
	solver.out() << "...";
      solver.out() << endl;
    }
    else
      solver.out()<< "No solution" << endl;

    solver.printInformation();
  }
  catch (IloException& ex) {
    cout << "Error: " << ex << endl;
  }
  env.end();
  return 0;
}

/*
0 67 1 50 2 54 3 60 4 52
5 53 6 73 7 61 8 56 9 55
10 58 11 57 12 66 13 62 14
59 15 79 16 64 17 63 18 69
19 65 20 68 21 81 22 70 23
71 24 72 25 80 26 74 27 75
28 76 29 77 30 78 31 96 32
87 33 83 34 82 35 85 36 84
37 90 38 86 39 91 40 88 41
89 42 93 43 92 44 99 45 94
46 97 95 47 98 48 51 49
Number of fails               : 8
Number of choice points       : 95
Number of variables           : 298
Number of constraints         : 203
Reversible stack (bytes)      : 305544
Solver heap (bytes)           : 145000
Solver global heap (bytes)    : 4044
And stack (bytes)             : 4044
Or stack (bytes)              : 4044
Search Stack (bytes)          : 4044
Constraint queue (bytes)      : 11160
Total memory used (bytes)     : 477880
Running time since creation   : 0.07
*/
