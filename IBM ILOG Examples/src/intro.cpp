// -------------------------------------------------------------- -*- C++ -*-
// File: ./examples/src/intro.cpp
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

The problem is to find values for x and y from the following information:
x + y = 17
x - y = 5
x can be any integer from 5 through 12
y can be any integer from 2 through 17

------------------------------------------------------------ */

#include <ilsolver/ilosolverint.h>
ILOSTLBEGIN

int main(){
  IloEnv env;
  try {
    IloModel model(env);
    IloIntVar x(env, 5, 12);
    IloIntVar y(env, 2, 17);
    model.add(x + y == 17);
    model.add(x - y == 5);
    IloSolver solver(model);
    if (solver.solve()){
      solver.out() << "x = " << solver.getValue(x) << endl;
      solver.out() << "y = " << solver.getValue(y) << endl;
    }
  }
  catch (IloException& ex) {
    cout << "Error: " << ex << endl;
  }
  env.end();
  return 0;
}
/*
x = 11
y = 6
*/
