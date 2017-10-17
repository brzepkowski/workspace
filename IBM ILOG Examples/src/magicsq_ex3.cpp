// -------------------------------------------------------------- -*- C++ -*-
// File: ./examples/src/magicsq_ex3.cpp
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

Experiment with changing the order in which variables are bound
in the solution search. Note how the number of fails, number of
choice points, and elapsed time since creation change as you try the
following parameters for the goal IloGenerate: the
default (IloChooseFirstUnboundInt), IloChooseMinSizeInt,
IloChooseMaxSizeInt, IloChooseMinMinInt, IloChooseMaxMinInt,
IloChooseMinMaxInt, and IloChooseMaxMaxInt.

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
    IloSolver solver(model);
    IloGoal goal = IloGenerate(env, square);
    //IloGoal goal = IloGenerate(env, square, IloChooseMinSizeInt);
    //IloGoal goal = IloGenerate(env, square, IloChooseMaxSizeInt);
    //IloGoal goal = IloGenerate(env, square, IloChooseMinMinInt);
    //IloGoal goal = IloGenerate(env, square, IloChooseMaxMinInt);
    //IloGoal goal = IloGenerate(env, square, IloChooseMinMaxInt);
    //IloGoal goal = IloGenerate(env, square, IloChooseMaxMaxInt);
    if (solver.solve(goal))
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
      solver.printInformation();
  }
  catch (IloException& ex) {
    cout << "Error: " << ex << endl;
  }
  env.end();
  return 0;
}
/*
//Feasible Solution using IloGoal goal = IloGenerate(env, square);
 1 2 13 24 25
 3 22 19 6 15
 23 16 10 11 5
 21 7 9 20 8
 17 18 14 4 12

Number of fails               : 5486
Number of choice points       : 5497
Number of variables           : 25
Number of constraints         : 13
Reversible stack (bytes)      : 8064
Solver heap (bytes)           : 24144
Solver global heap (bytes)    : 4044
And stack (bytes)             : 4044
Or stack (bytes)              : 4044
Search Stack (bytes)          : 4044
Constraint queue (bytes)      : 11152
Total memory used (bytes)     : 59536
Elapsed time since creation   : 0.371
//Feasible Solution using IloGoal goal = IloGenerate(env, square, IloChooseMinSizeInt);
 1 2 13 24 25
 3 23 17 6 16
 20 21 11 8 5
 22 4 14 18 7
 19 15 10 9 12

Number of fails               : 791
Number of choice points       : 799
Number of variables           : 25
Number of constraints         : 13
Reversible stack (bytes)      : 8064
Solver heap (bytes)           : 20124
Solver global heap (bytes)    : 4044
And stack (bytes)             : 4044
Or stack (bytes)              : 4044
Search Stack (bytes)          : 4044
Constraint queue (bytes)      : 11152
Total memory used (bytes)     : 55516
Elapsed time since creation   : 0.09
//Feasible Solution using IloGoal goal = IloGenerate(env, square, IloChooseMaxSizeInt);
 1 2 24 21 17
 3 16 12 11 23
 25 18 13 5 4
 22 10 7 20 6
 14 19 9 8 15

Number of fails               : 15650718
Number of choice points       : 15650729
Number of variables           : 25
Number of constraints         : 13
Reversible stack (bytes)      : 12084
Solver heap (bytes)           : 28164
Solver global heap (bytes)    : 4044
And stack (bytes)             : 4044
Or stack (bytes)              : 4044
Search Stack (bytes)          : 4044
Constraint queue (bytes)      : 11152
Total memory used (bytes)     : 67576
Elapsed time since creation   : 997.564

//Feasible Solution using IloGoal goal = IloGenerate(env, square, IloChooseMinMinInt);
 1 2 24 21 17
 3 16 12 11 23
 25 18 13 5 4
 22 10 7 20 6
 14 19 9 8 15

Number of fails               : 1325537
Number of choice points       : 1325548
Number of variables           : 25
Number of constraints         : 13
Reversible stack (bytes)      : 12084
Solver heap (bytes)           : 24144
Solver global heap (bytes)    : 4044
And stack (bytes)             : 4044
Or stack (bytes)              : 4044
Search Stack (bytes)          : 4044
Constraint queue (bytes)      : 11152
Total memory used (bytes)     : 63556
Elapsed time since creation   : 97.169
//Feasible Solution using IloGoal goal = IloGenerate(env, square, IloChooseMaxMinInt);
 1 2 13 24 25
 3 23 17 6 16
 20 21 11 8 5
 22 4 14 18 7
 19 15 10 9 12

Number of fails               : 849
Number of choice points       : 859
Number of variables           : 25
Number of constraints         : 13
Reversible stack (bytes)      : 8064
Solver heap (bytes)           : 20124
Solver global heap (bytes)    : 4044
And stack (bytes)             : 4044
Or stack (bytes)              : 4044
Search Stack (bytes)          : 4044
Constraint queue (bytes)      : 11152
Total memory used (bytes)     : 55516
Elapsed time since creation   : 0.1
//Feasible Solution using IloGoal goal = IloGenerate(env, square, IloChooseMinMaxInt);
 1 2 13 24 25
 3 23 19 4 16
 22 15 10 12 6
 21 8 9 20 7
 18 17 14 5 11

Number of fails               : 612
Number of choice points       : 623
Number of variables           : 25
Number of constraints         : 13
Reversible stack (bytes)      : 8064
Solver heap (bytes)           : 16104
Solver global heap (bytes)    : 4044
And stack (bytes)             : 4044
Or stack (bytes)              : 4044
Search Stack (bytes)          : 4044
Constraint queue (bytes)      : 11152
Total memory used (bytes)     : 51496
Elapsed time since creation   : 0.09
//Feasible Solution using IloGoal goal = IloGenerate(env, square, IloChooseMaxMaxInt);
 1 2 13 24 25
 3 22 19 6 15
 23 16 10 11 5
 21 7 9 20 8
 17 18 14 4 12

Number of fails               : 9020
Number of choice points       : 9029
Number of variables           : 25
Number of constraints         : 13
Reversible stack (bytes)      : 12084
Solver heap (bytes)           : 24144
Solver global heap (bytes)    : 4044
And stack (bytes)             : 4044
Or stack (bytes)              : 4044
Search Stack (bytes)          : 4044
Constraint queue (bytes)      : 11152
Total memory used (bytes)     : 63556
Elapsed time since creation   : 0.601
*/
