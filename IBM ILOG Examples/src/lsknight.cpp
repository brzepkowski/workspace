// -------------------------------------------------------------- -*- C++ -*-
// File: ./examples/src/lsknight.cpp
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

Place the minimum of knights on an n x n chessboard
such that each square is either occupied by a knight
or attacked by a knight.

------------------------------------------------------------ */

#include <ilsolver/iimls.h>

ILOSTLBEGIN

//////////////////////////////////////////////////////////////////////////
// Neighborhoods to add, remove and relocate a knight.

IloNHood AddKnight(IloEnv env, IloIntVarArray knights, char *name = 0) {
  return IloSetToValue(env, knights, 1, name);
}

IloNHood RemoveKnight(IloEnv env, IloIntVarArray knights, char *name = 0) {
  return IloSetToValue(env, knights, 0, name);
}

IloNHood MoveKnight(IloEnv env, IloIntVarArray knights, char *name = 0) {
  return RemoveKnight(env, knights, name) * AddKnight(env, knights, name);
}

/////////////////////////////////////////////////////////////////////////
// Display routines to print out the chess board.

void DisplayBoard(IloSolver solver,
                  IloInt side,
                  IloArray<IloIntVarArray> knight) {
  IloInt i, j;
  for (i = 0; i < side; i++) {
    for (j = 0; j < side; j++) solver.out() << "+---";
    solver.out() << "+" << endl;
    for (j = 0; j < side; j++) {
      solver.out() << "| "
                   << (solver.getIntValue(knight[i][j]) ? "K" : " ") << " ";
    }
    solver.out() << "|" << endl;
  }
  for (j = 0; j < side; j++) solver.out() << "+---";
  solver.out() << "+" << endl;
}

/////////////////////////////////////////////////////////////////////////
// Main program.

