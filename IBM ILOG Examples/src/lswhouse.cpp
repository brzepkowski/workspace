// -------------------------------------------------------------- -*- C++ -*-
// File: ./examples/src/lswhouse.cpp
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

Each client (nbClients) has to be served by one warehouse (nbWhouses)
There is a service cost for each client by each warehouse (Cost matrix)
Building a new warehouse costs buildingCost.
Global cost is the sum of services costs (transcost) and the
building global costs.
Find which warehouses have to be built while minimizing the cost.

------------------------------------------------------------ */


#include <ilsolver/iimls.h>

ILOSTLBEGIN


// Function to read the problem from a file

void readProblem(IloEnv env,
                 istream &is,
                 IloInt &nbClients,
                 IloInt &nbWhouses,
                 IloIntArray &capacities,
                 IloIntArray &demands,
                 IloIntArray &buildCosts,
                 IloArray<IloIntArray> &serveCosts) {
  is >> nbWhouses >> nbClients;
  capacities = IloIntArray(env, nbWhouses);
  demands = IloIntArray(env, nbClients);
  buildCosts = IloIntArray(env, nbWhouses);
  serveCosts = IloArray<IloIntArray>(env, nbClients);
  env.out() << nbClients << " clients, " << nbWhouses << " warehouses" << endl;
  IloInt i;
  for (i = 0; i < nbWhouses; i++) {
    is >> capacities[i];
    is >> buildCosts[i];
  }
  env.out() << "Capacities: " << capacities << endl;
  env.out() << "Build costs: " << buildCosts << endl;
  for (i = 0; i < nbClients; i++) {
    serveCosts[i] = IloIntArray(env, nbWhouses);
    is >> demands[i];
    for (IloInt j = 0; j < nbWhouses; j++) is >> serveCosts[i][j];
  }
  env.out() << "Demands: " << demands << endl;
}

// Function to display the solution

void DisplaySolution(IloSolver solver,
                     const char *phrase,
                     IloIntVar cost,
                     IloIntVarArray offer) {
  solver.out() << endl << phrase << endl
               << "[" << solver.getValue(cost) << "]" << endl
               << solver.getIntVarArray(offer) << endl;
}

// Function to perform simple tabu search with a short term memory

void TabuSearch(IloSolver solver,
                IloIntVarArray open,
                IloSolution sol,
                IloSolution whole,
                IloGoal subGoal,
                IloInt iter = IloIntMax) {
  IloEnv env = solver.getEnv();
  IloRandom rand(env);
  IloNHood nh = IloRandomize(env, IloFlip(env, open), rand);
  IloMetaHeuristic mh = IloTabuSearch(env, 2);
  IloIntVar cost = sol.getObjectiveVar();
  IloSearchSelector selectMove = IloMinimizeVar(env, cost, 1.0);

  // Goal makes a move, and updates the (whole) best solution if better.
  IloGoal move = IloSingleMove(env, sol, nh, mh, selectMove, subGoal) &&
                 IloStoreBestSolution(env, whole);
  // Standard optimization loop.
  do {
    IloInt i = 0;
    while (--iter >= 0 && solver.solve(move)) {
      solver.out() << "Iter = " << ++i << ", "
                   << sol.getObjectiveValue() << " : ";
      for (IloInt j = 0; j < open.getSize(); j++)
        solver.out() << solver.getValue(open[j]);
      solver.out() << " (Best = " << whole.getObjectiveValue() << ")" << endl;
    }
  } while (iter > 0 && !mh.complete());
}

// Goal to assign customers to specific warehouses

ILCGOAL4(IlcSetOffer, IlcIntVarArray, offer, IlcIntVarArray, open,
                      IloArray<IloIntArray>, costs, IloIntArray, buildCosts) {
  IlcInt i = IlcChooseMinSizeInt(offer);
  if (i >= 0) {
    // Choose closest warehouse first.
    IlcInt best = -1;
    IlcInt lowest = IlcIntMax;
    for (IlcIntExpIterator it(offer[i]); it.ok(); ++it) {
      IloIntArray row = costs[i];
      IlcInt cost = row[*it] + buildCosts[*it] * (open[*it].getMin() == 0);
      if (cost <= lowest) { lowest = cost; best = *it; }
    }
    return IlcAnd(IlcOr(IlcSetValue(offer[i], best),
                        IlcRemoveValue(offer[i], best)),
                  this);
  }
  return 0;
}

ILOCPGOALWRAPPER4(IloSetOffer, solver, IloIntVarArray, offer, IloIntVarArray, open,
                  IloArray<IloIntArray>, costs, IloIntArray, buildCosts) {
  return IlcSetOffer(solver,
                     solver.getIntVarArray(offer),
                     solver.getIntVarArray(open),
                     costs,
                     buildCosts);
}

