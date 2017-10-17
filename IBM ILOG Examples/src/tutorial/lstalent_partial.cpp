// -------------------------------------------------------------- -*- C++ -*-
// File: examples/src/tutorial/lstalent_partial.cpp
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

  // Expression representing the global cost
  IloIntExpr cost(env);

  // Loop over all actors, building cost
  for (IloInt a = 0; a < numActors; a++) {
    // Expression for the waiting time for this actor
    IloIntExpr actorWait(env);

    // Calculate the first and last slots where this actor plays

    // If an actor is not in a scene,
    // he waits if he is on set when the scene is filmed

    // Accumulate the cost of waiting time for this actor
    cost += actorCost[a] * actorWait;
  }
  model.add(idleCost == cost);
  return model;
}

// Define an LNS fragment which is a random segment

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

      // Create an initial solution to the problem

      // Create the solution object

      // Create the LNS neighborhood

      // Create the LNS completion goal

      // Create a greedy local search move goal

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
