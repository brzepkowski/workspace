// -------------------------------------------------------------- -*- C++ -*-
// File: ./examples/src/tutorial/magicsq_partial.cpp
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
square are consecutive and start with 1.

Here is a magic square of size 3:

           __________
          |8 | 3 | 4 |
          |1 | 5 | 9 |
          |6 | 7 | 2 |
           ----------

------------------------------------------------------------ */

#include <ilsolver/ilosolverint.h>
ILOSTLBEGIN
int main(int argc, char* argv[]){
  IloEnv env;
  try {
    IloModel model(env);
    IloInt n = (argc > 1) ? atoi(argv[1]) : 5;
    IloInt sum = n*(n*n+1)/2;
    IloInt i, j;
    //Declare the decision variables
    //Add the constraint IloAllDiff
    //Add the constraints on rows
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
    //Create an instance of IloSolver
    //Create the goal
    //Search for a solution
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


