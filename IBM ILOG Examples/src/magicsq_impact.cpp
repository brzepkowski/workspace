// -------------------------------------------------------------- -*- C++ -*-
// File: ./examples/src/magicsq_impact.cpp
// --------------------------------------------------------------------------
// Licensed Materials - Property of IBM
// 5724-Y47
// Copyright IBM Corporation 1990, 2009. All Rights Reserved.
//
// Note to U.S. Government Users Restricted Rights:
// Use, duplication or disclosure restricted by GSA ADP Schedule
// Contract with IBM Corp.
// --------------------------------------------------------------------------

#include <ilsolver/ilosolver.h>

ILOSTLBEGIN

IloGoal GetMyGoal(IloEnv env, IloIntVarArray x){
  IloCustomizableGoal d = IloCustomizableGoal(env, x);
  IloEvaluator<IlcIntVar> e1 = -IlcImpactVarEvaluator(env);
  IloEvaluator<IlcIntVar> e2 = -IlcLocalImpactVarEvaluator(env, -1);
  IloEvaluator<IlcIntVar> e3 = IlcRandomVarEvaluator(env);
  d.addMinNumberFilter(e1, 5);
  d.addFilter(e2);
  d.addFilter(e3);
  IloEvaluator<IlcInt> e4 = IlcImpactValueEvaluator(env);
  IloEvaluator<IlcInt> e5 = IlcRandomValueEvaluator(env);
  d.addFilter(e4);
  d.addFilter(e5);
  return IloInitializeImpactGoal(env, -1) && d;
}

int main(int argc, char* argv[]){
  IloEnv env;
  try {
    IloModel model(env);

    IloInt n = (argc > 1) ? atoi(argv[1]) : 8;
    IloInt sum = n*(n*n+1)/2;
    IloInt i, j;

    IloInt  goalType = (argc > 2) ? atoi(argv[2]) : 1;

    cout << "Solving magic square " << n << endl;

    IloIntVarArray square(env, n*n, 1, n*n);

    model.add(IloAllDiff(env, square));

    // Constraints on rows
    for (i = 0; i < n; i++){
      IloExpr exp(env);
      for(j = 0; j < n; j++)
        exp += square[n*i+j];
      model.add(exp == sum);
      exp.end();
    }

    // Constraints on columns
    for (i = 0; i < n; i++){
      IloExpr exp(env);
      for(j = 0; j < n; j++)
        exp += square[n*j+i];
      model.add(exp == sum);
      exp.end();
    }

    // Constraint on 1st diagonal
    IloExpr exp1(env);
    for (i = 0; i < n; i++)
      exp1 += square[n*i+i];
    model.add(exp1 == sum);
    exp1.end();

    // Constraint on 2nd diagonal
    IloExpr exp2(env);
    for (i = 0; i < n; i++)
      exp2 += square[n*i+n-i-1];
    model.add(exp2 == sum);
    exp2.end();

    IloSolver solver(env);
	solver.use(IlcImpactInformation(solver, square));

	solver.extract(model);

    IloGoal g;
    switch(goalType){
      case 0: g = IloInitializeImpactGoal(env, -1) && IloCustomizableGoal(env, square);
        break;
      case 1: g =    IloInitializeImpactGoal(env, -1)
                  && IloRestartGoal(env, IloCustomizableGoal(env, square), 1000);
        break;
      case 2: g = GetMyGoal(env, square);

    }

    if (solver.solve(IloInitializeImpactGoal(env, -1) && g)) {
      for (i = 0; i < n; i++){
        for (j = 0; j < n; j++)
          solver.out() << " " << solver.getValue(square[n*i+j]);
        solver.out() << endl;
      }
      solver.out() << endl;
	  cout << "Impacts" << endl;
      for (i = 0; i < n; i++){
        for (j = 0; j < n; j++)
          solver.out() << " " << solver.getImpact(solver.getIntVar(square[n*i+j]));
        solver.out() << endl;
      }
      solver.out() << endl;
    }
    else
      solver.out() << "No solution " << endl;
      solver.printInformation();
  }
  catch (IloException& ex) {
    cout << "Error: " << ex << endl;
  }
  env.end();
  return 0;
}
