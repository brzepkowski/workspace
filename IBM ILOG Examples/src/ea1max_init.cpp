// -------------------------------------------------------------- -*- C++ -*-
// File: examples/src/ea1max_init.cpp
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
    IloInt problemSize = 50;     // Bits per solution
    IloInt populationSize = 50;  // Number of solutions in population
    if (argc > 1)
      problemSize = atoi(argv[1]);
    if (argc > 2)
      populationSize = atoi(argv[2]);
    env.out() << "Problem size: " << problemSize << endl;
    env.out() << "Population size: " << populationSize << endl;

    // Create the model
    IloModel model(env);
    IloBoolVarArray bits(env, problemSize);     // The bits to count
    for (IloInt v = 0; v < problemSize; v++) {
      char name[10];
      sprintf(name, "%02ld", v + 1);
      bits[v].setName(name);                         // Name the variable
    }
    IloIntVar count(env, 0, problemSize, "count");   // The bit count
    model.add(count == IloSum(bits));                // Count the bits

    // Create the solution prototype
    IloSolution prototype(env);
    prototype.add(bits);
    IloObjective obj = IloMaximize(env, count, "Obj");
    prototype.setObjective(obj);

    // Initialize the population
    IloSolver solver(model);
    IloSolutionPool population(env);
    IloEAOperatorFactory factory(env, bits);
    factory.setPrototype(prototype);
    factory.setSearchLimit(IloFailLimit(env, problemSize));
    solver.solve(IloExecuteProcessor(
      env, factory.randomize("rand")(populationSize) >> population
    ));

    // Display the population
    env.out() << "INITIAL POPULATION " << population << endl;
  } catch(IloException ex) {
    env.out() << "Caught: " << ex << endl;
  }
  env.end();
  return 0;
}
