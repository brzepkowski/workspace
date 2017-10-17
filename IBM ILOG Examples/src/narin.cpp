// -------------------------------------------------------------- -*- C++ -*-
// File: ./examples/src/narin.cpp
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

x and y are two constrained floating-point variables
k is a constrained integer variable

x is in [ -5 , 10^8 ]
y is in [  0 , 10^8 ]
k is in [ -1000 , 1000 ]

Solve the system of equations:

x^3 +10x = y^x - 2^k
kx + 7.7y = 2.4
(k -1)^(y+1) <= 10
((log(y + 2x + 12) <= k + 5) or (y >= k^2)) => (x <= 0 and y <= 1)
(x <= 0) => (k > 3)

------------------------------------------------------------ */

#include <ilsolver/ilosolverfloat.h>

ILOSTLBEGIN

int main(int argc, char** argv){
  IloEnv env;
  try {
    IloModel model(env);
    IloNumVar x(env, -5, 1e8);
    IloNumVar y(env, 0, 1e8);
    IloIntVar z(env, -1000, 1000);
    IloIntVar k(env, -1000, 1000);

    model.add(IloPower(x,3) + 10*x == IloPower(y, x) - IloPower(2, k));
    model.add(k*x + 7.7*y == 2.4);
    model.add(IloPower(k-1, y+1) <= 10);
    model.add(IloIfThen(env, IloLog(y + 2*x + 12) <= k + 5 || y >= k*k, x <= 0 && y <= 1));
    model.add(IloIfThen(env, x <= 0, k > 3));

    IloNumVarArray vars(env);
    vars.add(x);
    vars.add(y);

    IloSolver solver(env);

    solver.out().precision(16);

    solver.extract(model);

    if (solver.solve(IloGenerateBounds(env, vars, .1))) {
      solver.out() << "x = " <<  solver.getFloatVar(x) << endl;
      solver.out() << "y = " <<  solver.getFloatVar(y) << endl;
      solver.out() << "k = " <<  solver.getIntVar(k) << endl;
    }
    solver.printInformation();
  }
  catch (IloException& ex) {
    cout << "Error: " << ex << endl;
  }
  env.end();
  return 0;
}
/*
x = [-1.285057857942743..-1.285057857928474]
y = [0.9792508352918879..0.979250835302613]
k = [4]
*/
