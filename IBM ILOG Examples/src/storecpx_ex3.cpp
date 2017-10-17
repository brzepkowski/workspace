// -------------------------------------------------------------- -*- C++ -*-
// File: ./examples/src/storecpx_ex3.cpp
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

const char* Suppliers[] = {"Bonn", "Bordeaux", "London", "Paris", "Rome",
                           "Munich", "Barcelona", "Prague", "Dublin", "Madrid",
                           "Lisbon", "Berlin", "Amsterdam", "Brussels", "Milan"};

int main(int argc, char** argv){
  IloEnv env;
  try {
    IloModel model(env);
    IloInt i, j;

    const char* fileName;
    if ( argc != 2 ) {
      env.warning() << "usage: " << argv[0] << " <filename>" << endl;
      env.warning() << "Using default file" << endl;
      fileName = "../../../examples/data/store_ex3.dat";
    }
    else
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

    IloGoal goal1 = IloGenerate(env, cost, IloChooseMaxRegretMin);
    IloGoal applygoal1 = IloApply(env, goal1, IloDDSEvaluator(env, 3, 1));
    IloGoal limitgoal1 = IloLimitSearch(env, applygoal1, IloFailLimit(env, 5));
    IloGoal goal2 = IloGenerate(env, supplier);
    IloGoal applygoal2 = IloApply(env, goal2, IloSBSEvaluator(env, 1, 1));
    IloGoal combinedGoal = limitgoal1 && applygoal2 && IloInstantiate(env, totalCost);
    IloSearchSelector minimizeSearchSelector = IloMinimizeVar(env,
                                                              totalCost,
                                                              1);

    IloGoal finalGoal = IloSelectSearch(env,
                                        combinedGoal,
                                        minimizeSearchSelector);

    IloSolver solver(model);
    if (solver.solve(finalGoal))
      {
      solver.out() << "------------------------------" << endl;
      solver.out() << "Solution:  " << endl;
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
    else
      solver.out() << "No solution" << endl;
    solver.printInformation();

  }
  catch (IloException& ex) {
    cout << "Error: " << ex << endl;
  }
  env.end();
  return 0;
}
/*
------------------------------
Solution:
Store  0 Warehouse:  Lisbon
Store  1 Warehouse:  Barcelona
Store  2 Warehouse:  Barcelona
Store  3 Warehouse:  Munich
Store  4 Warehouse:  Rome
Store  5 Warehouse:  Munich
Store  6 Warehouse:  Bonn
Store  7 Warehouse:  London
Store  8 Warehouse:  Berlin
Store  9 Warehouse:  Barcelona
Store  10 Warehouse:  Paris
Store  11 Warehouse:  Bordeaux
Store  12 Warehouse:  Bonn
Store  13 Warehouse:  Bordeaux
Store  14 Warehouse:  Lisbon
Store  15 Warehouse:  Munich
Store  16 Warehouse:  Bonn
Store  17 Warehouse:  Munich
Store  18 Warehouse:  Berlin
Store  19 Warehouse:  Barcelona
Store  20 Warehouse:  Bordeaux
Store  21 Warehouse:  Bonn
Store  22 Warehouse:  Paris
Store  23 Warehouse:  Prague
Store  24 Warehouse:  Barcelona
Store  25 Warehouse:  Munich
Store  26 Warehouse:  Munich
Store  27 Warehouse:  Bonn
Store  28 Warehouse:  London
Store  29 Warehouse:  Lisbon

Store  0 Cost:  2
Store  1 Cost:  15
Store  2 Cost:  8
Store  3 Cost:  1
Store  4 Cost:  4
Store  5 Cost:  1
Store  6 Cost:  1
Store  7 Cost:  13
Store  8 Cost:  6
Store  9 Cost:  22
Store  10 Cost:  2
Store  11 Cost:  5
Store  12 Cost:  11
Store  13 Cost:  15
Store  14 Cost:  10
Store  15 Cost:  11
Store  16 Cost:  1
Store  17 Cost:  10
Store  18 Cost:  5
Store  19 Cost:  22
Store  20 Cost:  8
Store  21 Cost:  1
Store  22 Cost:  9
Store  23 Cost:  19
Store  24 Cost:  22
Store  25 Cost:  1
Store  26 Cost:  1
Store  27 Cost:  10
Store  28 Cost:  19
Store  29 Cost:  2

Total cost:  557
------------------------------
------------------------------
Number of fails               : 5
Number of choice points       : 39
Number of variables           : 526
Number of constraints         : 526
Reversible stack (bytes)      : 76404
Solver heap (bytes)           : 361824
Solver global heap (bytes)    : 24144
And stack (bytes)             : 4044
Or stack (bytes)              : 4044
Search Stack (bytes)          : 4044
Constraint queue (bytes)      : 11160
Total memory used (bytes)     : 485664
Elapsed time since creation   : 0.181
*/

