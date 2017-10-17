// -------------------------------------------------------------- -*- C++ -*-
// File: examples/src/eacarseq.cpp
// --------------------------------------------------------------------------
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

// A structure to hold instance data
struct CarSeqProblem {
  IloEnv _env;
  IloInt _numberOfCars;
  IloInt _numberOfOptions;
  IloInt _numberOfConfigurations;
  IloInt _slack;

  IloIntArray _optionCapacity;
  IloIntArray _optionSequenceLength;

  IloIntArray            _numberOfCarsPerConfiguration;
  IloArray<IloBoolArray> _configurationNeedsOption;

  void read(istream& stream, IloInt slack);
  CarSeqProblem(IloEnv env) : _env(env) { }
};

// Read in a problem from a file
void CarSeqProblem::read(istream & stream, IloInt slack) {
  _slack = slack;

  // Dimension problem
  stream >> _numberOfCars >> _numberOfOptions >> _numberOfConfigurations;

  // setup data structures
  _optionCapacity = IloIntArray(_env, _numberOfOptions);
  _optionSequenceLength = IloIntArray(_env, _numberOfOptions);
  _numberOfCarsPerConfiguration =
    IloIntArray(_env, _numberOfConfigurations + 1);
  _configurationNeedsOption =
    IloArray<IloBoolArray>(_env, _numberOfConfigurations + 1);
  IloInt i;
  for (i = 0; i < _numberOfConfigurations + 1; i++)
    _configurationNeedsOption[i] = IloBoolArray(_env, _numberOfOptions);

  // read problem body
  for (i = 0; i < _numberOfOptions; i++)
    stream >> _optionCapacity[i];
  for (i = 0; i < _numberOfOptions; i++)
    stream >> _optionSequenceLength[i];
  for (i = 0; i < _numberOfConfigurations; i++) {
    IloInt id, ncars;
    stream >> id >> ncars;
    id++; // ids to begin at 1 (0 is empty configuration)
    _numberOfCarsPerConfiguration[id] = ncars;
    for (IloInt j = 0; j < _numberOfOptions; j++)
      stream >> _configurationNeedsOption[id][j];
  }

  // Setup empty configuration.
  _numberOfCarsPerConfiguration[0] = _slack;
  assert(IloSum(_numberOfCarsPerConfiguration) == _numberOfCars + _slack);
  for (i = 0; i < _numberOfOptions; i++)
    _configurationNeedsOption[0][i] = IloFalse;
}

// Build the Concert model
IloModel BuildModel(CarSeqProblem problem, IloIntVarArray seq) {
  IloEnv env = problem._env;
  IloModel model(env);

  // Global distribute.  Ensures that the correct number of cars
  // with each configration is built.
  IloIntVarArray cards(env, 1 + problem._numberOfConfigurations);
  IloInt i;
  for (i = 0; i < problem._numberOfConfigurations + 1; i++) {
    IloInt nb = problem._numberOfCarsPerConfiguration[i];
    cards[i] = IloIntVar(env, nb, nb);
  }
  model.add(IloDistribute(env, cards, seq));

  // Sequence constraints.  Ensure that no sub-sequence contains
  // more of a given options that allowed
  for (IloInt opt = 0; opt < problem._numberOfOptions; opt++) {
    // Build an array of integers which represent configurations
    // which require option opt.
    IloIntArray pertinentConfigs(env);
    for (i = 0; i <= problem._numberOfConfigurations; i++) {
      if (problem._configurationNeedsOption[i][opt])
        pertinentConfigs.add(i);
    }

    // use us a boolean array for which use[i] = true iff
    // seq[i] has a configuration which requires opt
    IloBoolVarArray use(env, seq.getSize());
    model.add(IloBoolAbstraction(env, use, seq, pertinentConfigs));

    // Limit each subsequence to the correct number of occurrences
    // Loop over all subsequences of the correct sequence length
    // for option opt.  Add a sum to limit the maximum number of
    // occurrences in each sub-sequence.
    IloInt sl = problem._optionSequenceLength[opt];
    IloInt max = seq.getSize() - sl;
    IloInt x;
    for (x = 0; x <= max; x++) {
      IloBoolVarArray subUse(env, sl);
      for (i = 0; i < sl; i++)
        subUse[i] = use[x + i];
      model.add(IloSum(subUse) <= problem._optionCapacity[opt]);
    }
  }
  return model;
}

// Build the cost variable
IloIntVar BuildCostVar(IloModel model, IloIntVarArray seq, IloInt slack) {
  IloEnv env(model.getEnv());
  IloIntVar cost(env, 0, slack);
  IloConstraintArray carAfter(env, slack);
  for (IloInt i = slack - 1; i >= 0; i--) {
    IlcInt j = seq.getSize() - slack + i;
    carAfter[i] = seq[j] != 0;
    if (i < slack - 1)
      carAfter[i] = carAfter[i] || carAfter[i + 1];
    model.add(carAfter[i] == cost > i);
  }
  return cost;
}

// Useful type definitions
typedef IlcRevInt * IlcRevIntPtr;
ILCARRAY(IlcRevIntPtr)
typedef IlcRevIntPtrArray IlcRevIntArray;

