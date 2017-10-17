// -------------------------------------------------------------- -*- C++ -*-
// File: ./examples/src/tutorial/color_partial.cpp
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

The problem involves choosing colors for the countries on a map in
such a way that at most four colors (blue, white, yellow, green) are
used and no neighboring countries are the same color. In this exercise,
you will find a solution for a map coloring problem with six countries:
Belgium, Denmark, France, Germany, Luxembourg, and the Netherlands.

------------------------------------------------------------ */

#include <ilsolver/ilosolverint.h>
ILOSTLBEGIN

const char* Names[] = {"blue", "white", "yellow", "green"};

int main(){
  //Create the environment
  try {
  //Create the model
    //Declare the decision variables
    //Add the constraints
    //Create an instance of IloSolver
    //Search for a solution
      {
      solver.out() << solver.getStatus() << " Solution" << endl;
      solver.out() << "Belgium:     "
                   << Names[(IloInt)solver.getValue(Belgium)]     << endl;
      solver.out() << "Denmark:     "
                   << Names[(IloInt)solver.getValue(Denmark)]     << endl;
      solver.out() << "France:      "
                   << Names[(IloInt)solver.getValue(France)]      << endl;
      solver.out() << "Germany:     "
                   << Names[(IloInt)solver.getValue(Germany)]     << endl;
      solver.out() << "Luxembourg:  "
                   << Names[(IloInt)solver.getValue(Luxembourg)]  << endl;
      solver.out() << "Netherlands: "
                   << Names[(IloInt)solver.getValue(Netherlands)] << endl;
     }
  }
  catch (IloException& ex) {
    cout << "Error: " << ex << endl;
  }
  env.end();
  return 0;
}
