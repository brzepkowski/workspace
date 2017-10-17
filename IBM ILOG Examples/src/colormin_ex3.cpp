// -------------------------------------------------------------- -*- C++ -*-
// File: ./examples/src/colormin_ex3.cpp
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
mixture of coins of the following types: 1 euro cent, 10 euro cents,
20 euro cents, and 1 euro. To make the problem more interesting, assume
that you have only 5 coins of 1 euro cent. You prefer to pay for the item
using as many of your lower denomination coins as possible.

Create an objective that uses the following coefficients to represent
the importance of using each type of coin: 1000 (1 euro cent),
800 (10 euro cents), 300 (20 euro cents), and 10 (1 euro). Change the
code so that you only search for the one maximized solution.
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
    IloObjective obj = IloMaximize(env, (1000 * (coins[0]))
                                      +  (800 * (coins[1]))
                                      +  (300 * (coins[2]))
                                      +   (10 * (coins[3])));
    model.add(obj);
    IloSolver solver(model);
    if (solver.solve())
      {
      solver.out() << solver.getStatus() << " Solution" << endl;
      solver.out() << "Objective = " << solver.getValue(obj) << endl;
      for (IloInt i = 0; i < nCoins; i++)
        solver.out() << solver.getValue(coins[i]) << " ";
        solver.out() << endl;
    }
    else
      solver.out() << "No Solution" << endl;
  }
  catch (IloException& ex) {
    cout << "Error: " << ex << endl;
  }
  env.end();
  return 0;
}
/*
[1, 10, 20, 100]
Optimal Solution
Objective = 12600
3 12 0 0
*/
