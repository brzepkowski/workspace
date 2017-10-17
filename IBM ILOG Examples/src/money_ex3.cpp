// -------------------------------------------------------------- -*- C++ -*-
// File: ./examples/src/money_ex3.cpp
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

Assume that you want to pay for an item that costs 1.23 euros.
You have a mixture of coins: one euro cent, two euro cents,
five euro cents, ten euro cents, twenty euro cents, and
one euro. To make the problem more interesting, assume that
you have only 5 coins of one euro cent, 5 coins of 2 euro cents,
and 10 coins of 5 euro cents. You must also use at least 1 coin
of one euro (100 cents).

------------------------------------------------------------ */
#include <ilsolver/ilosolverint.h>
ILOSTLBEGIN

int main() {
  IloEnv env;
  try {
    IloModel model(env);
    IloInt nCoins = 6, Sum = 123;
    IloIntArray coeffs(env, nCoins, 1, 2, 5, 10, 20, 100);
    env.out() << coeffs << endl;
    IloIntVarArray coins(env, nCoins, 0, Sum);
    model.add(coins[0] <= 5);
    model.add(coins[1] <= 5);
    model.add(coins[2] <= 10);
    model.add(coins[5] >= 1);
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
[1, 2, 5, 10, 20, 100]
solution  1:    0 4 1 1 0 1
solution  2:    0 4 3 0 0 1
solution  3:    1 1 0 0 1 1
solution  4:    1 1 0 2 0 1
solution  5:    1 1 2 1 0 1
solution  6:    1 1 4 0 0 1
solution  7:    2 3 1 1 0 1
solution  8:    2 3 3 0 0 1
solution  9:    3 0 0 0 1 1
solution  10:   3 0 0 2 0 1
solution  11:   3 0 2 1 0 1
solution  12:   3 0 4 0 0 1
solution  13:   3 5 0 1 0 1
solution  14:   3 5 2 0 0 1
solution  15:   4 2 1 1 0 1
solution  16:   4 2 3 0 0 1
solution  17:   5 4 0 1 0 1
solution  18:   5 4 2 0 0 1
*/
