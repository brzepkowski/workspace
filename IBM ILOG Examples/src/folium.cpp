// -------------------------------------------------------------- -*- C++ -*-
// File: ./examples/src/folium.cpp
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

Problem description
-------------------

Solve this system of equations:

                  x^2/y + y^2/x = 2
                  y = exp(-x)

x and y are floating-point numbers

------------------------------------------------------------ */

#include <ilsolver/ilosolverfloat.h>

ILOSTLBEGIN

int main() {
  IloEnv env;
  try {
    IloModel model(env);

    IloNumVar x(env, -1e30, 1e30);   // Declaring the variables
    IloNumVar y(env, -1e30, 1e30);

    model.add(x*x/y + y*y/x == 2.);  // Posting the constraints
    model.add(y == IloExponent(-x));

    IloSolver solver(model);
    solver.startNewSearch(IloDichotomize(env, x));

    while (solver.next()) {
      solver.out() << "x = " << solver.getFloatVar(x) << endl;
      solver.out() << "y = " << solver.getFloatVar(y) << endl << endl;
    }
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
x = [0.868418..0.868418]
y = [0.419615..0.419615]

x = [0.294563..0.294563]
y = [0.744857..0.744857]
*/
