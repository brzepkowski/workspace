// -------------------------------------------------------------- -*- C++ -*-
// File: ./examples/src/carseq_limit.cpp
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

Assume that you have 8 cars to paint in three available colors: green,
yellow, and blue. Due to technical limitations on the assembly
line, no more than three cars can be painted green, exactly three cars must
be painted yellow, and no more than two cars can be painted blue. The first
car off the assembly line cannot be painted green.
--------------------------------------------------------------------------*/
#include <ilsolver/ilosolver.h>

ILOSTLBEGIN

class IlcMyLimitI : public IlcSearchLimitI {
private:
  IlcInt& _counter;
  IlcInt _limit;
public:
  IlcMyLimitI(IloSolver, IlcBool, IlcInt& counter, IlcInt limit);
  void init(const IlcSearchNode);
  IlcBool check(const IlcSearchNode) const;
  IlcSearchLimitI* duplicateLimit(IloSolver);
};

IlcMyLimitI::IlcMyLimitI (IloSolver s, IlcBool dup, IlcInt& counter,
                              IlcInt limit) :
  IlcSearchLimitI(s, dup), _counter(counter), _limit(limit) {}

void IlcMyLimitI::init(const IlcSearchNode) {}

IlcBool IlcMyLimitI::check(const IlcSearchNode node) const {
  if (_counter >= _limit) {
    node.getSolver().out()
      << "Limit crossed: the counter has attained the limit "
      << _limit << endl;
    return IlcTrue;
  }
  return IlcFalse;
}

IlcSearchLimitI* IlcMyLimitI::duplicateLimit(IloSolver s) {
  return new IlcMyLimitI(s, IlcTrue, _counter, _limit);
}

IlcSearchLimit IlcMyLimit(IloSolver s, IlcInt& c, IlcInt l) {
  IlcSearchLimitI* limit = new (s.getHeap()) IlcMyLimitI(s, IlcFalse , c, l);
  return IlcSearchLimit(limit);
}

class IloMyLimitI : public IloSearchLimitI {
  IloInt& _counter;
  IloInt _limit;
public:
  IloMyLimitI(IloEnvI*, IloInt&, IloInt);
  virtual IlcSearchLimit extract(const IloSolver) const;
  virtual IloSearchLimitI* makeClone(IloEnvI* env) const;
  virtual void display(ILOSTD(ostream&)) const;
};

IloMyLimitI::IloMyLimitI(IloEnvI* e, IloInt& counter, IloInt limit) :
  IloSearchLimitI(e), _counter(counter), _limit(limit) {}

IlcSearchLimit IloMyLimitI::extract(const IloSolver solver) const {
  return IlcMyLimit(solver, _counter, _limit);
}

IloSearchLimitI* IloMyLimitI::makeClone(IloEnvI* env) const {
  return new (env) IloMyLimitI(env, _counter, _limit);
}

void IloMyLimitI::display(ostream& str) const {
  str << "IloMyLimit(" << _counter << ", " << _limit << ") ";
}

IloSearchLimit IloMyLimit(const IloEnv env, IloInt& counter, IloInt limit) {
  return new (env) IloMyLimitI(env.getImpl(), counter, limit);
}


const char* Names[] = {"green", "yellow", "blue"};

int main() {
  IloEnv env;
  try {
    IloModel model(env);
    const IloInt nbCars    = 8;
    const IloInt nbColors = 3;
    IloInt i;
    IloIntVarArray cars(env, nbCars, 0, nbColors-1);
    IloIntArray colors(env, 3, 0, 1, 2);

    IloIntVarArray cards(env, nbColors);
    cards[0] = IloIntVar(env, 0, 3);
    cards[1] = IloIntVar(env, 3, 3);
    cards[2] = IloIntVar(env, 0, 2);

    model.add(IloDistribute(env, cards, colors, cars));
    model.add(cars[0] != 0);

    IloSolver solver(model);
    IloGoal goal = IloGenerate(env, cars, IloChooseMinSizeInt);
    IloInt counter=0;
    IloSearchLimit limit = IloMyLimit(env, counter, 2);
    IloGoal finalGoal = IloLimitSearch(env, goal, limit);
    solver.startNewSearch(finalGoal);
    while (solver.next())
      {
      solver.out() << solver.getStatus() << " Solution" << endl;
      if (solver.getValue(cars[5]) == 0) { // Car 6 is green
        for (i = 0; i < nbCars; i++) {
          solver.out() << "Car " << i+1 <<  " color:     "
                       << Names[(IloInt)solver.getValue(cars[i])]
                       << endl;
        }
        counter++;
      }
    }
      solver.endSearch();
  }
  catch (IloException& ex) {
    cout << "Error: " << ex << endl;
  }
  env.end();
  return 0;
}

