// -------------------------------------------------------------- -*- C++ -*-
// File: ./examples/src/freq.cpp
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

Frequency assignment problem
----------------------------

Problem Description

The problem is given here in the form of discrete data; that is,
each frequency is represented by a number that can be called its
channel number.  For practical purposes, the network is divided
into cells (this problem is an actual cellular phone problem).
In each cell, there is a transmitter which uses different
channels.  The shape of the cells have been determined, as well
as the precise location where the transmitters will be
installed.  For each of these cells, traffic requires a number
of frequencies.

Between two cells, the distance between frequencies is given in
the matrix on the next page.

The problem of frequency assignment is to avoid interference.
As a consequence, the distance between the frequencies within a
cell must be greater than 16.  To avoid inter-cell interference,
the distance must vary because of the geography.  In the
example, we're assuming that the same frequencies can be used in
the first cell and in the fourth cell, but we cannot use the same
frequencies in the first cell and the third cell.

------------------------------------------------------------ */
#include <ilsolver/ilosolverint.h>

ILOSTLBEGIN

const int nbCell               = 25;
const int nbAvailFreq          = 256;
const int nbChannel[nbCell] =
  { 8,6,6,1,4,4,8,8,8,8,4,9,8,4,4,10,8,9,8,4,5,4,8,1,1 };
const int dist[nbCell][nbCell] = {
  { 16,1,1,0,0,0,0,0,1,1,1,1,1,2,2,1,1,0,0,0,2,2,1,1,1 },
  { 1,16,2,0,0,0,0,0,2,2,1,1,1,2,2,1,1,0,0,0,0,0,0,0,0 },
  { 1,2,16,0,0,0,0,0,2,2,1,1,1,2,2,1,1,0,0,0,0,0,0,0,0 },
  { 0,0,0,16,2,2,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,1,1 },
  { 0,0,0,2,16,2,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,1,1 },
  { 0,0,0,2,2,16,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,1,1 },
  { 0,0,0,0,0,0,16,2,0,0,1,1,1,0,0,1,1,1,1,2,0,0,0,1,1 },
  { 0,0,0,0,0,0,2,16,0,0,1,1,1,0,0,1,1,1,1,2,0,0,0,1,1 },
  { 1,2,2,0,0,0,0,0,16,2,2,2,2,2,2,1,1,1,1,1,1,1,0,1,1 },
  { 1,2,2,0,0,0,0,0,2,16,2,2,2,2,2,1,1,1,1,1,1,1,0,1,1 },
  { 1,1,1,0,0,0,1,1,2,2,16,2,2,2,2,2,2,1,1,2,1,1,0,1,1 },
  { 1,1,1,0,0,0,1,1,2,2,2,16,2,2,2,2,2,1,1,2,1,1,0,1,1 },
  { 1,1,1,0,0,0,1,1,2,2,2,2,16,2,2,2,2,1,1,2,1,1,0,1,1 },
  { 2,2,2,0,0,0,0,0,2,2,2,2,2,16,2,1,1,1,1,1,1,1,1,1,1 },
  { 2,2,2,0,0,0,0,0,2,2,2,2,2,2,16,1,1,1,1,1,1,1,1,1,1 },
  { 1,1,1,0,0,0,1,1,1,1,2,2,2,1,1,16,2,2,2,1,2,2,1,2,2 },
  { 1,1,1,0,0,0,1,1,1,1,2,2,2,1,1,2,16,2,2,1,2,2,1,2,2 },
  { 0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,2,2,16,2,2,1,1,0,2,2 },
  { 0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,2,2,2,16,2,1,1,0,2,2 },
  { 0,0,0,1,1,1,2,2,1,1,2,2,2,1,1,1,1,2,2,16,1,1,0,1,1 },
  { 2,0,0,0,0,0,0,0,1,1,1,1,1,1,1,2,2,1,1,1,16,2,1,2,2 },
  { 2,0,0,0,0,0,0,0,1,1,1,1,1,1,1,2,2,1,1,1,2,16,1,2,2 },
  { 1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,1,1,16,1,1 },
  { 1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,2,2,2,2,1,2,2,1,16,2 },
  { 1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,2,2,2,2,1,2,2,1,2,16 }
};

IlcRevInt** freqUsage;


class IlcFreqConstraintI : public IlcConstraintI {
protected:
  IlcIntVarArray _x;
 public:
  IlcFreqConstraintI(IloSolver s, IlcIntVarArray x)
    : IlcConstraintI(s), _x(x) {}
  ~IlcFreqConstraintI() {}
  virtual void post();
  virtual void propagate() {}
  void varDemon(IlcIntVar var);
};

ILCCTDEMON1(FreqDemon, IlcFreqConstraintI, varDemon, IlcIntVar, var)

void IlcFreqConstraintI::post () {
  IlcInt i;
  for (i = 0; i < _x.getSize(); i++)
    _x[i].whenValue(FreqDemon(getSolver(), this, _x[i]));
}

