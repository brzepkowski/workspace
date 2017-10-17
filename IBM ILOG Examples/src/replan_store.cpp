// -------------------------------------------------------------- -*- C++ -*-
// File: ./examples/src/replan_store.cpp
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

In this lesson, you will solve a logistics problem. A company has 10 stores.
Each store must be supplied by one warehouse. The company has five possible
locations where it has property and can build a supplier warehouse: Bonn,
Bordeaux, London, Paris, and Rome. The warehouse locations have different
capacities. A warehouse built in Bonn or Paris could supply only one store.
A warehouse built in London could supply two stores; a warehouse built in
Rome could supply three stores; and a warehouse built in Bordeaux could
supply four stores. The supply costs vary for each store, depending on which
warehouse is the supplier. For example, a store that is located in Paris
would have low supply costs if it were supplied by a warehouse also in Paris.
That same store would have much higher supply costs if it were supplied by the
other warehouses.

In a real world problem, the cost of building a warehouse would vary depending
on warehouse location. To keep this problem simple in order to focus on search
strategies, the cost of building any warehouse is set to 30.

The problem is to find the most cost-effective solution to this problem, while
making sure that each store is supplied by a warehouse.

------------------------------------------------------------ */
#include <ilsolver/ilosolverint.h>
ILOSTLBEGIN

class FileError : public IloException {
public:
  FileError() : IloException("Cannot open data file") {}
};