int main(int argc, const char *argv[])
{
  IloEnv env;
  try {
    // Program constants
    IloInt side = 6;   // Default chess board size.
    IloInt factor = 1; // Default multiplication factor for num of moves.
    IloInt length = 2; // Default length of the tabu list.
    IloInt seed = 0;   // Default random number generator seed.
    // Read any options
    if (argc > 1) side = atoi(argv[1]);
    if (argc > 2) factor = atoi(argv[2]);
    if (argc > 3) seed = atoi(argv[3]);
    if (argc > 4) length = atoi(argv[4]);
    // Create model
    IloModel model(env);
    IloSolver solver(env);
    // Create the 0-1 variables for the knights
    IloIntVarArray knightArr(env, side * side, 0, 1);
    IloArray<IloIntVarArray> knight(env, side);
    IloInt i;

    // Arrange the knight variables in a 2-dimensional array (board)
    for (i = 0; i < side; i++) {
      knight[i] = IloIntVarArray(env, side);
      for (IloInt j = 0; j < side; j++) {
        IloIntVar k = knightArr[i * side + j];
        char name[20];
        sprintf(name, "(%ld, %ld)", i, j);
        k.setName(name);
        knight[i][j] = k;
      }
    }

    // Add the constraint that say each square must be occupied or
    // attacked by a knight
    for (IloInt x = 0; x < side; x++) {
      for (IloInt y = 0; y < side; y++) {
        IloExpr attacks(env);
        attacks += knight[x][y];
        for (IloInt xdiff = -2; xdiff <= 2; xdiff++) {
          if (xdiff == 0) continue;
          IloInt range = (xdiff < 0) ? 3 + xdiff : 3 - xdiff;
          for (IloInt ydiff = -range; ydiff <= range; ydiff += 2*range) {
            IloInt x1 = x + xdiff;
            if (x1 >= side || x1 < 0) continue;
            IloInt y1 = y + ydiff;
            if (y1 >= side || y1 < 0) continue;
            attacks += knight[x1][y1];
          }
        }
        model.add(attacks != 0);
      }
    }
    // Set up the cost variable
    IloInt minK = (IloInt)ceil(side * side / 9.0);
    IloIntVar numberOfKnights(env, minK, side*side);
    model.add(numberOfKnights == IloSum(knightArr));

    // Set up solution object
    IloSolution sol(env, "Current Board");
    sol.add(IloMinimize(env, numberOfKnights));
    sol.add(knightArr);

    // Create the inital solution
    solver.extract(model);
    solver.solve(IloGenerate(env, knightArr) && IloStoreSolution(env, sol));
    solver.out() << "Size " << side << ", " << solver.getValue(numberOfKnights)
                 << " knights, seed: " << seed << endl;
    DisplayBoard(solver, side, knight);

    // Local search

    // Create a best solution by duplication of the current one.
    IloSolution best = sol.makeClone(env);
    best.setName("Best Board");

    // Create the neighborhood.
    // Try to 1) Remove a random knight
    //        2) Move a random knight to a random empty square
    //        3) Add a knight at a random place.
    // Whatever works first will be performed.
    IloRandom rand(env, seed);
    IloNHood nhood = IloRandomize(env, RemoveKnight(env, knightArr), rand) +
                     IloRandomize(env, MoveKnight(env, knightArr), rand) +
                     IloRandomize(env, AddKnight(env, knightArr), rand);

    // Meta-heuristic
    IloMetaHeuristic tabu = IloTabuSearch(env, length);

    // Goal for making a move.
    // This is the most general form, notifying the neighborhood
    // and the meta-heuristic with the chosen neighbor.
    IloNeighborIdentifier nid(env);
    IloSolutionDeltaCheck sdCheck = tabu.getDeltaCheck();
    IloGoal move = IloStart(env, tabu, sol) &&
                   IloSelectSearch(
                      env,
                      IloScanNHood(env, nhood, nid, sol, sdCheck) &&
                      IloTest(env, tabu),
                      IloFirstSolution(env)
                   ) &&
                   IloNotify(env, tabu, nid) &&
                   IloNotify(env, nhood, nid) &&
                   IloStoreSolution(env, sol);

    // The search itself
    IloInt iter = factor * side * side;
    IloInt moves = 0;
    do {
      // Go while there are iterations and the move works
      while (--iter >= 0 && solver.solve(move)) {
        moves++;
        // Store the new best solution if better, and display
        if (sol.getObjectiveValue() < best.getObjectiveValue()) {
          solver.out() << "Move " << moves
                       << ", Knights = " << sol.getObjectiveValue() << endl;
          best.copy(sol);
        }
      }
      // If there are still iterations left, ask tabu if it is complete.
    } while (iter > 0 && !tabu.complete());

    // Restore the best solution found.
    solver.solve(IloRestoreSolution(env, best));
    DisplayBoard(solver, side, knight);
    solver.out() << "Size " << side << ", " << solver.getValue(numberOfKnights)
                 << " knights, seed: " << seed << endl;
    // Display information
    solver.out()  << "Number of moves taken         : " << moves << endl;

    solver.printInformation();
  }
  catch(IloException& ex) {
    cout << "Error: " << ex << endl;
  }
  env.end();
  return 0;
}

/*
Size 6, 9 knights, seed: 0
+---+---+---+---+---+---+
|   |   |   |   |   |   |
+---+---+---+---+---+---+
|   |   |   |   |   |   |
+---+---+---+---+---+---+
|   | K | K |   | K | K |
+---+---+---+---+---+---+
|   |   | K |   | K |   |
+---+---+---+---+---+---+
|   |   |   |   | K |   |
+---+---+---+---+---+---+
| K |   |   |   | K |   |
+---+---+---+---+---+---+
Move 2, Knights = 8
+---+---+---+---+---+---+
|   |   |   |   |   |   |
+---+---+---+---+---+---+
|   |   |   |   |   |   |
+---+---+---+---+---+---+
|   | K | K |   | K | K |
+---+---+---+---+---+---+
|   | K | K |   | K |   |
+---+---+---+---+---+---+
|   |   |   |   |   |   |
+---+---+---+---+---+---+
|   |   |   |   | K |   |
+---+---+---+---+---+---+
Size 6, 8 knights, seed: 0
Number of moves taken         : 36
Number of fails               : 1027
Number of choice points       : 938
Number of variables           : 37
Number of constraints         : 90
Reversible stack (bytes)      : 24144
Solver heap (bytes)           : 96504
Solver global heap (bytes)    : 16104
And stack (bytes)             : 4044
Or stack (bytes)              : 4044
Search Stack (bytes)          : 4044
Constraint queue (bytes)      : 11144
Total memory used (bytes)     : 160028
Running time since creation   : 0.66
*/