void IlcFreqConstraintI::varDemon (IlcIntVar var) {
  IlcRevInt& freq = *freqUsage[var.getValue()];
  freq.setValue(getSolver(), freq.getValue() + 1);
}

IlcConstraint IlcFreqConstraint(IloSolver s, IlcIntVarArray x) {
  return new (s.getHeap()) IlcFreqConstraintI(s, x);
}



ILOCPCONSTRAINTWRAPPER1(IloFreqConstraint, solver, IloIntVarArray, _vars) {
  use(solver, _vars);
  return IlcFreqConstraint(solver, solver.getIntVarArray(_vars));
}


IlcInt bestFreq(IlcIntVar var) {
  IlcInt max = -1;
  IlcInt maxIndex = 0;
  IlcInt i;
  for(IlcIntExpIterator iter(var);iter.ok();++iter){
    i=*iter;
    if (*freqUsage[i] > max){
      max = *freqUsage[i];
      maxIndex = i;
    }
  }
  return maxIndex;
}

ILCGOAL1(Instantiate, IlcIntVar, var) {
  if (var.isBound()) return 0;
  IlcInt val = bestFreq(var);
  return IlcOr(var == val,
               IlcAnd(var != val,
                      this));
}

void SetCluster(IlcIntVar var, IlcInt clusterSize) {
  var.setObject((IlcAny) clusterSize);
}

IlcInt GetCluster(IlcIntVar var) {
  return (IlcInt) (var.getObject());
}

static IlcChooseIndex2(IlcChooseClusterFreq,
                       var.getSize(),
                       GetCluster(var),
                       IlcIntVar)

ILCGOAL1(MyGenerate, IlcIntVarArray, vars) {
  IloSolver solver = getSolver();
  IlcInt chosen = IlcChooseClusterFreq(vars);
  if(chosen == -1)
    return 0;
  return IlcAnd(Instantiate(solver, vars[chosen]),this);
}

ILOCPGOALWRAPPER1(MyIloGenerate, solver, IloIntVarArray, vars) {
  return MyGenerate(solver, solver.getIntVarArray(vars));
}

IlcInt partialsum[nbCell];
inline IlcInt acc(IlcInt i, IlcInt j) {
  return partialsum[i]+j;
}

int main(){
  IloEnv env;
  try {
    IloModel model(env);
    IloSolver solver(env);

    IlcInt i, ii, j, jj;
    IlcInt usedFreq = 0;
    IlcInt nbXmiter = 0;

    for (i = 0; i < nbCell; i++) {
      partialsum[i] = nbXmiter;
      nbXmiter += nbChannel[i];
    }

    IloIntVarArray X(env, nbXmiter, 0, nbAvailFreq - 1);

    for (i = 0; i < nbCell; i++)
      for (j = i; j < nbCell; j++)
        for (ii = 0; ii < nbChannel[i]; ii++)
          for (jj = 0; jj < nbChannel[j]; jj++)
            if (dist[i][j] != 0 && (i != j || ii != jj))
              model.add(IloAbs(X[acc(i,ii)] - X[acc(j,jj)]) >= dist[i][j]);

    model.add(IloFreqConstraint(env, X));

    solver.extract(model);

    for (i = 0; i < nbCell; i++)
      for (ii = 0; ii < nbChannel[i]; ii++)
        SetCluster(solver.getIntVar(X[acc(i,ii)]), nbChannel[i]);

    freqUsage = new (env) IlcRevInt* [nbAvailFreq];
    for (i = 0; i < nbAvailFreq; i++)
      freqUsage[i] = new (env) IlcRevInt(solver);

    solver.solve(MyIloGenerate(env, X));

    for (i = 0; i < nbCell; i++) {
      for (j = 0; j < nbChannel[i]; j++)
        solver.out() << solver.getValue(X[acc(i,j)]) << "  " ;
      solver.out() << endl;
    }
    solver.out() << "Total # of sites       " << nbXmiter << endl;
    for (i = 0; i < nbAvailFreq; i++)
      if (freqUsage[i]->getValue() != 0)
        usedFreq++;
    solver.out() << "Total # of frequencies " << usedFreq << endl;
  }
  catch (IloException& ex) {
    cout << "Error: " << ex << endl;
  }
  env.end();
  return 0;
}
/*
25  41  57  73  89  105  121  137
1  21  37  53  71  87
6  23  39  55  75  103
0
2  18  34  50
4  20  36  52
75  10  27  43  59  96  112  128
13  30  46  62  80  99  115  131
13  35  51  67  83  99  115  131
19  64  80  96  112  128  144  160
0  16  32  48
8  26  42  58  74  90  106  122  138
2  24  40  56  72  88  104  120
4  28  44  60
11  30  46  62
20  36  52  68  84  100  116  132  148  164
18  34  50  66  82  98  114  130
9  25  41  57  73  89  105  121  137
7  23  39  55  71  87  103  119
5  21  37  53
10  27  43  59  75
6  22  38  54
0  21  37  53  71  87  103  128
1
3
Total # of sites       148
Total # of frequencies 95
*/
