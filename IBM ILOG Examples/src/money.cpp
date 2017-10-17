// -------------------------------------------------------------- -*- C++ -*-
// File: ./examples/src/money.cpp
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

Assume that you want to pay for an item that costs 1.23 euros. You have a
mixture of coins: one euro cent, ten euro cents, twenty euro cents, and one
euro. To make the problem more interesting, assume that you have only 5 coins
of one euro cent.

------------------------------------------------------------ */
#include <ilsolver/ilosolverint.h>
ILOSTLBEGIN

int main() {
  IloEnv env;
  try {
    IloModel model(env);
    IloInt nCoins = 4, Sum = 123;
    IloIntArray coeffs(env, nCoins, 1, 10, 20, 100);
    env.out() << coeffs << endl;
    IloIntVarArray coins(env, nCoins, 0, Sum);
    model.add(coins[0] <= 5);
    model.add(IloScalProd(coins, coeffs) == Sum);
    IloSolver solver(model);
    solver.startNewSearch();
    IloInt solutionCounter = 0;
    while(solver.next()) {
      solver.out() << "solution  " << ++solutionCounter << ":\t";
      for (IloInt i = 0; i < nCoins; i++)
        solver.out() << solver.getValue(coins[i]) << " ";
      solver.out() << endl;
    }
    solver.endSearch();
  }
  catch (IloException& ex) {
    cout << "Error: " << ex << endl;
  }
  env.end();
  return 0;
}
/*
[1, 10, 20, 100]
solution  1:    3 0 1 1
solution  2:    3 0 6 0
solution  3:    3 2 0 1
solution  4:    3 2 5 0
solution  5:    3 4 4 0
solution  6:    3 6 3 0
solution  7:    3 8 2 0
solution  8:    3 10 1 0
solution  9:    3 12 0 0
*/
