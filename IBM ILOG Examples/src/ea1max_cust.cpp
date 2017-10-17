// -------------------------------------------------------------- -*- C++ -*-
// File: examples/src/ea1max_cust.cpp
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
//   - define a customized selector for parents
//   - use mutiple operators to generate offspring
//


// Display the generation statistics
void DisplayGenerationStatistics(ostream& stream,
                                 IloInt generation,
                                 IloSolutionPool population) {
  IloNum sum = 0.0;
  for (IloSolutionPool::Iterator it(population); it.ok(); ++it)
    sum += (*it).getObjectiveValue();
  stream << "GENERATION " << generation
         << " WORST "
         << population.getWorstSolution().getObjectiveValue()
         << " BEST "
         << population.getBestSolution().getObjectiveValue()
         << " AVERAGE " << (sum / population.getSize())
         << endl;
}

// The custom selector
ILOCTXSELECTOR1(CustomSelector, IloSolution, IloSolutionPool, in,
                IloSolver, solver, IlcInt, tournamentSize) {
  IloInt size = in.getSize();
  if (size != 0) {
    IlcRandom rnd = solver.getRandom();
    IloSolution candidate = in.getSolution(rnd.getInt(size));
    for (IlcInt tournament = 1; tournament < tournamentSize; tournament++) {
      IloSolution competition = in.getSolution(rnd.getInt(size));
      if (competition.isBetterThan(candidate))
        candidate = competition;
    }
    select(candidate);
  }
}

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

    // Create the user-defined tournament selector
    IloPoolProc selector =
      IloSelectSolutions(env, CustomSelector(env, 3), IloTrue);

    // Create a processor array store operators
    IloPoolProcArray ops(env);

    // Add the mutation operator to the pool
    ops.add(factory.mutate(1.0 / problemSize, "mutate"));

    // Add the uniform crossover operator to the pool
    ops.add(factory.uniformXover(0.5, "uXover"));

    // Creates a processor which will choose the sub-processor to apply
    IloPoolProc applyOp = IloSelectProcessor(
      env, ops, IloRandomSelector<IloPoolProc,IloPoolProcArray>(env)
    );

    // Create the reproduction goal
    IloSolutionPool offspring(env, "offspring");
    IloGoal reproduceGoal = IloExecuteProcessor(
      env, population >> selector >> applyOp(populationSize) >> offspring
    );

    // Replace the entire population with offspring
    IloGoal replacementGoal =
      IloExecuteProcessor(env, population >> IloDestroyAll(env)) &&
      IloExecuteProcessor(env, offspring >> population);

    // Each generation reproduces and replace solutions
    IloGoal generationGoal = reproduceGoal && replacementGoal;

    // Setup the optimum value
    IloInt optimumValue = problemSize;
    env.out() << "Optimum value: " << optimumValue << endl;

    // The main generational loop
    for (generation++; generation < maxGeneration; generation++) {
      solver.solve(generationGoal);
      DisplayGenerationStatistics(env.out(), generation, population);
      if (population.getBestSolution().getObjectiveValue() == optimumValue) {
        env.out() << "OPTIMUM FOUND " << endl;
        break;
      }
    }
  } catch(IloException ex) {
    env.out() << "Caught: " << ex << endl;
  }
  env.end();
  return 0;
}
