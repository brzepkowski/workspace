// -------------------------------------------------------------- -*- C++ -*-
// File: ./examples/src/facility_solver.cpp
// --------------------------------------------------------------------------
// Licensed Materials - Property of IBM
// 5724-Y47
// Copyright IBM Corporation 1990, 2009. All Rights Reserved.
//
// Note to U.S. Government Users Restricted Rights:
// Use, duplication or disclosure restricted by GSA ADP Schedule
// Contract with IBM Corp.
// --------------------------------------------------------------------------

#include <ilsolver/ilosolverint.h>

ILOSTLBEGIN

class FileError : public IloException {
public:
  FileError() : IloException("Cannot open data file") {}
};

typedef IloArray<IloIntVarArray> IntVarMatrix;

int main(int argc, char **argv){
  IloEnv env;
  try{
    IloInt i, j;

    IloIntArray capacity(env), fixedCost(env);
    IloIntArray2 cost(env);
    IloInt      nbLocations;
    IloInt      nbClients;

    const char* filename  = "../../../examples/data/facility.dat";
    if (argc > 1)
      filename = argv[1];
    ifstream file(filename);
    if (!file) {
      cout << "usage:   " << argv[0] << " <file>" << endl;
      throw FileError();
    }

    file >> capacity >> fixedCost >> cost;
    nbLocations = capacity.getSize();
    nbClients   = cost.getSize();

    IloBool consistentData = (fixedCost.getSize() == nbLocations);
    for (i = 0; consistentData && (i < nbClients); i++)
      consistentData = (cost[i].getSize() == nbLocations);
    if (!consistentData) {
      cout << "ERROR: data file '"
           << filename << "' contains inconsistent data" << endl;
    }

    IloModel model(env);
    IloIntVarArray load(env, nbLocations);
    for (j = 0; j < nbLocations; j++)
      load[j] = IloIntVar(env, 0, capacity[j]);
    IloIntVarArray supply(env, nbClients, 0, nbLocations - 1);
    model.add(IloPack(env, load, supply));
    IloExpr obj(env);
    for (j = 0; j < nbLocations; j++)
      obj += (load[j] > 0) * fixedCost[j];
    for (i = 0; i < nbClients; i++)
      obj += cost[i][supply[i]];
    model.add(IloMinimize(env, obj));
    IloSolver solver(env);
    solver.extract(model);
    solver.solve();

    solver.out() << "Optimal value: " << solver.getValue(obj) << endl;
    for (j = 0; j < nbLocations; j++){
      if (solver.getValue(load[j]) != 0) {
        solver.out() << "Facility " << j << " is open, it serves clients ";
        for (i = 0; i < nbClients; i++){
          if (solver.getValue(supply[i]) == j)
            solver.out() << i << " ";
        }
        solver.out() << endl;
      }
    }
    solver.printInformation();
  }
  catch(IloException& e){
    cout  << " ERROR: " << e.getMessage() << endl;
  }
  env.end();
  return 0;
}
