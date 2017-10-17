// -------------------------------------------------------------- -*- C++ -*-
// File: ./examples/src/carseq_basic_ex3.cpp
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

Assume that you have 20 cars to paint in four available colors: green,
yellow, blue, and white. Due to technical limitations on the assembly
line, no more than six cars can be painted green, exactly five cars must
be painted yellow, no more than seven cars can be painted blue, and no
less than 5 cars and no more than 12 cars can be painted white. The first
car off the assembly line cannot be painted green.

------------------------------------------------------------ */
#include <ilsolver/ilosolverint.h>

ILOSTLBEGIN
const char* Names[] = {"green", "yellow", "blue", "white"};
int main() {
  IloEnv env;
  try {
    IloModel model(env);
    const IloInt nbCars    = 20;
    const IloInt nbColors = 4;
    IloInt i;
    IloIntVarArray cars(env, nbCars, 0, nbColors-1);
    IloIntArray colors(env, 4, 0, 1, 2, 3);
    IloIntVarArray cards(env, nbColors);
    cards[0] = IloIntVar(env, 0, 6);
    cards[1] = IloIntVar(env, 5, 5);
    cards[2] = IloIntVar(env, 0, 7);
    cards[3] = IloIntVar(env, 5, 12);
    model.add (IloDistribute(env, cards, colors, cars));
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
Car 5 color:     green
Car 6 color:     green
Car 7 color:     green
Car 8 color:     yellow
Car 9 color:     yellow
Car 10 color:     yellow
Car 11 color:     yellow
Car 12 color:     blue
Car 13 color:     blue
Car 14 color:     blue
Car 15 color:     blue
Car 16 color:     white
Car 17 color:     white
Car 18 color:     white
Car 19 color:     white
Car 20 color:     white
Number of fails               : 0
Number of choice points       : 15
Number of variables           : 25
Number of constraints         : 2
Reversible stack (bytes)      : 4044
Solver heap (bytes)           : 12084
Solver global heap (bytes)    : 4044
And stack (bytes)             : 4044
Or stack (bytes)              : 4044
Search Stack (bytes)          : 4044
Constraint queue (bytes)      : 11152
Total memory used (bytes)     : 43456
Elapsed time since creation   : 0.16
*/