// Main program

int main(int argc, const char *argv[]){
  IloEnv env;
  try {
    IloModel m(env);
    // Open input file.
    ifstream infile;
    const char *fname;
    if (argc > 1) fname = argv[1];
    else          fname = "../../../examples/data/cap.txt";
    infile.open(fname);
    if (!infile.good()) {
      env.out() << "Could not open " << fname << endl;
      env.end();
      return 0;
    }
    // Decide on local search, proof, or local search, then proof.
    const char *lp = "lp";
    if (argc > 2) lp = argv[2];
    IloBool local = IloFalse;
    IloBool proof = IloFalse;
    if (strchr(lp, 'l')) local = IloTrue;
    if (strchr(lp, 'p')) proof = IloTrue;

    // Read in the problem
    IloInt nbWhouses, nbClients;
    IloIntArray capacities(env);
    IloIntArray demands(env);
    IloIntArray buildCosts(env);
    IloArray<IloIntArray> serveCosts;
    readProblem(env, infile, nbClients, nbWhouses,
                capacities, demands, buildCosts, serveCosts);
    infile.close();
    // Build all the constraints.
    IloIntVarArray offer(env, nbClients, 0, nbWhouses - 1);
    IloIntVarArray transCost(env, nbClients, 0, IloIntMax);
    IloIntVarArray open(env, nbWhouses, 0, 1);
    IloIntVarArray load(env, nbWhouses);
    IlcInt i;
    for (i = 0; i < nbWhouses; i++)
      load[i] = IloIntVar(env, 0, capacities[i]);
    m.add(IloPack(env, load, offer, demands));

    // Only open if customers are served.
    for (i = 0; i < nbWhouses; i++)
      m.add(open[i] == (load[i] != 0));
    m.add(IloScalProd(open, capacities) >= IloSum(demands));

    // Total cost is sum of shipment and build costs
    IloIntVar cost(env, 0, IloIntMax);
    cost.setName("Cost\t");
    m.add(cost == IloSum(transCost) + IloScalProd(open, buildCosts));

    // Element constraints.
    for (i = 0; i < nbClients; i++) {
      IloIntArray costs(env, nbWhouses);
      for (IlcInt j = 0; j < nbWhouses; j++)
        costs[j] = serveCosts[i][j];
      IloIntVar dRes(env, costs);
      m.add(IloTableConstraint(env, dRes, costs, offer[i]));
      m.add(transCost[i] == dRes);
    }

    // Set up two solutions.
    // sol: Holds only the 0-1 variables which defining the open warehouses
    // whole: Holds the variables which assign a warehouse to each client
    // NOTE: whole defines a solution, whereas sol only defines which
    //       warehouses will be used.  The clients must be then be assigned
    //       a specific warehouse each for a complete solution.
    //       We perform local search over the openness of warehouses,
    //       and at each move assign the customers to warehouses.
    IloSolution sol(env);
    IloObjective obj = IloMinimize(env, cost);
    sol.add(obj);
    for (i = 0; i < nbWhouses; i++) {
      char name[10];
      sprintf(name, "O-%ld", i);
      open[i].setName(name);
      sol.add(open[i]);
    }
    IloSolution whole(env);
    whole.add(obj);
    whole.add(offer);
    IloGoal g;
    IloInt upperBound = IloIntMax;
    IloSolver solver(env);

    // Inital solution
    // Complete search goal to assign clients to warehouses
    IloGoal generateOffer =
            IloSetOffer(env, offer, open, serveCosts, buildCosts);
    g = generateOffer &&
        IloStoreSolution(env, sol) &&
        IloStoreSolution(env, whole);
    solver.extract(m);
    if (solver.solve(g)) {
      DisplaySolution(solver, "Initial Solution", cost, offer);
    }
    else {
      solver.out() << "No initial solution" << endl;
      env.end();
      return 0;
    }
    if (local) {
      // Local Search

      // The sub-goal to assign clients to open warehouses, at minimum cost
      IloGoal subGoal = IloSelectSearch(env, generateOffer,
                                        IloMinimizeVar(env, cost, 1));

      // Run the tabu search
      TabuSearch(solver, open, sol, whole, subGoal, 30);
    }
    // Get the upper bound
    upperBound = (IloInt)whole.getObjectiveValue();
    if (proof) {
      cost.setUb(upperBound - 1);
      g = IloGenerate(env, open) &&
          generateOffer &&
          IloStoreSolution(env, whole);
      g = IloSelectSearch(env, g, IloMinimizeVar(env, cost, 1));

      // Optimization loop
      if (!local) solver.extract(m);
      if (solver.solve(g)) {
        solver.out() << "Complete search found solution of cost "
                     << solver.getValue(cost) << endl;
        solver.out() << solver.getIntVarArray(offer) << endl;
        if (local) {
          solver.out() << "Better solution found by complete search" << endl;
          solver.out() << "Local search was "
                       << 100.0 * (upperBound / whole.getObjectiveValue() - 1)
                       << "% above optimal" << endl;
        }
      }
      else {
        if (local) solver.out() << "No better solution found" << endl;
        else       solver.out() << "No solution" << endl;
      }
    }
    // Reporting final solution
    cost.setUb(whole.getObjectiveValue());
    g = IloRestoreSolution(env, whole);
    solver.solve(g);
    DisplaySolution(solver, "Final Solution", cost, offer);
  } catch(IloException& ex) {
    cout << "Error: " << ex << endl;
  }
  env.end();
  return 0;
}

