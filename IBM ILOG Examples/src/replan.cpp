// -------------------------------------------------------------- -*- C++ -*-
// File: ./examples/src/replan.cpp
// --------------------------------------------------------------------------
// Licensed Materials - Property of IBM
// 5724-Y47
// Copyright IBM Corporation 1990, 2009. All Rights Reserved.
//
// Note to U.S. Government Users Restricted Rights:
// Use, duplication or disclosure restricted by GSA ADP Schedule
// Contract with IBM Corp.
// --------------------------------------------------------------------------

#include <ilsolver/ilosolverint.h>

ILOSTLBEGIN

void Print(IloSolver solver, IloIntVarArray x){
  IloInt nqueen = x.getSize();
  for (IloInt i = 0; i < nqueen && i <= 100; i++)
    solver.out() << solver.getIntValue(x[i]) << " ";
  if (nqueen>100) solver.out() << "...";
  solver.out() << endl;
}

int main(){
  IloEnv env;
  try {
    IloModel model(env);
    IloInt nqueen = 8;

    IloIntVarArray x(env, nqueen, 0, nqueen-1),
      x1(env, nqueen, -2*nqueen, 2*nqueen),
      x2(env, nqueen, -2*nqueen, 2*nqueen);

    for (IloInt i = 0; i < nqueen; i++) {
      model.add(x1[i] == x[i]+i);
      model.add(x2[i] == x[i]-i);
    }
    model.add(IloAllDiff(env, x));
    model.add(IloAllDiff(env, x1));
    model.add(IloAllDiff(env, x2));

    IloGoal goal = IloGenerate(env, x);
    IloSolver solver(model);
    solver.solve(goal);

    solver.out() << "store 4 last vars" << endl;
    IloSolution frozen(env);
    frozen.add(x[4]);
    frozen.add(x[5]);
    frozen.add(x[6]);
    frozen.add(x[7]);
    frozen.store(solver);
    IloGoal restoreFrozen = IloRestoreSolution(env, frozen);

    IloSolution free(env);
    free.add(x[0]);
    free.add(x[1]);
    free.add(x[2]);
    free.add(x[3]);
    IloGoal storeFree = IloStoreSolution(env, free);

    solver.startNewSearch(restoreFrozen && goal && storeFree);
    while (solver.next()) {
      Print(solver, x);
    }
    solver.endSearch();

    solver.out() << "repeat last solution" << endl;
    IloGoal restoreFree = IloRestoreSolution(env, free);
    solver.solve(restoreFrozen && restoreFree);
    Print(solver, x);
    solver.out() << "change solution" << endl;
    model.add(x[OL] < 3);
    solver.startNewSearch(restoreFrozen && goal);
    while (solver.next()) {
      Print(solver, x);
    }
    solver.endSearch();
    solver.printInformation();

  }
  catch (IloException& ex) {
    cout << "Error: " << ex << endl;
  }
  env.end();
  return 0;
}
