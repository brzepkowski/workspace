// -------------------------------------------------------------- -*- C++ -*-
// File: examples/src/tutorial/ea1max_gen_partial.cpp
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
//   - use a predefined selector
//   - specify a genetic operator
//   - replace the existing population by offspring
//   - access the best and worst solutions in the population
//


// Display the generation statistics

int main(int argc, const char * argv[]) {
  IloEnv env;
  try {
    IloInt problemSize = 50;     // Bits per solution
    IloInt populationSize = 50;  // Number of solutions in population
    IloInt maxGeneration = 60;   // Generation count limit
    if (argc > 1)
      problemSize = atoi(argv[1]);
    if (argc > 2)
      populationSize = atoi(argv[2]);
    if (argc > 3)
      maxGeneration = atoi(argv[3]);
    env.out() << "Problem size: " << problemSize << endl;
    env.out() << "Population size: " << populationSize << endl;
    env.out() << "Max generation: " << maxGeneration << endl;

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

    // The generation count
    IloInt generation = 0;

    // Initialize the population
    IloSolver solver(model);
    IloSolutionPool population(env);
    IloEAOperatorFactory factory(env, bits);
    factory.setPrototype(prototype);
    factory.setSearchLimit(IloFailLimit(env, problemSize));
    solver.solve(IloExecuteProcessor(
      env, factory.randomize("rand")(populationSize) >> population
    ));

    DisplayGenerationStatistics(env.out(), generation, population);

    // Use tournament selection to choose parents

    // Use mutation as a single operator

    // Create the reproduction goal

    // Replace the entire population with offspring

    // Each generation reproduces and replace solutions

    // Setup the optimum value

    // The main generational loop
  } catch(IloException ex) {
    env.out() << "Caught: " << ex << endl;
  }
  env.end();
  return 0;
}