/*
50 clients, 16 warehouses
Capacities: [58268, 58268, 58268, 58268, 58268, 58268, 58268, 58268, 58268, 58268, 58268, 58268, 58268, 58268, 58268, 58268]
Build costs: [7500, 7500, 7500, 7500, 7500, 7500, 7500, 7500, 7500, 7500, 0, 7500, 7500, 7500, 7500, 7500]
Demands: [146, 87, 672, 1337, 31, 559, 2370, 1089, 33, 32, 5495, 904, 1466, 143, 615, 564, 226, 3016, 253, 195, 38, 807, 551, 304, 814, 337, 4368, 577, 482, 495, 231, 322, 685, 12912, 325, 366, 3671, 2213, 705, 328, 1681, 1117, 275, 500, 2241, 733, 222, 49, 1464, 222]

Initial Solution
[965952]
IlcIntVarArrayI[[10] [10] [10] [10] [10] [10] [1] [10] [10] [10] [3] [10] [10] [10] [10] [7] [3] [8] [3] [7] [3] [9] [10] [10] [10] [10] [12] [10] [10] [14] [10] [10] [10] [2] [10] [3] [2] [10] [7] [10] [10] [3] [7] [9] [12] [7] [7] [7] [10] [10]]
Iter = 1, 943477 : 0111010111101010 (Best = 943477)
Iter = 2, 939949 : 0111010111111010 (Best = 939949)
Iter = 3, 937269 : 1111010111111010 (Best = 937269)
Iter = 4, 934199 : 1111010111111000 (Best = 934199)
Iter = 5, 935152 : 1111010111111001 (Best = 934199)
Iter = 6, 935445 : 1111010110111001 (Best = 934199)
Iter = 7, 933569 : 1111011110111001 (Best = 933569)
Iter = 8, 932616 : 1111011110111000 (Best = 932616)
Iter = 9, 933876 : 1111011110101000 (Best = 932616)
Iter = 10, 935883 : 1111011110101100 (Best = 932616)
Iter = 11, 938953 : 1111011110101110 (Best = 932616)
Iter = 12, 937692 : 1111011110111110 (Best = 932616)
Iter = 13, 935686 : 1111011110111010 (Best = 932616)
Iter = 14, 932616 : 1111011110111000 (Best = 932616)
Iter = 15, 933569 : 1111011110111001 (Best = 932616)
Iter = 16, 934829 : 1111011110101001 (Best = 932616)
Iter = 17, 937899 : 1111011110101011 (Best = 932616)
Iter = 18, 936946 : 1111011110101010 (Best = 932616)
Iter = 19, 935686 : 1111011110111010 (Best = 932616)
Iter = 20, 932616 : 1111011110111000 (Best = 932616)
Iter = 21, 933569 : 1111011110111001 (Best = 932616)
Iter = 22, 934829 : 1111011110101001 (Best = 932616)
Iter = 23, 937899 : 1111011110101011 (Best = 932616)
Iter = 24, 936946 : 1111011110101010 (Best = 932616)
Iter = 25, 935686 : 1111011110111010 (Best = 932616)
Iter = 26, 932616 : 1111011110111000 (Best = 932616)
Iter = 27, 933569 : 1111011110111001 (Best = 932616)
Iter = 28, 934829 : 1111011110101001 (Best = 932616)
Iter = 29, 937899 : 1111011110101011 (Best = 932616)
Iter = 30, 936946 : 1111011110101010 (Best = 932616)
No better solution found

Final Solution
[932616]
IlcIntVarArrayI[[7] [11] [0] [5] [7] [0] [1] [2] [7] [7] [3] [10] [5] [0] [6] [7] [3] [8] [3] [6] [3] [6] [10] [0] [11] [10] [12] [10] [10] [0] [0] [10] [0] [2] [11] [11] [5] [5] [7] [5] [10] [3] [7] [6] [12] [7] [7] [6] [5] [11]]
*/
