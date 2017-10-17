// -------------------------------------------------------------- -*- C++ -*-
// File: ./examples/src/tutorial/carseq_basic_partial.cpp
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

Assume that you have 8 cars to paint in three available colors: green,
yellow, and blue. Due to technical limitations on the assembly
line, no more than three cars can be painted green, exactly three cars must
be painted yellow, and no more than two cars can be painted blue. The first
car off the assembly line cannot be painted green.
// --------------------------------------------------------------------------*/

#include <ilsolver/ilosolverint.h>

ILOSTLBEGIN

int main() {
  IloEnv env;
  try {
    IloModel model(env);
    const IloInt nbCars    = 8;
    const IloInt nbColors = 3;
    IloInt i;
    //Declare the decision variables
    //Declare the array of values
    //Declare the array of decision variables cards
    //Add the distribute constraint
    //Add the constraint that the first car cannot be green
    //Create an instance of IloSolver
    //Search for a solution
      {
      solver.out() << solver.getStatus() << " Solution" << endl;
      for (i = 0; i < nbCars; i++) {
        solver.out() << "Car " << i+1 <<  " color:     "
                     << Names[(IloInt)solver.getValue(cars[i])]
                     << endl;
      }
    }
    else
      solver.out() << "No solution " << endl;
      solver.printInformation();

  }
  catch (IloException& ex) {
    cout << "Error: " << ex << endl;
  }
  env.end();
  return 0;
}
