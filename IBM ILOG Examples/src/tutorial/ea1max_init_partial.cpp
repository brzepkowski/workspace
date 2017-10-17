// -------------------------------------------------------------- -*- C++ -*-
// File: examples/src/tutorial/ea1max_init_partial.cpp
// --------------------------------------------------------------------------
// Licensed Materials - Property of IBM
// 5724-Y47
// Copyright IBM Corporation 1990, 2009. All Rights Reserved.
//
// Note to U.S. Government Users Restricted Rights:
// Use, duplication or disclosure restricted by GSA ADP Schedule
// Contract with IBM Corp.
// --------------------------------------------------------------------------

#include <ilsolver/iim.h>

ILOSTLBEGIN

//
// This example shows how to:
//  - build a solution prototype
//  - instantiate an accessor for use with predefined genetic operators
//  - create a solution pool
//  - perform a goal which fills the solution pool with random solutions
//

int main(int argc, const char * argv[]) {
  IloEnv env;
  try {
    // Parse command line parameters

    // Create the model

    // Create the solution prototype

    // Initialize the population

    // Display the population
  } catch(IloException ex) {
    env.out() << "Caught: " << ex << endl;
  }
  env.end();
  return 0;
}
