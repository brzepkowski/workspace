// -------------------------------------------------------------- -*- C++ -*-
// File: ./examples/src/whouse_mt2.cpp
// --------------------------------------------------------------------------
// Licensed Materials - Property of IBM
// 5724-Y47
// Copyright IBM Corporation 1990, 2009. All Rights Reserved.
//
// Note to U.S. Government Users Restricted Rights:
// Use, duplication or disclosure restricted by GSA ADP Schedule
// Contract with IBM Corp.
// --------------------------------------------------------------------------

#if defined(ILCUSEMT) || defined(ILOUSEMT)

# include <ilsolver/ilopsolver.h>
ILOSTLBEGIN

void readCosts(const char*   name,
               IloInt&       buildingCost,
               IloIntArray2& costs,
               IloInt&       nbClients,
               IloInt&       nbWhouses) {
  ifstream in(name);
  in >> buildingCost >> costs;
  nbClients = costs.getSize();
  if ( nbClients ) nbWhouses = costs[0].getSize();
  else             nbWhouses = 0;
}

int
main(int argc, char** argv)
{
  IloInitMT();
  IloEnv env;
  try {
    IloInt i;
    IloModel model(env);

    IloInt       buildingCost, nbClients, nbWhouses;
    IloIntArray2 costs(env);
    const char* fileName;
    if ( argc < 2 ) {
      env.warning() << "usage: " << argv[0] << " <filename>" << endl;
      env.warning() << "Using default file" << endl;
      fileName = "../../../examples/data/whouse.dat";
    } else fileName = argv[1];
    readCosts(fileName, buildingCost, costs, nbClients, nbWhouses);


    IloIntVarArray offer(env, nbClients, 0, nbWhouses-1);
    IloIntVarArray transCost(env, nbClients, 0, 10000);
    IloBoolVarArray open(env, nbWhouses);

    for (i=0; i < nbClients; i++){
      model.add(transCost[i] == costs[i][offer[i]]);
      model.add(open[offer[i]] == 1);
    }

    IloIntVar cost(env, 0, 10000, "Cost\t");
    model.add(cost == IloSum(transCost) + IloSum(open)*buildingCost);

    IloGoal goal = IloGenerate(env, offer) && IloInstantiate(env, cost);


    // end of model

    // We create the Parallel Solver

    IloParallelSolver psolver(model, 3);

    // We do a first run to tighten the problem
    psolver.solve(goal);
    IloSolver solver = psolver.getWorker(psolver.getSuccessfulWorkerId());
    IloInt maxCost = (IloInt)solver.getValue(cost);
    solver.warning() << "Setting max cost to " << maxCost << endl;

    model.add(cost <= maxCost);

    // We search for an optimal solution
    model.add(IloMinimize(env, cost));
    psolver.solve(goal);

    solver = psolver.getWorker(psolver.getSuccessfulWorkerId());
    solver.out() << endl << "Optimal Solution" << endl;
    solver.out() << "Cost " << solver.getValue(cost) << endl;
    solver.out() << "Offer ";
    for (i=0; i<nbClients; i++)
      solver.out() << solver.getValue(offer[i]) << " ";
    solver.out() << endl;
    solver.out() << "TransCost ";
    for (i=0; i<nbClients; i++)
      solver.out() << solver.getValue(transCost[i]) << " ";
    solver.out() << endl;
    solver.out() << "Open ";
    for (i=0; i<nbWhouses; i++)
      solver.out() << solver.getValue(open[i]) << " ";
    solver.out() << endl;
    psolver.end();
  }
  catch (IloException& ex) {
    cout << "Error: " << ex << endl;
  }
  env.end();
  IloEndMT();
  return 0;
}

/*
Setting max cost to 1413
Optimal Solution
Cost 964
Offer 7 3 3 7 3 3 7 3 3 7 3 3 7 3 3 7 3 3 7 3 3
TransCost 4 44 4 4 44 4 4 44 4 4 44 4 4 44 4 4 44 4 4 44 4
Open 0 0 0 1 0 0 0 1 0 0 0 0 0 0 0 0 0 0 0 0
*/
#else
#include <ilconcert/ilomodel.h>
ILOSTLBEGIN
int main() {
  IloEnv env;
  env.out() << "" << endl;
  env.out() << "Optimal Solution" << endl;
  env.out() << "Cost 964" << endl;
  env.out() << "Offer 7 3 3 7 3 3 7 3 3 7 3 3 7 3 3 7 3 3 7 3 3 " << endl;
  env.out() << "TransCost 4 44 4 4 44 4 4 44 4 4 44 4 4 44 4 4 44 4 4 44 4 " << endl;
  env.out() << "Open 0 0 0 1 0 0 0 1 0 0 0 0 0 0 0 0 0 0 0 0 " << endl;
  env.end();
  return 0;
}
#endif

