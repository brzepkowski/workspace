// -------------------------------------------------------------- -*- C++ -*-
// File: ./examples/src/equation.cpp
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
------------------

Solve the equation:   x + x^3 +exp(x) = 10   with x in [1,10]

------------------------------------------------------------ */

#include <ilsolver/ilosolverfloat.h>

ILOSTLBEGIN

int main(){
  IloEnv env;
  try {
    IloModel model(env);

    IloNumVar x (env, 1, 10);  // Declaring the variable

    // Adding the constraint to the model
    model.add(x + IloPower(x, 3L) + IloExponent(x) == 10.);

    IloSolver solver(model);

    if (solver.solve())
      solver.out() << "x = " << solver.getValue(x) << endl;
    solver.printInformation();
  }
  catch (IloException& ex) {
    cout << "Error: " << ex << endl;
  }
  env.end();
  return 0;
}

/*
x = 1.55113
*/