ILCARRAY(IlcIntArray)
typedef IlcIntArrayArray IlcIntArray2;

// Class to produce configs in priority order
class ConfigurationPriorities {
private:
  IlcIntArray2   _priorities;
  IlcRevIntArray _index;

  static void Sort(IlcIntArray a);
public:
  ConfigurationPriorities(CarSeqProblem problem, IlcIntVarArray prio);
  IlcInt getHighestPriority(IlcInt conf) const {
    return _priorities[conf][_index[conf]->getValue()];
  }
  void markScheduled(IlcInt conf) {
    IloSolver solver(_priorities.getSolver());
    _index[conf]->setValue(solver, _index[conf]->getValue() + 1);
  }
};

// Build configuration priorities
ConfigurationPriorities
::ConfigurationPriorities(CarSeqProblem problem, IlcIntVarArray prio) {
  IloSolver solver(prio.getSolver());
  IlcInt numConf = problem._numberOfConfigurations;

  // Create array of sets
  _priorities = IlcIntArray2(solver, 1 + numConf);
  _index = IlcRevIntArray(solver, 1 + numConf);

  IlcInt k = 0;
  for (IlcInt conf = 1; conf <= numConf; conf++) {
    IlcInt numCars = problem._numberOfCarsPerConfiguration[conf];
    _priorities[conf] = IlcIntArray(solver, numCars);
    for (IlcInt j = 0; j < numCars; j++)
      _priorities[conf][j] = prio[k++].getValue();
    Sort(_priorities[conf]);
    _index[conf] = new (solver.getHeap()) IlcRevInt(solver, 0);
  }
}

// Sort integers in decreasing order
void ConfigurationPriorities::Sort(IlcIntArray a) {
  for (IlcInt i = 0; i < a.getSize() - 1; i++) {
    IlcInt big = i;
    for (IlcInt j = i + 1; j < a.getSize(); j++)
      if (a[j] > a[big]) big = j;
    IlcInt tmp = a[i]; a[i] = a[big]; a[big] = tmp;
  }
}

// Decode priorities to a sequence
ILCGOAL3(DecodeSlave, ConfigurationPriorities *, priorities,
                      IlcIntVarArray, seq,
                      IlcInt, idx) {
  IlcInt i;
  for (i = idx; i < seq.getSize() && seq[i].isBound(); i++)
    if (seq[i].getValue() != 0)
      priorities->markScheduled(seq[i].getValue());

  // No unbound slots, found a solution
  if (i == seq.getSize())
    return 0;

  // Decide on a configuration
  IloInt bestPrio = -1;
  IloInt bestConf = 0;
  for (IlcIntExpIterator it(seq[i]); it.ok(); ++it) {
    IlcInt conf = *it;
    if (conf != 0) {
      IlcInt p = priorities->getHighestPriority(conf);
      if (p > bestPrio) {
        bestPrio = p;
        bestConf = conf;
      }
    }
  }

  // Instantiate remainder of sequence
  IlcGoal loop = DecodeSlave(getSolver(), priorities, seq, i);
  return IlcAnd(IlcOr(seq[i] == bestConf, seq[i] != bestConf), loop);
}

// Build the priority structure
ILCGOAL3(Decode,
         CarSeqProblem, problem,
         IlcIntVarArray, prio,
         IlcIntVarArray, seq) {
  IloSolver solver(getSolver());
  ConfigurationPriorities * cp = new (solver.getHeap())
    ConfigurationPriorities(problem, prio);
  return DecodeSlave(solver, cp, seq, 0);
}

// An ILO wrapper for the decoding goal
ILOCPGOALWRAPPER3(Decode, solver,
                  CarSeqProblem, problem,
                  IloIntVarArray, prio,
                  IloIntVarArray, seq) {
  IlcIntVarArray solverPrio = solver.getIntVarArray(prio);
  IlcIntVarArray solverSeq = solver.getIntVarArray(seq);
  return Decode(solver, problem, solverPrio, solverSeq);
}

// Force improvement of offspring
ILOIIMOP2(ImproveOn, AnySize, IlcFloat, p, IlcFloat, q) {
  IloSolutionPool pool = getInputPool();
  IloIntVar var = pool.getSolution(0).getObjectiveVar();
  IlcInt limit = IlcIntMax;
  if (getSolver().getRandom().getFloat() < p)
    limit = (IloInt)pool.getBestSolution().getObjectiveValue() - 1;
  else if (getSolver().getRandom().getFloat() < q)
    limit = (IloInt)pool.getWorstSolution().getObjectiveValue() - 1;
  return getSolver().getIntVar(var) <= limit;
}

// The replacement selector
ILOSELECTOR2(ReplaceWorstParent, IloSolution,
             IloSolutionPool, in,
             IloSolutionPool, pop,
             IloSolutionPool, parents) {
  IloSolution candidate = parents.getWorstSolution();
  IloSolution child = in.getSolution(0);
  pop.sort();
  if (pop.getSize() <= 1 || !candidate.isBetterThan(pop.getSolution(1))) {
    pop.remove(candidate);
    pop.add(child);
  }
  else
    candidate = child;
  select(candidate);
}

