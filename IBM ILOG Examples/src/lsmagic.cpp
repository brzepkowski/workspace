// -------------------------------------------------------------- -*- C++ -*-
// File: ./examples/src/lsmagic.cpp
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
columns, and diagonals are equal, but the numbers in the magic
square are consecutive and start with 1.

Here is a magic square of size 3:

           __________
          |8 | 3 | 4 |
          |1 | 5 | 9 |
          |6 | 7 | 2 |
           ----------

------------------------------------------------------------ */

#include <ilsolver/iimls.h>

ILOSTLBEGIN
// Generates a random instantiation of the variables in a
// magic square. Used for creating a first solution

ILCGOAL2(RandGenerateIlc, IlcIntVarArray, a, IloRandom, rand) {
  IloInt count = 0;
  IlcInt i;
  for (i = 0; i < a.getSize(); i++)
    if (!a[i].isBound()) count += 1;
  if (count == 0) return 0;
  count = rand.getInt(count);
  for (i = 0; i < a.getSize(); i++) {
    if (!a[i].isBound())
      if (count-- == 0) break;
  }
  return IlcAnd(IlcInstantiate(a[i]), this);
}

ILOCPGOALWRAPPER2(RandGenerate, solver, IloIntVarArray, b, IloRandom, rand) {
  return RandGenerateIlc(solver, solver.getIntVarArray(b), rand);
}


// Displays the magic square

void DisplayGrid(IloSolver solver,
                 const IloIntVarArray Square,
                 IloInt n,
                 IloIntVar cost) {
  solver.out() << "Cost = " << solver.getValue(cost) << endl;
  for (IloInt i = 0; i < n; i++) {
    for (IloInt j = 0; j < n; j++) {
      IloInt value = solver.getIntValue(Square[n * i + j]);
      if (value < 10) solver.out() << " ";
      if (value < 100) solver.out() << " ";
      solver.out() << value << " ";
    }
    solver.out() << endl;
  }
  solver.out() << endl;
}

// Returns a constrained integer variable representing
// the absolute difference from the specified target.
IloExpr TargetDifference(const IloIntVarArray a, IloInt target) {
  return IloAbs(IloSum(a) - target);
}

int main(int argc, const char* argv[]){
  IloEnv env;
  try {
    IloModel model(env);
    // Read size from input - default is 8.
    IloInt n = (argc > 1) ? atoi(argv[1]) : 8;

    // Read the factor determining the number of iterations - default is 10
    IloInt factor = (argc > 2) ? atoi(argv[2]) : 10;

    // The sum of each, row column and diagonal
    IloInt sum = n * (n * n + 1) / 2;
    IloInt i, j;
    IloIntVarArray Square(env, n * n, 1, n * n);
    IloConstraint allDiff = IloAllDiff(env, Square);
    model.add(allDiff);

    // The violation level of each row, column or diagonal
    IloIntVarArray violation(env, 2 * n + 2, 0, n * n * n);
    // Construct the rows and columns
    for (i = 0; i < n; i++) {
      IloIntVarArray row(env, n);
      IloIntVarArray col(env, n);
      for (j = 0; j < n; j++) {
        row[j] = Square[n * i + j];
        col[j] = Square[n * j + i];
      }
      model.add(violation[i]     == TargetDifference(row, sum));
      model.add(violation[i + n] == TargetDifference(col, sum));
    }
    // Construct the two diagonals
    IloIntVarArray diag1(env, n);
    IloIntVarArray diag2(env, n);
    for (j = 0; j < n; j++) {
      diag1[j] = Square[j * (n + 1)];
      diag2[j] = Square[(j + 1) * (n - 1)];
    }
    model.add(violation[2 * n]     == TargetDifference(diag1, sum));
    model.add(violation[2 * n + 1] == TargetDifference(diag2, sum));
    // Construct the cost function.
    IloIntVar cost(env, 0, n * n * n * n);
    model.add(cost == IloSum(violation));
    // Local search - first create the solution object
    IloSolution sol(env);
    sol.add(Square);
    sol.add(IloMinimize(env, cost));
    // Generate and store the first solution
    IloRandom randGen(env);
    IloGoal g = RandGenerate(env, Square, randGen) &&
                IloStoreSolution(env, sol);
    IloSolver solver(env);
    solver.extract(model);

    if (solver.solve(g)) {
      // Display the first solution, and backtrack to before
      // the first solution was created.
      DisplayGrid(solver, Square, n, cost);
      model.remove(allDiff);

      // We will use a randomized `swap' neighborhood
      IloNHood nhood = IloRandomize(env, IloSwap(env, Square), randGen);

      // Use greedy descent, first accept search.
      IloGoal greedyMove = IloSingleMove(env,
                                         sol,
                                         nhood,
                                         IloImprove(env, 1));
      // Run the greedy search
      while (solver.solve(greedyMove))
      ;

      solver.out() << "After first accept, cost = "
                   << sol.getObjectiveValue() << endl;
      // Take a copy of the solution in `bestSol'
      IloSolution bestSol = sol.makeClone(env);

      // We will use tabu search, and take the best neighbor
      // The tabu lists forbid previous assignments for 3 + n*n/20 moves
      // and keep new assignments for at least 2 moves.

      IloMetaHeuristic tabu = IloTabuSearch(env, 3 + n * n / 20, 2);
      IloGoal tabuMove =
        IloSingleMove(env, sol, nhood, tabu, IloMinimizeVar(env, cost, 1))
        && IloUpdateBestSolution(env, bestSol, sol);

      // Compute the number of iterations
      IloInt iter = factor * n;
      // Meta-heuristic optimization loop: display square at each stage.
      do {
        while (iter > 0 &&
               bestSol.getObjectiveValue() > 0 &&
               solver.solve(tabuMove)) {
          DisplayGrid(solver, Square, n, cost);
          iter--;
        }
      } while (--iter > 0 &&
               bestSol.getObjectiveValue() > 0 &&
               !tabu.complete());
      solver.solve(IloRestoreSolution(env, bestSol));
      // Display best solution at end.
      solver.out() << "After " << factor * n - iter
                   << " iterations of tabu search:" << endl;

      DisplayGrid(solver, Square, n, cost);
    }
    else {
      // No first solution.
      solver.out() << "No solution." << endl;
    }
    solver.printInformation();
  } catch (IloException& ex) {
    cout << "Error: " << ex << endl;
  }
  env.end();
  return 0;
}