const char* Suppliers[] = {"Bonn", "Bordeaux", "London", "Paris", "Rome"};
int main(int argc, char** argv){
  IloEnv env;
  try {
    IloModel model(env);
    IloInt i, j;
    const char* fileName;
    if ( argc != 2 ) {
      env.warning() << "usage: " << argv[0] << " <filename>" << endl;
      env.warning() << "Using default file" << endl;
      fileName = "../../../examples/data/store.dat";
    } else
      fileName = argv[1];

    ifstream file(fileName);
    if ( !file )
      throw FileError();
    // model data
    IloInt       buildingCost;
    IloIntArray2 costMatrix(env);
    IloIntArray  capacity(env);

    file >> buildingCost >> costMatrix >> capacity;
    IloInt nStores    = costMatrix.getSize();
    IloInt nSuppliers = capacity.getSize();
    env.out() << "Cost to build a warehouse:  " << endl;
    env.out() << buildingCost << endl;
    env.out() << "Relative costs for stores:  " << endl;
    env.out() << costMatrix        << endl;
    env.out() << "Warehouse capacities:  " << endl;
    env.out() << capacity     << endl;

    // build model
    IloIntVarArray supplier(env, nStores, 0, nSuppliers-1);
    IloIntVarArray cost(env, nStores, 0, 99999);
    IloIntVarArray open(env, nSuppliers, 0,1);
    for (i = 0; i < nStores; i++){
      model.add(cost[i] == costMatrix[i][supplier[i]]);
      model.add(open[supplier[i]]==1 );
    }
    for (j = 0; j < nSuppliers; j++ ) {
      IloIntVarArray temp(env, nStores, 0, 1);
      for (IloInt k = 0; k < nStores; k++)
        model.add(temp[k] == (supplier[k] == j));
      model.add(IloSum(temp) <= capacity[j]);
    }
    IloIntVar totalCost(env, 0, 999999);
    model.add(totalCost == IloSum(cost) + IloSum(open) * buildingCost);
    IloGoal searchGoal = IloGenerate(env, cost, IloChooseMaxRegretMin) &&
      IloGenerate(env, supplier) &&
      IloInstantiate(env, totalCost);
    // solve model
    IloSolver solver(model);
    IloSolution solution(env);
    solution.add(supplier[0]);
    solution.add(supplier[1]);
    solution.add(supplier[2]);
    solution.add(supplier[3]); // we fix the suppliers of the first 4 stores
    if (solver.solve(searchGoal && IloStoreSolution(env, solution)))
      {
        solver.out() << "------------------------------" << endl;
        solver.out() << "Initial Solution:  " << endl;
        for (i = 0; i < nStores; i ++)
          solver.out() << "Store  " << i << " " << "Warehouse:  "
                       << Suppliers[(IloInt)solver.getValue(supplier[i])]
                       << " " << endl;
        solver.out() << endl;
        for (j = 0; j < nStores ; j ++)
          solver.out() << "Store  " << j << " " << "Cost:  "
                       << solver.getValue(cost[j]) << " " << endl;
        solver.out() << endl;
        solver.out() << "Total cost:  " << solver.getValue(totalCost)
                     << endl;
        solver.out() << solution << endl;
        solver.out() << "------------------------------" << endl;

        model.add(IloMinimize(env, totalCost));
        solver.solve(IloRestoreSolution(env, solution) && searchGoal);

        solver.out() << "------------------------------" << endl;
        solver.out() << "Final Solution:  " << endl;
        for (i = 0; i < nStores; i ++)
          solver.out() << "Store  " << i << " " << "Warehouse:  "
                       << Suppliers[(IloInt)solver.getValue(supplier[i])]
                       << " " << endl;
        solver.out() << endl;
        for (j = 0; j < nStores ; j ++)
          solver.out() << "Store  " << j << " " << "Cost:  "
                       << solver.getValue(cost[j]) << " " << endl;
        solver.out() << endl;
        solver.out() << "Total cost:  " << solver.getValue(totalCost)
                     << endl;
        solver.out() << "------------------------------" << endl;

      }
    else {
      solver.out() << "No solution" << endl;
    }
    solver.printInformation();
  }
  catch (IloException& ex) {
    cout << "Error: " << ex << endl;
  }
  env.end();
  return 0;
}
/*
usage: ./replan_store <filename>
Using default file
Cost to build a warehouse:
30
Relative costs for stores:
[[20, 24, 11, 25, 30], [28, 27, 82, 83, 74], [74, 97, 71, 96, 70], [2, 55, 73, 69, 61], [46, 96, 59, 83, 4], [42, 22, 29, 67, 59], [1, 5, 73, 59, 56], [10, 73, 13, 43, 96], [93, 35, 63, 85, 46], [47, 65, 55, 71, 95]]Warehouse capacities:
[1, 4, 2, 1, 3]
ILOG Solver 6.000, licensed to "ILOG Gentilly".
------------------------------
Initial Solution:
Store  0 Warehouse:  London
Store  1 Warehouse:  Bordeaux
Store  2 Warehouse:  Rome
Store  3 Warehouse:  Bonn
Store  4 Warehouse:  Rome
Store  5 Warehouse:  Bordeaux
Store  6 Warehouse:  Bordeaux
Store  7 Warehouse:  London
Store  8 Warehouse:  Bordeaux
Store  9 Warehouse:  Paris

Store  0 Cost:  11
Store  1 Cost:  27
Store  2 Cost:  70
Store  3 Cost:  2
Store  4 Cost:  4
Store  5 Cost:  22
Store  6 Cost:  5
Store  7 Cost:  13
Store  8 Cost:  35
Store  9 Cost:  71

Total cost:  410
IloSolution[ IloSavedIntVar[2] IloSavedIntVar[1] IloSavedIntVar[4] IloSavedIntVar[0] ]
------------------------------
------------------------------
Final Solution:
Store  0 Warehouse:  London
Store  1 Warehouse:  Bordeaux
Store  2 Warehouse:  Rome
Store  3 Warehouse:  Bonn
Store  4 Warehouse:  Rome
Store  5 Warehouse:  Bordeaux
Store  6 Warehouse:  Bordeaux
Store  7 Warehouse:  London
Store  8 Warehouse:  Rome
Store  9 Warehouse:  Bordeaux

Store  0 Cost:  11
Store  1 Cost:  27
Store  2 Cost:  70
Store  3 Cost:  2
Store  4 Cost:  4
Store  5 Cost:  22
Store  6 Cost:  5
Store  7 Cost:  13
Store  8 Cost:  46
Store  9 Cost:  65

Total cost:  385
------------------------------
Number of fails               : 4
Number of choice points       : 6
Number of variables           : 81
Number of constraints         : 126
Reversible stack (bytes)      : 16104
Solver heap (bytes)           : 68620
Solver global heap (bytes)    : 4044
And stack (bytes)             : 4044
Or stack (bytes)              : 4044
Search Stack (bytes)          : 4044
Constraint queue (bytes)      : 11160
Total memory used (bytes)     : 112060
Running time since creation   : 0.01
*/


