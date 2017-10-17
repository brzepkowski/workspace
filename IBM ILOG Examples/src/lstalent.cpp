// -------------------------------------------------------------- -*- C++ -*-
// File: examples/src/lstalent.cpp
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
// Read talent scheduling problem data from a file
IloBool ReadData(const char * filename,
                 IloIntArray actorPay,
                 IloIntArray sceneDuration,
                 IloArray<IloIntSet> actorInScene) {
  IloEnv env = actorPay.getEnv();
  ifstream in(filename);
  if (!in.good())
    return IloFalse;

  IloInt numActors, numScenes, a, s;

  in >> numActors;
  for (a = 0; a < numActors; a++) {
    IloInt pay;
    in >> pay;
    actorPay.add(pay);
  }

  in >> numScenes;
  for (s = 0; s < numScenes; s++) {
    IloInt duration;
    in >> duration;
    sceneDuration.add(duration);
  }

  for (a = 0; a < numActors; a++) {
    actorInScene.add(IloIntSet(env));
    for (s = 0; s < numScenes; s++) {
      IloBool inScene;
      in >> inScene;
      if (inScene)
        actorInScene[a].add(s);
    }
  }
  if (!in.good())
    return IloFalse;

  in.close();
  return IloTrue;
}

// Build the talent scheduling model
IloModel BuildModel(IloIntVarArray scene,
                    IloIntVar idleCost,
                    IloIntArray actorCost,
                    IloIntArray sceneDuration,
                    IloArray<IloIntSet> actorInScene) {
  IloEnv env = scene.getEnv();
  IloInt numScenes = scene.getSize();
  IloInt numActors = actorCost.getSize();
  IloModel model(env);

  // Make the slot-based secondary model
  IloIntVarArray slot(env, numScenes, 0, numScenes - 1);
  model.add(IloInverse(env, scene, slot));

  // Expression representing the global cost
  IloIntExpr cost(env);

  // Loop over all actors, building cost
  for (IloInt a = 0; a < numActors; a++) {
    // Expression for the waiting time for this actor
    IloIntExpr actorWait(env);

    // Calculate the first and last slots where this actor plays
    IloIntVarArray position(env);
    for (IloIntSet::Iterator it(actorInScene[a]); it.ok(); ++it)
      position.add(slot[*it]);
    IloIntExpr firstSlot = IloMin(position);
    IloIntExpr lastSlot = IloMax(position);

    // If an actor is not in a scene,
    // he waits if he is on set when the scene is filmed
    for (IloInt s = 0; s < numScenes; s++) {
      if (!actorInScene[a].contains(s)) { // not in scene
        IloIntExpr wait = (firstSlot <= slot[s] && slot[s] <= lastSlot);
        actorWait += sceneDuration[s] * wait;
      }
    }

    // Accumulate the cost of waiting time for this actor
    cost += actorCost[a] * actorWait;
  }
  model.add(idleCost == cost);
  return model;
}

// Define an LNS fragment which is a random segment
ILODEFINELNSFRAGMENT1(SegmentLNSNHood, solver, IloIntVarArray, x) {
  IlcRandom r = solver.getRandom();
  IloInt a = r.getInt(x.getSize());
  IloInt b = r.getInt(x.getSize());
  if (a > b) { IloInt tmp = a; a = b; b = tmp; }
  for (IlcInt i = a; i <= b; i++)
    addToFragment(solver, x[i]);
}

int main(int argc, const char * argv[]) {
  IloEnv env;
  try {
    const char * inputFile = "../../../examples/data/rehearsal.txt";
    IloInt seed = 1;
    if (argc > 1)
      inputFile = argv[1];
    if (argc > 2)
      seed = atoi(argv[2]);

    IloIntArray actorPay(env);
    IloIntArray sceneDuration(env);
    IloArray<IloIntSet> actorInScene(env);
    IloBool ok = ReadData(inputFile, actorPay, sceneDuration, actorInScene);
    if (!ok) {
      env.out() << "Error reading " << inputFile << endl;
    }
    else {
      // Create the decision variables, cost, and the model
      IloInt numScenes = sceneDuration.getSize();
      IloInt numActors = actorPay.getSize();
      IloIntVarArray scene(env, numScenes, 0, numScenes - 1);
      IloIntVar idleCost(env, 0, IloIntMax);
      IloModel model = BuildModel(
        scene, idleCost, actorPay, sceneDuration, actorInScene
      );

      // Create the solver, and seed its random number generator
      IloSolver solver(model);
      solver.getRandom().reSeed(seed);

      // Create an initial solution to the problem
      IloGoal goal = IloGenerate(env, scene);
      solver.solve(goal);

      // Create the solution object
      IloSolution solution(env);
      solution.add(scene);
      IloObjective objective = IloMinimize(env, idleCost);
      solution.add(objective);
      solution.store(solver);

      // Create the LNS neighborhood
      IloNHood nhood = SegmentLNSNHood(env, scene);

      // Create the LNS completion goal
      IloGoal complete = IloLimitSearch(env, goal, IloFailLimit(env, 100));

      // Create a greedy local search move goal
      IloGoal move = IloSingleMove(
        env, solution, nhood, IloImprove(env), complete
      );

      // Enter the local search loop
      IloInt choicePoints = 0;
      IloInt noMove = 0;
      IloInt movesTried = 0;
      while (noMove < 100) {
        movesTried++;
        if (solver.solve(move)) {
          noMove = 0;
          cout << movesTried << ": Cost = "
               << solver.getValue(idleCost) << endl;
        }
        else
          noMove++;
        choicePoints += solver.getNumberOfChoicePoints();
      }
      cout << "Total choice points = " << choicePoints << endl;

      // Display the order of scene filming
      solver.solve(IloRestoreSolution(env, solution));
      cout << "Solution of idle cost " << solver.getValue(idleCost) << endl;
      cout << "Order:";
      for (IloInt s = 0; s < numScenes; s++)
        cout << " " << 1 + solver.getValue(scene[s]);
      cout << endl;

      // Give more detailed information on the schedule
      for (IloInt a = 0; a < numActors; a++) {
        cout << "|";
        for (IloInt s = 0; s < numScenes; s++) {
          IloInt sc = solver.getIntValue(scene[s]);
          for (IloInt d = 0; d < sceneDuration[sc]; d++) {
            if (actorInScene[a].contains(sc))
              cout << "X";
            else
              cout << ".";
          }
          cout << "|";
        }
        cout << "  (Rate = " << actorPay[a] << ")" << endl;
      }
      solver.end();
    }
  } catch (IloException & ex) {
    cout << "Caught: " << ex << endl;
  }
  env.end();
  return 0;
}