/*
Cost = 640
 14  19  58  10  55  59   4  48
 43  64  61  42  52   3   1  12
  2  44  34  15  50  30  60  41
 47  17  45  54  57   5  32  46
 63  28  33  53  25  37   9  29
 40  23  31   7  62  21  16  18
 22  38  35  36  56   8  39  20
 51  11  24  27  13  26  49   6

After first accept, cost = 3
Cost = 4
 22  44  48  28  26  56   4  32
 17  64  43  35  52  34   1  14
  9  41  33  19   8  30  62  58
 61  16  45  15  31   5  38  49
 21  37  10  54  25  51   2  59
 40  18  20   7  60  42  50  23
 27  29  36  55  53   3  46  12
 63  11  24  47   6  39  57  13

Cost = 4
 22  44  48  28  26  56   4  32
 17  64  43  35  52  34   1  14
  9  41  33  19   8  30  62  58
 61  16  45  15  31   5  38  49
 21  37  11  54  25  51   2  59
 40  18  20   7  60  42  50  23
 27  29  36  55  53   3  46  12
 63  10  24  47   6  39  57  13

Cost = 4
 22  44  48  28  26  56   4  32
 17  64  43  35  53  34   1  14
  9  41  33  19   8  30  62  58
 61  16  45  15  31   5  38  49
 21  37  11  54  25  51   2  59
 40  18  20   7  60  42  50  23
 27  29  36  55  52   3  46  12
 63  10  24  47   6  39  57  13

Cost = 3
 22  44  48  28  26  56   4  32
 17  64  43  35  53  34   1  13
  9  41  33  19   8  30  62  58
 61  16  45  15  31   5  38  49
 21  37  11  54  25  51   2  59
 40  18  20   7  60  42  50  23
 27  29  36  55  52   3  46  12
 63  10  24  47   6  39  57  14

Cost = 4
 22  44  48  28  26  56   4  32
 17  64  43  35  53  34   1  13
  9  42  33  19   8  30  62  58
 61  16  45  15  31   5  38  49
 21  37  11  54  25  51   2  59
 40  18  20   7  60  41  50  23
 27  29  36  55  52   3  46  12
 63  10  24  47   6  39  57  14

Cost = 2
 22  44  48  28  26  56   4  32
 17  64  43  35  53  34   1  13
  9  42  33  19   7  30  62  58
 61  16  45  15  31   5  38  49
 21  37  11  54  25  51   2  59
 40  18  20   8  60  41  50  23
 27  29  36  55  52   3  46  12
 63  10  24  47   6  39  57  14

Cost = 0
 22  44  48  28  26  56   4  32
 17  64  43  34  53  35   1  13
  9  42  33  19   7  30  62  58
 61  16  45  15  31   5  38  49
 21  37  11  54  25  51   2  59
 40  18  20   8  60  41  50  23
 27  29  36  55  52   3  46  12
 63  10  24  47   6  39  57  14

After 8 iterations of tabu search:
Cost = 0
 22  44  48  28  26  56   4  32
 17  64  43  34  53  35   1  13
  9  42  33  19   7  30  62  58
 61  16  45  15  31   5  38  49
 21  37  11  54  25  51   2  59
 40  18  20   8  60  41  50  23
 27  29  36  55  52   3  46  12
 63  10  24  47   6  39  57  14

Number of fails               : 9874
Number of choice points       : 9775
Number of variables           : 83
Number of constraints         : 19
Reversible stack (bytes)      : 72384
Solver heap (bytes)           : 257304
Solver global heap (bytes)    : 28164
And stack (bytes)             : 4044
Or stack (bytes)              : 4044
Search Stack (bytes)          : 4044
Constraint queue (bytes)      : 11144
Total memory used (bytes)     : 381128
Running time since creation   : 4.23
*/
