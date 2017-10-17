// -------------------------------------------------------------- -*- C++ -*-
// File: examples/src/eabinpack.cpp
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

// Read in a bin packing problem
IloBool ReadProblem(const char * fname, IloInt& cap, IloIntArray& weight) {
  ifstream is(fname);
  if (!is.good())
    return IloFalse;
  IloInt n;
  is >> n >> cap;
  for (IloInt i = 0; i < n; i++) {
    IloInt w;
    is >> w;
    weight.add(w);
  }
  is.close();
  return IloTrue;
}

// A simple goal to pack bins
ILCGOAL3(Pack, IlcIntVarArray, load,
               IlcIntVarArray, where,
               IlcIntArray, weight) {
  IlcGoal g;
  IlcInt i = IlcChooseFirstUnboundInt(where);
  if (i >= 0) {
    IlcInt b = where[i].getMin();
    IlcConstraint ct = where[i] == b;
    IlcGoal pack = load[b].getMin() == 0 ? ct: IlcOr(ct, !ct);
    g = IlcAnd(pack, this);
  }
  return g;
}

// The packing goal wrapper
ILOCPGOALWRAPPER3(Pack, solver,
                  IloIntVarArray, load,
                  IloIntVarArray, where,
                  IloIntArray, weight) {
  return Pack(solver,
              solver.getIntVarArray(load),
              solver.getIntVarArray(where),
              solver.getIntArray(weight));
}

// A genetic operator over bins
ILOIIMOP5(PackingOperator, numParents,
          IloInt, numParents,
          IloIntVarArray, load,
          IloIntVarArray, where,
          IloIntArray, weight,
          IloIntVar, used) {
  IloSolver solver = getSolver();
  IloInt numItems = weight.getSize();
  IloInt numBins = solver.getMax(used);
  IloSolutionPool parents = getInputPool();
  IloInt reqdMeanLoad = (IloSum(weight) + numBins - 1) / numBins;
  IlcRandom rnd = solver.getRandom();
  IloInt targetBin = rnd.getInt(numBins);
  IloInt currentBin = 0;
  IloInt tries = 0;

  while (currentBin < targetBin && tries < numParents * numBins) {
    // Pack a single bin
    IloSolution p = parents.getSolution(rnd.getInt(numParents));
    IloInt srcBin = rnd.getInt(p.getValue(used));
    if (p.getValue(load[srcBin]) >= reqdMeanLoad) {
      IloBool fit = IloTrue;
      for (IlcInt i = 0; fit && i < numItems; i++) {
        if (p.getValue(where[i]) == srcBin)
          fit = solver.getIntVar(where[i]).isInDomain(currentBin);
      }
      if (fit) {
        for (IlcInt i = 0; i < numItems; i++) {
          if (p.getValue(where[i]) == srcBin)
            solver.setValue(where[i], currentBin);
        }
        tries = 0;
        currentBin++;
      }
    }
    tries++;
  }
  return 0;
}

// Improve on worst in input pool
ILOIIMOP2(ImproveOn, 1, IloIntVar, used, IlcFloat, p) {
  IloInt limit = (IloInt)getInputPool().getWorstSolution().getValue(used);
  IloSolver solver(getSolver());
  solver.setMax(used, limit - (solver.getRandom().getFloat() < p));
  return 0;
}