// Display the costs of solutions
void DisplayCosts(IloSolutionPool pool) {
  IloEnv env(pool.getEnv());
  IloNum sum = 0;
  for (IloSolutionPool::Iterator it(pool); it.ok(); ++it) {
    IloNum cost = (*it).getObjectiveValue();
    sum += cost;
    env.out() << cost << " ";
  }
  env.out() << "(Mean = " << sum / pool.getSize() << ")" << endl;
}

// Main GA solving function
void SolveWithGA(IloSolver solver,
                 IloModel model,
                 CarSeqProblem problem,
                 IloIntVarArray sequence,
                 IloIntVar cost) {

  // Indirect representation
  IloEnv env(solver.getEnv());
  IloIntVarArray prio(problem._env, problem._numberOfCars, 0, 1000);
  model.add(prio);

  // Create the solution prototype
  IloSolution prototype(env);
  prototype.add(prio);
  prototype.add(sequence);
  prototype.add(IloMinimize(env, cost));

  // Create the decoding goal
  IloGoal decode = Decode(env, problem, prio, sequence);

  // Set up an operator factory
  IloEAOperatorFactory factory(env, prio);
  factory.setAfterOperate(decode);
  factory.setSearchLimit(IloFailLimit(env, 100));
  factory.setPrototype(prototype);

  // Start timing
  IloTimer timer(env);
  timer.start();

  // Create initial population
  const IloInt popSize = 30;
  IloSolutionPool pop(env);
  IloPoolProc create = factory.randomize();
  solver.solve(IloExecuteProcessor(env, create(popSize) >> pop));

  // Sort the population wrt. quality and display.
  pop.sort();
  env.out() << "Initial population: ";
  DisplayCosts(pop);

  // Enforce probabilistic improvement
  factory.setBeforeOperate(ImproveOn(env, 0.5, 0.9));

  // Create genetic operators
  IloPoolProcArray ops(env);
  ops.add(factory.mutate(2.0 / problem._numberOfCars, "mutation"));
  ops.add(factory.uniformXover("uxover"));
  ops.add(factory.onePointXover("1ptxo"));

  // Make a processor that will perform a breeding cycle
  IloRandomSelector<IloSolution, IloSolutionPool> randomSelector(env);
  IloPoolProc rndSel = IloSelectSolutions(env, randomSelector, IloTrue);
  IloPoolProc breed = IloSelectProcessor(
    env,
    ops,
    IloRandomSelector<IloPoolProc,IloPoolProcArray>(env)
  );


  // Child replaces worst parent
  IloSolutionPool parents(env);
  IloPoolProc replace =
    IloSelectSolutions(env, ReplaceWorstParent(env, pop, parents));
  IloPoolProc destroy = IloDestroyAll(env);
  IloPoolProc replaceAndDestroy = replace >> destroy;

  // Create single-generation processor
  IloPoolProc cycle = pop >> rndSel >> parents >> breed(1) >> replaceAndDestroy;

  // The main genetic algorithm loop
  const IloInt maxGen = 10000;
  IloInt gen;
  IloSolution best = pop.getBestSolution();
  for (gen = 1; best.getObjectiveValue() != 0 && gen <= maxGen; gen++) {
    solver.solve(IloExecuteProcessor(env, cycle));
    best = pop.getBestSolution();
    pop.sort();
    if ((gen % 10) == 0) {
      env.out() << "Generation " << gen << ": ";
      DisplayCosts(pop);
    }
  }

  // Display best solution and statistics
  solver.solve(IloRestoreSolution(env, best));
  env.out() << "Time taken = " << timer.getTime() << endl;
  env.out() << "Terminated at generation " << gen << endl;
  env.out() << "Solution of cost " << solver.getValue(cost) << " is: " << endl;
  for (IloInt i = 0; i < sequence.getSize(); i++)
    env.out() << solver.getValue(sequence[i]) << " ";
  env.out() << endl;
}

// Solve car sequencing problems using a GA
int main(int argc, const char * argv[]) {
  IloEnv env;
  try {
    // Default input file
    const char * fname = "../../../examples/data/carseq1.dat";

    // Number of empty configurations to add by default
    IloInt slack = 20;

    // Read command line arguments
    if (argc > 1)
      fname = argv[1];
    if (argc > 2)
      slack = atoi(argv[2]);

    // Open input file and read problem data
    ifstream stream(fname);
    if (!stream.good()) {
      env.out() << "Cannot open " << fname << endl;
      return 0;
    }
    CarSeqProblem problem(env);
    problem.read(stream, slack);

    // Create the decision variables and the model.
    IloIntVarArray seq(env, problem._numberOfCars + problem._slack,
                       0, problem._numberOfConfigurations);
    IloModel model = BuildModel(problem, seq);
    IloIntVar costVar = BuildCostVar(model, seq, problem._slack);

    // Create the solver and solve.
    IloSolver solver(model);
    SolveWithGA(solver, model, problem, seq, costVar);
  } catch (IloException & ex) {
    env.out() << "Caught: " << ex << endl;
  }
  env.end();
  return 0;
}
