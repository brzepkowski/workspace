// -------------------------------------------------------------- -*- C++ -*-
// File: ./examples/src/wilkins.cpp
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

Solve the equation:

          (x + 1)(x + 2)...(x + 20) + 2^(-23)x^19 = 0

with x in [-1e10, 1e10]

------------------------------------------------------------ */

#include <ilsolver/ilosolverfloat.h>

ILOSTLBEGIN

int main () {
  IloEnv env;
  try {
    IloModel model(env);

    IloNumVar x(env, -1e10, 1e10);

    IloExpr y = x + 1;
    for(IloInt i = 2; i <= 20; i++)
      y = y * (x + i);

    model.add(y + IloPower(2, -23) * IloPower(x, 19) == 0);
    y.end();

    IloSolver solver(env);
    solver.setDefaultPrecision(1e-8);

    solver.out().precision(8);

    solver.extract(model);

    solver.startNewSearch(IloGenerateBounds(env, x, 1e-5) &&
                          IloDichotomize(env, x));

    while (solver.next())
      solver.out() << "x = " << solver.getFloatVar(x) << endl;
    solver.printInformation();
    solver.endSearch();
  }
  catch (IloException& ex) {
    cout << "Error: " << ex << endl;
  }
  env.end();
  return 0;
}
/*
x = [-1..-1]
x = [-2..-2]
x = [-3..-3]
x = [-4..-4]
x = [-4.9999999..-4.9999999]
x = [-6.0000069..-6.0000069]
x = [-6.9996972..-6.9996972]
x = [-8.0072676..-8.0072676]
x = [-8.9172503..-8.9172502]
x = [-20.846908..-20.846908]
*/








