// -------------------------------------------------------------- -*- C++ -*-
// File: ./examples/src/colormin_ex4.cpp
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

Assume that you want to pay for an item that costs 1.23 euros. You
have a mixture of coins: one euro cent, ten euro cents, twenty
euro cents, and one euro. To make the problem more interesting, assume
that you have only 5 coins of one euro cent.

Minimize the number of coins used.

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
    IloObjective obj = IloMinimize(env, coins[0] + coins[1] + coins[2] + coins[3]);
    IloSolver solver(model);
      if (solver.solve())
        {
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
3 0 1 1
*/
