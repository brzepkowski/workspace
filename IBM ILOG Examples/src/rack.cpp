// -------------------------------------------------------------- -*- C++ -*-
// File: ./examples/src/rack.cpp
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

A set of electronic cards must be plugged into racks with
electric connectors. Different types of racks can hold
different sets of cards, incurring different costs. Given a set
of cards and a maximal number of racks to use, we must find a
set of (at most) the given number of racks to fit in all the
cards, while we hold the cost to a minimum.

A rack has the following features:

- a maximal power it can provide;

- a number of connection slots; each slot can receive one card;

- its price; the price of a rack depends on its number of
connection slots and on the maximal power it can provide.

For cards, we consider only the power they use.

The constraints of this example can be summarized this way:

- plug all given cards into racks;

- use at most a given number of racks;

- each rack has a given number of connectors and can receive
  at most one card per connector;

- the sum of power used by the cards plugged into a rack must
  be less than or equal to the maximal power of this rack.

As the maximal number of racks to use is fixed, the objective
is to find two points about each rack used:

- the cards it receives;
- its correct type according to the number of plugged cards
and to the power it must provide.

We will search for a solution that minimizes the total cost
(that is, the sum of rack prices).

------------------------------------------------------------ */
#include <ilsolver/ilosolverint.h>

ILOSTLBEGIN

const IloInt nbRackTypes = 2;
const IloInt maxSlots = 16;
enum {power, price, nbSlot};

IloInt Racks[3][nbRackTypes+1] = {
    { 0, 150, 200 },
    { 0, 150, 200 },
    { 0, 8,    16 }
};


const IloInt nbCardTypes = 4;

const IloInt nbRack = 5;
IloInt nbCards[nbCardTypes] = {10, 4, 2, 1};

class Rack {
  public:
    IloIntVar      _type;
    IloIntVar      _price;
    IloIntVarArray _counters;
  Rack(IloModel model, IloIntArray cardPower);
};

Rack::Rack(IloModel model, IloIntArray cardPower) {
  IloEnv env = model.getEnv();
  _type = IloIntVar(env, 0, nbRackTypes);
  _counters = IloIntVarArray(env, nbCardTypes, 0, maxSlots);
  _price = IloIntVar(env, 0, 10000);
  IloIntArray prices(env, nbRackTypes + 1);
  IloIntArray rackPower (env, nbRackTypes + 1);
  IloIntArray rackSlot (env, nbRackTypes + 1);
  for (IloInt i=0; i<nbRackTypes + 1; i++) {
    prices[i] = Racks[price][i];
    rackPower[i] = Racks[power][i];
    rackSlot[i] = Racks[nbSlot][i];
  }
  model.add(_price ==  prices[_type]);
  model.add(IloScalProd(_counters, cardPower) <= rackPower[_type]);
  model.add(IloSum(_counters) <= rackSlot[_type]);
}

int main () {
  IloEnv env;
  try {
    IloModel model(env);

    IloIntArray cardPower(env, (int)nbCardTypes, 20, 40, 50, 75);
    Rack* racks[nbRack];
    IloInt i,j;
    for (i = 0; i < nbRack; i++)
      racks[i] = new Rack(model, cardPower);
    // all cards must be plugged
    for(j = 0; j < nbCardTypes; j++){
      IloIntVarArray vars(env,nbRack);
      for(i = 0; i < nbRack; i++)
        vars[i] = racks[i]->_counters[j];
      model.add(IloSum(vars) == nbCards[j]);
    }
    // remove symmetries
    for(i = 1; i < nbRack; i++){
      model.add(racks[i]->_type >= racks[i-1]->_type);
      model.add(IloIfThen(env, racks[i-1]->_type == racks[i]->_type,
                               racks[i-1]->_counters[0] <= racks[i]->_counters[0]));
    }
    // cost constraints
    IloIntVarArray prices(env, nbRack);
    for(i=0;i<nbRack;i++)
      prices[i] = racks[i]->_price;

    IloObjective objective = IloMinimize(env, IloSum(prices));
    model.add(objective);

    IloGoal goal = IloGoalTrue(env);
    for(i=0;i<nbRack;i++){
      goal = goal && IloInstantiate(env, racks[i]->_type);
      goal = goal && IloGenerate(env, racks[i]->_counters);
    }
    IloSolver solver(model);

    if (solver.solve(goal)) {
      solver.out() << endl
                   << "Found an " << solver.getStatus()<< " solution at cost "
                   << solver.getValue(objective) << endl << endl;
      for(i = 0; i < nbRack; i++) {
        if (solver.getValue(racks[i]->_type) !=0) {
          solver.out() << "Rack " << i << endl
                       << "Type : " << solver.getValue(racks[i]->_type)
                       << endl << "Price : "
                       << solver.getValue(racks[i]->_price) << endl
                       << "Counters : ";
          for (j = 0; j < nbCardTypes; j++)
            solver.out() << solver.getValue(racks[i]->_counters[j]) << " ";
          solver.out() << endl << endl;
        }
      }
    }
    else
      solver.out() << "No solution" << endl;
    solver.printInformation();
    for (i = 0; i < nbRack; ++i)
      delete racks[i];
  }
  catch (IloException& ex) {
    cout << "Error: " << ex << endl;
  }
  env.end();
  return 0;
}

/*
Found an OPTIMAL solution at cost 550

Rack 2
Type : 1
Price : 150
Counters : 0 1 2 0

Rack 3
Type : 2
Price : 200
Counters : 0 3 0 1

Rack 4
Type : 2
Price : 200
Counters : 10 0 0 0

Number of fails               : 279
Number of choice points       : 285
Number of variables           : 31
Number of constraints         : 30
Reversible stack (bytes)      : 4044
Solver heap (bytes)           : 20124
Solver global heap (bytes)    : 4044
And stack (bytes)             : 4044
Or stack (bytes)              : 4044
Search Stack (bytes)          : 4044
Constraint queue (bytes)      : 11144
Total memory used (bytes)     : 51488
Running time since creation   : 0.19
*/
