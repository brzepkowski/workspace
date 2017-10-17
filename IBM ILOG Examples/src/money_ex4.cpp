// -------------------------------------------------------------- -*- C++ -*-
// File: ./examples/src/money_ex4.cpp
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

 You are the manager of a coffee shop. You sell a house blend that is
 composed of the following five types of coffee: Costa Rican (cost per
 unit is $2), Hawaiian (cost per unit $5), Jamaican (cost per unit $3),
 Kenyan (cost per unit $9), and Nicaraguan (cost per unit $4). You need
 to make a batch of the house blend. In this batch, you must use at
 least 2 units of each type of coffee. You can use no more than 3 units
 of the Costa Rican coffee, no more than 4 units of the Kenyan coffee,
 and no more than 3 units of the Jamaican coffee. You want the total cost
 for the batch of house blend to be equal to $100.


------------------------------------------------------------ */
#include <ilsolver/ilosolverint.h>
ILOSTLBEGIN

int main() {
  IloEnv env;
  try {
    IloModel model(env);
    IloInt nCoffee = 5, Sum = 100;
    IloIntArray coeffs(env, nCoffee, 2, 5, 3, 9, 4);
    env.out() << coeffs << endl;
    IloIntVarArray units(env, nCoffee, 0, Sum);
    model.add(units[0] >= 2);
    model.add(units[1] >= 2);
    model.add(units[2] >= 2);
    model.add(units[3] >= 2);
    model.add(units[4] >= 2);
    model.add(units[0] <= 3);
    model.add(units[2] <= 3);
    model.add(units[4] <= 4);
    model.add(IloScalProd(units, coeffs) == Sum);
    IloSolver solver(model);
    solver.startNewSearch();
    IloInt solutionCounter = 0;
    while(solver.next()) {
      solver.out() << "solution  " << ++solutionCounter << ":\t";
      for (IloInt i = 0; i < nCoffee; i++)
        solver.out() << solver.getValue(units[i]) << " ";
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
[2, 5, 3, 9, 4]
solution 1:     2 2 2 8 2
solution 2:     2 3 2 7 3
solution 3:     2 4 2 6 4
solution 4:     2 5 3 6 2
solution 5:     2 6 3 5 3
solution 6:     2 7 3 4 4
solution 7:     2 11 2 3 2
solution 8:     2 12 2 2 3
solution 9:     3 2 3 7 3
solution 10:    3 3 3 6 4
solution 11:    3 7 2 5 2
solution 12:    3 8 2 4 3
solution 13:    3 9 2 3 4
solution 14:    3 10 3 3 2
solution 15:    3 11 3 2 3
*/
