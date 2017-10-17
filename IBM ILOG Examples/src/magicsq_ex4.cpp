// -------------------------------------------------------------- -*- C++ -*-
// File: ./examples/src/magicsq_ex4.cpp
// --------------------------------------------------------------------------
// Licensed Materials - Property of IBM
// 5724-Y47
// Copyright IBM Corporation 1990, 2009. All Rights Reserved.
//
// Note to U.S. Government Users Restricted Rights:
// Use, duplication or disclosure restricted by GSA ADP Schedule
// Contract with IBM Corp.
// --------------------------------------------------------------------------

/* ------------------------------------------------------------

Problem Description
-------------------

A magic square is a square matrix where the sum of all rows,
columns, and main diagonals are equal, but the numbers in the magic
square are consecutive and start with 1. A Gnomon magic square is
a n=4 magic square in which the elements in each quarter
(2 x 2 corner) have the same sum.

------------------------------------------------------------ */

#include <ilsolver/ilosolverint.h>
ILOSTLBEGIN
int main() {
  IloEnv env;
  try {
    IloModel model(env);
    IloInt n = 4;
    IloInt sum = n*(n*n+1)/2;
    IloInt i, j;
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
    // Constraint on upper left quarter
    IloExpr exp3(env);
    for (i = 0; i < 2; i++)
      for(j = 0; j < 2; j++)
        exp3 += square[i+n*j];
        model.add(exp3 == sum);
        exp3.end();
    // Constraint on upper right quarter
    IloExpr exp4(env);
    for (i = 2; i < n; i++)
      for(j = 0; j < 2; j++)
        exp4 += square[i+n*j];
        model.add(exp4 == sum);
        exp4.end();
    // Constraint on lower left quarter
    IloExpr exp5(env);
    for (i = 0; i < 2; i++)
      for(j = 2; j < n; j++)
        exp5 += square[i+n*j];
        model.add(exp5 == sum);
        exp5.end();
    // Constraint on lower right quarter
    IloExpr exp6(env);
    for (i = 2; i < n; i++)
      for(j = 2; j < n; j++)
        exp6 += square[i+n*j];
        model.add(exp6 == sum);
        exp6.end();
    IloSolver solver(model);
    if (solver.solve(IloGenerate(env, square, IloChooseMinSizeInt)))
      {
      for (i = 0; i < n; i++){
        for (j = 0; j < n; j++)
          solver.out() << " " << solver.getValue(square[n*i+j]);
        solver.out() << endl;
      }
      solver.out() << endl;
    }
    else
      solver.out() << "No solution " << endl;
  }
  catch (IloException& ex) {
    cout << "Error: " << ex << endl;
  }
  env.end();
  return 0;
}
/*
Feasible Solution
 1 4 13 16
 14 15 2 3
 8 5 12 9
 11 10 7 6
*/
