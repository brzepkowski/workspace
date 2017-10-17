// -------------------------------------------------------------- -*- C++ -*-
// File: examples/src/tutorial/ea1max_values.cpp
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

// This example shows how to:
//   - use a fitness based parent selector (roulette wheel)
//   - transform solution values used for selection
//   - compute solution values in the generation goal
//   - use a replacement goal which replaces a fraction of the pool
//     with offspring


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

// Stores invocation and improvement statistics for operators
struct OperatorStatistics {
  IloInt invocations;
  IloInt improvements;
  IloInt successes;
  OperatorStatistics() : invocations(0), improvements(0), successes(0) { }
};

// Retrieve operator statistics (and create if necessary)
OperatorStatistics * GetOperatorStatistics(IloPoolOperator op) {
  OperatorStatistics * stat = (OperatorStatistics *)op.getObject();
  if (stat == 0) {
    stat = new (op.getEnv()) OperatorStatistics();
    op.setObject(stat);
  }
  return stat;
}


// Setup the processor to operator mapping
IloPoolProc BuildGAProcessor(IloPoolOperator op) {
  IloPoolProc proc(op);
  proc.setObject(op.getImpl());
  return proc;
}

OperatorStatistics * GetOperatorStatistics(IloPoolProc proc) {
  return GetOperatorStatistics((IloPoolOperatorI*)proc.getObject());
}

// Defines a listener which records solution improvements
ILOIIMLISTENER2(ImprovementListener, IloPoolOperator::SuccessEvent, event,
                IloSolution&, best,
                IloInt&, generation) {
  IloSolution newSolution = event.getSolution();
  IloPoolOperator op = event.getOperator();
  OperatorStatistics * stat = GetOperatorStatistics(op);
  stat->successes++;
  if (newSolution.isBetterThan(best)) {
    best.end();
    best = newSolution.makeClone(getEnv());
    cout << " IMPROVEMENT " << best.getObjectiveValue()
         << " BY " << op.getDisplayName()
         << " GENERATION " << generation
         << endl;
    stat->improvements++;
  }
}

// Defines a listener which records operator invocations
ILOIIMLISTENER0(OperatorListener, IloPoolOperator::InvocationEvent, event) {
  IloPoolOperator op = event.getOperator();
  OperatorStatistics * stat = GetOperatorStatistics(op);
  stat->invocations++;
}

// Raise the result of an evaluator to a given power, and normalizes

// This comparator returns true iff "left" has generated
// more improvements than "right".
ILOCOMPARATOR0(OperatorImprovementComparator, IloPoolProc, left, right) {
  OperatorStatistics * s1 = GetOperatorStatistics(left);
  OperatorStatistics * s2 = GetOperatorStatistics(right);
  return s1->improvements > s2->improvements;
}

// This comparator returns true iff "left" has fewer invocations than "right"
ILOCOMPARATOR0(OperatorInvocationComparator, IloPoolProc, left, right) {
  OperatorStatistics * s1 = GetOperatorStatistics(left);
  OperatorStatistics * s2 = GetOperatorStatistics(right);
  return s1->invocations < s2->invocations;
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

    // Initialize the best solution generated at initialization time
    IloSolution best = population.getBestSolution().makeClone(env);

    // Evaluate the objective values of solutions

    // Increases discrepencies between solutions

    // Selects parents depending on their probabilities

    // Add the invocation listener
    factory.addListener(OperatorListener(env));

    // Add the improvement listener
    factory.addListener(ImprovementListener(env, best, generation));

    // Create a processor array store operators
    IloPoolProcArray ops(env);

    // Add the mutation operator to the pool
    ops.add(BuildGAProcessor(factory.mutate(1.0 / problemSize, "mutate")));

    // Add 1 and 2 point crossover

    // Add the uniform crossover operator to the pool
    ops.add(BuildGAProcessor(factory.uniformXover(0.5, "uXover")));

    // Add transposition operators

    // Creates a processor which will choose a sub-processor to apply
    IloPoolProc applyOp = IloSelectProcessor(
      env, ops, IloRandomSelector<IloPoolProc,IloPoolProcArray>(env)
    );

    // Create the reproduction goal

    // Create the selector of individuals to remove

    // Create the replacement goal

    // Each generation reproduces and replace solutions
    IloGoal generationGoal = reproduceGoal && replacementGoal;

    // Compute parent probabilities before each generation

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

    // Create composite operator comparator
    IloComparator<IloPoolProc> opComparator = IloComposeLexical(
       OperatorImprovementComparator(env),
       OperatorInvocationComparator(env)
    );

    // Sort the operator pool
    IloBool done;
    do {
      done = IloTrue;
      for (IloInt i = 1; i < ops.getSize(); i++) {
        if (opComparator.isBetterThan(ops[i], ops[i-1])) {
          IloPoolProc tmp = ops[i-1]; ops[i-1] = ops[i]; ops[i] = tmp;
          done = IloFalse;
        }
      }
    } while (!done);

    // Loop over the operator pool and display the pools
    for (IloInt i = 0; i < ops.getSize(); i++) {
      IloPoolProc op = ops[i];
      OperatorStatistics* stat = GetOperatorStatistics(op);
      env.out() << " " << op.getDisplayName()
                << " INVOKE " << stat->invocations
                << " SUCCESS " << stat->successes
                << " IMPROVE " << stat->improvements
                << endl;
    }
  } catch(IloException ex) {
    env.out() << "Caught: " << ex << endl;
  }
  env.end();
  return 0;
}
