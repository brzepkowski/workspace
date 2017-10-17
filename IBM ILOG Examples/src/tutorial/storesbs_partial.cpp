// -------------------------------------------------------------- -*- C++ -*-
// File: ./examples/src/tutorial/storesbs_partial.cpp
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

    //Model the data
    //Input the data

    IloInt nStores    = costMatrix.getSize();
    IloInt nSuppliers = capacity.getSize();
    env.out() << "Cost to build a warehouse:  " << endl;
    env.out() << buildingCost << endl;
    env.out() << "Relative costs for stores:  " << endl;
    env.out() << costMatrix        << endl;
    env.out() << "Warehouse capacities:  " << endl;
    env.out() << capacity     << endl;

    //Declare the supplier decision variables
    //Declare the cost decision variables
    //Declare the warehouse open decision variables
    //Add the constraints on relative cost
    //Add the constraints on open warehouses
    //Add the constraints on warehouse capacity
    //Declare the totalCost decision variable
    //Add the constraint on total cost
    //Create the combinedGoal
    //Create the node evaluator
    //Create the SBSNodeEvaluatorGoal
    //Create the search selector
    //Create the finalGoal

    IloSolver solver(model);
    //Search for a solution
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
    else solver.out() << "No solution" << endl;
    solver.printInformation();

  }
  catch (IloException& ex) {
    cout << "Error: " << ex << endl;
  }
  env.end();
  return 0;
}