// Main GA Solving body
void SolveWithGA(IloInt cap,
                 IloIntArray weight,
                 IloInt popSize,
                 IloInt maxGen,
                 IloNum mutationProb) {

  // The bin packing model
  IloEnv env = weight.getEnv();
  IloModel mdl(env);
  IloInt n = weight.getSize();
  IloInt totalWeight = IloSum(weight);
  IloInt lb = (totalWeight + cap - 1) / cap;
  IloInt numBins = lb * 11 / 9 + 5;
  IloIntVarArray where(env, n, 0, numBins);
  IloIntVarArray load(env, numBins, 0, cap);
  IloIntVar used(env, lb, numBins);
  mdl.add(used == 1 + IloMax(where));
  mdl.add(IloPack(env, load, where, weight));

  // Solution prototype
  IloSolution prototype(env);
  prototype.add(where);
  prototype.add(load);
  prototype.add(used);
  prototype.add(IloMinimize(env, used));

  // Create initial population
  IloSolutionPool population(env);
  IloGoal packGoal = Pack(env, load, where, weight);
  packGoal = IloRandomPerturbation(env, packGoal, 50.0 / (50 + n));
  IloPoolProc src = IloSource(env, packGoal, prototype);
  IloPoolProc create = src(popSize) >> population;
  IloSolver solver(mdl);
  solver.solve(IloExecuteProcessor(env, create));

  // Genetic operators
  IloPoolOperator improve = ImproveOn(env, used, 0.75);
  IloSearchLimit limit(IloFailLimit(env, 1000));
  IloPoolOperator mutop = PackingOperator(env, 1, load, where, weight, used);
  IloPoolProc mut = IloLimitOperator(env, improve && mutop && packGoal, limit);
  IloPoolProc breed = mut;
  if (popSize > 1) {
    IloPoolProcArray ops(env);
    ops.add(mut);
    IloPoolOperator xoop = PackingOperator(env, 2, load, where, weight, used);
    IloPoolProc xo = IloLimitOperator(env, improve && xoop && packGoal, limit);
    ops.add(xo);
    IloExplicitEvaluator<IloPoolProc> operatorProbs(env);
    operatorProbs.setEvaluation(mut, mutationProb);
    operatorProbs.setEvaluation(xo, 1.0 - mutationProb);
    IloRouletteWheelSelector<IloPoolProc, IloPoolProcArray> opSel(
      env, operatorProbs
    );
    breed = IloSelectProcessor(env, ops, opSel);
  }

  // Single generation goal
  IloSolutionPool offspring(env);
  IloPoolProc rndSel = IloSelectSolutions(env,
                         IloRandomSelector<IloSolution, IloSolutionPool>(env)
                       );
  IloPoolProc gen = population >> rndSel >> breed(popSize) >> offspring;
  IloGoal genGoal = IloExecuteProcessor(env, gen)
                 && IloExecuteProcessor(env, population >> IloDestroyAll(env))
                 && IloExecuteProcessor(env, offspring >> population);

  // Genetic algorithm loop
  IloInt best = population.getBestSolution().getValue(used);
  IloInt i;
  for (i = 0; i <= maxGen && best > lb; i++) {
    env.out() << "Generation " << i << ": ";
    env.out() << best << " - "
              << population.getWorstSolution().getValue(used) << endl;
    solver.solve(genGoal);
    best = population.getBestSolution().getValue(used);
  }

  // Display best
  solver.solve(IloRestoreSolution(env, population.getBestSolution()));
  IloInt binsUsed = (IloInt)solver.getValue(used);
  env.out() << "Best solution uses " << binsUsed << " bins" << endl;
  if (binsUsed == lb)
    env.out() << "Solution is optimal" << endl;
  else
    env.out() << "Solution is within "
              << binsUsed - lb << " of optimal" << endl;
  for (i = 0; i < binsUsed; i++) {
    env.out() << "Bin " << i + 1 << " of weight "
              << solver.getValue(load[i]) << ": ";
    for (IloInt j = 0; j < n; j++) {
      if (solver.getValue(where[j]) == i)
        env.out() << weight[j] << " ";
    }
    env.out() << endl;
  }
}

// Main body
int main(int argc, const char * argv[]) {
  IloEnv env;
  try {
    const char * fname = "../../../examples/data/binpack1.dat";
    IloInt maxGen = 100;
    IloInt popSize = 30;
    IloNum mutationProb = 0.75;
    if (argc > 1)
      fname = argv[1];
    if (argc > 2)
      maxGen = atoi(argv[2]);
    if (argc > 3)
      popSize = atoi(argv[3]);
    if (argc > 4)
      mutationProb = atof(argv[4]);
    IloInt cap;
    IloIntArray weight(env);
    if (ReadProblem(fname, cap, weight))
      SolveWithGA(cap, weight, popSize, maxGen, mutationProb);
    else
      env.out() << "Could not open " << fname << endl;
  } catch (IloException & ex) {
    env.out() << "Caught: " << ex << endl;
  }
  env.end();
  return 0;
}
