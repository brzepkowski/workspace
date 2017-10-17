// -------------------------------------------------------------- -*- C++ -*-
// File: ./examples/src/tutorial/money_partial.cpp
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
  //Create the environment
  try {
    //Create the model
    IloInt nCoins = 4, Sum = 123;
    //Declare the array of coefficients
    env.out() << coeffs << endl;
    //Declare the decision variables
    //Add the constraint using IloScalProd
    //Add the constraint on the number of 1 cent coins
    //Create an instance of IloSolver
    //Search for a solution
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

