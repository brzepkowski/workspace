// -------------------------------------------------------------- -*- C++ -*-
// File: ./examples/src/carseq_basic.cpp
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

Assume that you have 8 cars to paint in three available colors: green,
yellow, and blue. Due to technical limitations on the assembly
line, no more than three cars can be painted green, exactly three cars must
be painted yellow, and no more than two cars can be painted blue. The first
car off the assembly line cannot be painted green.
--------------------------------------------------------------------------*/
#include <ilsolver/ilosolverint.h>

ILOSTLBEGIN
const char* Names[] = {"green", "yellow", "blue"};
int main() {
  IloEnv env;
  try {
    IloModel model(env);
    const IloInt nbCars    = 8;
    const IloInt nbColors = 3;
    IloInt i;
    IloIntVarArray cars(env, nbCars, 0, nbColors-1);
    IloIntArray colors(env, 3, 0, 1, 2);
    IloIntVarArray cards(env, nbColors);
    cards[0] = IloIntVar(env, 0, 3);
    cards[1] = IloIntVar(env, 3, 3);
    cards[2] = IloIntVar(env, 0, 2);
    model.add(IloDistribute(env, cards, colors, cars));
    model.add(cars[0] != 0);
    IloSolver solver(model);
    if (solver.solve(IloGenerate(env, cars, IloChooseMinSizeInt)))
      {
      solver.out() << solver.getStatus() << " Solution" << endl;
      for (i = 0; i < nbCars; i++) {
        solver.out() << "Car " << i+1 <<  " color:     "
                     << Names[(IloInt)solver.getValue(cars[i])]
                     << endl;
      }
    }
    else
      solver.out() << "No solution " << endl;
      solver.printInformation();
  }
  catch (IloException& ex) {
    cout << "Error: " << ex << endl;
  }
  env.end();
  return 0;
}
/*
Feasible Solution
Car 1 color:     yellow
Car 2 color:     green
Car 3 color:     green
Car 4 color:     green
Car 5 color:     yellow
Car 6 color:     yellow
Car 7 color:     blue
Car 8 color:     blue
Number of fails               : 0
Number of choice points       : 6
Number of variables           : 12
Number of constraints         : 2
Reversible stack (bytes)      : 4044
Solver heap (bytes)           : 8064
Solver global heap (bytes)    : 4044
And stack (bytes)             : 4044
Or stack (bytes)              : 4044
Search Stack (bytes)          : 4044
Constraint queue (bytes)      : 11152
Total memory used (bytes)     : 39436
Elapsed time since creation   : 0.17
*/

