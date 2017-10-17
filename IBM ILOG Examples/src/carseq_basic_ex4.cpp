// -------------------------------------------------------------- -*- C++ -*-
// File: ./examples/src/carseq_basic_ex4.cpp
// --------------------------------------------------------------------------
// Licensed Materials - Property of IBM
// 5724-Y47
// Copyright IBM Corporation 1990, 2009. All Rights Reserved.
//
// Note to U.S. Government Users Restricted Rights:
// Use, duplication or disclosure restricted by GSA ADP Schedule
// Contract with IBM Corp.
// --------------------------------------------------------------------------

/* --------------------------------------------------------------------------

Problem Description
-------------------

A magic sequence is a sequence of N+1 values (x0, x1, ... xN)
such that 0 will appear in the sequence x0 times, 1 will appear
x1 times, ... and N will appear in the sequence xN times.

For example, for N=3, the following sequence is a solution:
(1, 2, 1, 0).  That is, 0 is present once, 1 is present twice,
2 is present once, and 3 is not present (present zero times,
as it were).

------------------------------------------------------------ */
# include <ilsolver/ilosolverint.h>

ILOSTLBEGIN

int main(int argc, char** argv){
  IloEnv env;
  try {
    IloModel model(env);
    IloInt n = (argc > 1) ? atoi(argv[1]) : 10;
    IloInt i;
    IloIntVarArray vars(env, n + 1, 0, n + 1);
    IloIntArray coeffs(env, n + 1);
    for (i = 0; i < n + 1; i++)
      coeffs[i] = i;
    model.add(IloDistribute(env, vars, coeffs, vars));
    // redundant constraint
    model.add(IloScalProd(vars,coeffs) == n + 1);
    IloSolver solver(model);
    if (solver.solve(IloGenerate(env, vars, IloChooseMinSizeInt))) {
      for (i = 0; i < n + 1; i++)
        solver.out() << solver.getValue(vars[i]) << " ";
      solver.out() << endl;
    }
    else
      solver.out() << "No solution" << endl;
    solver.printInformation();
  }
  catch (IloException& ex) {
    cerr << "Error: " << ex << endl;
  }
  env.end();
  return 0;
}
/*
7 2 1 0 0 0 0 1 0 0 0
Number of fails               : 5
Number of choice points       : 6
Number of variables           : 23
Number of constraints         : 14
Reversible stack (bytes)      : 4044
Solver heap (bytes)           : 12084
Solver global heap (bytes)    : 8088
And stack (bytes)             : 4044
Or stack (bytes)              : 4044
Search Stack (bytes)          : 4044
Constraint queue (bytes)      : 11144
Total memory used (bytes)     : 47492
Running time since creation   : 0.01
*/
