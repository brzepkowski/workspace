// -------------------------------------------------------------- -*- C++ -*-
// File: ./examples/src/storesbs_ex3.cpp
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
    IloGoal combinedGoal = IloGenerate(env, cost, IloChooseMaxRegretMin) &&
                           IloGenerate(env, supplier) &&
                           IloInstantiate(env, totalCost);
    IloNodeEvaluator SBSNodeEvaluator = IloSBSEvaluator(env, 4);
    IloGoal SBSNodeEvaluatorGoal = IloApply(env, combinedGoal, SBSNodeEvaluator);
    IloSearchSelector minimizeSearchSelector = IloMinimizeVar(env,
                                                              totalCost,
                                                              0.1);
    IloGoal finalGoal = IloSelectSearch(env, SBSNodeEvaluatorGoal,
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
	  solver.printInformation();
      }
    else {
      solver.out() << "No solution" << endl;
    }

  }
  catch (IloException& ex) {
    cout << "Error: " << ex << endl;
  }
  env.end();
  return 0;
}
/*
Using default file
Cost to build a warehouse:
30
Relative costs for stores:
[[20, 24, 11, 25, 30, 28, 27, 82, 83, 74,
2, 55, 73, 69, 61], [28, 27, 82, 83, 74, 93, 15, 63, 87, 46,
47, 57, 55, 71, 95], [74, 97, 71, 96, 70, 42, 8, 29, 67, 59,
93, 15, 63, 87, 46], [2, 55, 73, 69, 61, 1, 65, 73, 59, 56,
93, 35, 19, 85, 46], [46, 96, 59, 83, 4, 42, 22, 56, 67, 59,
11, 73, 15, 43, 96], [42, 22, 29, 67, 59, 1, 5, 57, 67, 56,
42, 8, 29, 67, 59], [1, 5, 73, 59, 56, 1, 5, 73, 59, 56,
42, 8, 29, 67, 59], [10, 73, 13, 43, 96, 42, 22, 56, 67, 59,
42, 56, 29, 56, 59], [93, 35, 63, 85, 46, 13, 22, 29, 66, 59,
65, 6, 73, 59, 56], [47, 65, 55, 71, 95, 42, 22, 29, 69, 59,
93, 25, 45, 85, 46],
[17, 19, 67, 2, 15, 93, 35, 19, 85, 46,
47, 57, 55, 71, 95], [26, 5, 78, 43, 19, 10, 15, 13, 9, 96,
68, 35, 62, 85, 46], [11, 73, 15, 43, 96, 42, 56, 29, 56, 59,
47, 57, 55, 71, 95], [93, 15, 63, 87, 46, 93, 55, 63, 92, 46,
47, 65, 55, 92, 95], [47, 65, 37, 71, 95, 47, 65, 45, 71, 95,
10, 73, 58, 43, 96], [42, 22, 29, 67, 49, 11, 45, 13, 43, 96,
42, 56, 29, 56, 59], [1, 5, 73, 59, 56, 47, 65, 45, 71, 95,
68, 35, 62, 85, 46], [10, 75, 13, 43, 62, 10, 15, 13, 9, 96,
93, 35, 19, 85, 46], [93, 25, 45, 85, 46, 65, 6, 73, 59, 56,
12, 5, 92, 59, 56], [47, 65, 55, 92, 95, 42, 22, 29, 69, 59,
47, 57, 55, 71, 95],
[42, 8, 29, 67, 59, 93, 35, 89, 85, 46,
47, 65, 56, 71, 95], [1, 5, 57, 67, 56, 42, 22, 56, 67, 5,
65, 6, 73, 59, 56], [10, 15, 13, 9, 96, 42, 22, 29, 69, 59,
47, 57, 55, 71, 95], [68, 35, 62, 85, 46, 93, 35, 19, 85, 46,
47, 57, 55, 71, 95], [47, 57, 55, 71, 95, 42, 22, 29, 69, 59,
93, 25, 45, 85, 46], [42, 22, 29, 69, 59, 1, 65, 73, 59, 56,
93, 35, 19, 85, 46], [1, 65, 73, 59, 56, 1, 65, 73, 59, 56,
93, 35, 19, 85, 46], [10, 73, 13, 83, 96, 11, 45, 13, 43, 96,
42, 56, 29, 56, 59], [93, 35, 19, 85, 46, 93, 35, 19, 85, 46,
47, 57, 55, 71, 95], [47, 65, 45, 71, 95, 28, 27, 82, 83, 74,
2, 55, 73, 69, 61]]
Warehouse capacities:
[5, 9, 12, 3, 4, 6, 8, 1, 6, 11,
3, 10, 8, 9, 3]

------------------------------
Solution:
Store  0 Warehouse:  London
Store  1 Warehouse:  Barcelona
Store  2 Warehouse:  Barcelona
Store  3 Warehouse:  Munich
Store  4 Warehouse:  Lisbon
Store  5 Warehouse:  Munich
Store  6 Warehouse:  Bordeaux
Store  7 Warehouse:  London
Store  8 Warehouse:  Munich
Store  9 Warehouse:  Barcelona
Store  10 Warehouse:  Bordeaux
Store  11 Warehouse:  Bordeaux
Store  12 Warehouse:  London
Store  13 Warehouse:  Bordeaux
Store  14 Warehouse:  Lisbon
Store  15 Warehouse:  Munich
Store  16 Warehouse:  Bordeaux
Store  17 Warehouse:  London
Store  18 Warehouse:  Barcelona
Store  19 Warehouse:  Barcelona
Store  20 Warehouse:  Bordeaux
Store  21 Warehouse:  Bordeaux
Store  22 Warehouse:  London
Store  23 Warehouse:  Bordeaux
Store  24 Warehouse:  Barcelona
Store  25 Warehouse:  Munich
Store  26 Warehouse:  Munich
Store  27 Warehouse:  London
Store  28 Warehouse:  London
Store  29 Warehouse:  Lisbon

Store  0 Cost:  11
Store  1 Cost:  15
Store  2 Cost:  8
Store  3 Cost:  1
Store  4 Cost:  11
Store  5 Cost:  1
Store  6 Cost:  5
Store  7 Cost:  13
Store  8 Cost:  13
Store  9 Cost:  22
Store  10 Cost:  19
Store  11 Cost:  5
Store  12 Cost:  15
Store  13 Cost:  15
Store  14 Cost:  10
Store  15 Cost:  11
Store  16 Cost:  5
Store  17 Cost:  13
Store  18 Cost:  6
Store  19 Cost:  22
Store  20 Cost:  8
Store  21 Cost:  5
Store  22 Cost:  13
Store  23 Cost:  35
Store  24 Cost:  22
Store  25 Cost:  1
Store  26 Cost:  1
Store  27 Cost:  13
Store  28 Cost:  19
Store  29 Cost:  2

Total cost:  490
------------------------------
Number of fails               : 68415
Number of choice points       : 65840
Number of variables           : 526
Number of constraints         : 526
Reversible stack (bytes)      : 104544
Solver heap (bytes)           : 361824
Solver global heap (bytes)    : 4584576
And stack (bytes)             : 4044
Or stack (bytes)              : 4044
Search Stack (bytes)          : 4044
Constraint queue (bytes)      : 13164
Total memory used (bytes)     : 5076240
Elapsed time since creation   : 86.835
*/

