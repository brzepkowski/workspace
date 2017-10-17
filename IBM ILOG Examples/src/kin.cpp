// -------------------------------------------------------------- -*- C++ -*-
// File: ./examples/src/kin.cpp
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

Problem description
-------------------

constant:
   range idx= [1..6];

Variable:
   s: array[idx] in [-1..1];
   c: array[idx] in [-1..1] start 0.5;
   a: array[1..4] in [-1e8..1e8];
Body: solve system all
   constraint tr[i in idx] : s[i]^2 + c[i]^2 = 1;
   c[5]*s[6]*a[1]+c[6]*a[2]= 0.4077;
   c[1]*s[5]*a[2] + s[1]*c[5]= 1.9115;
   s[5]*a[3] = 1.9791;
   c[1]*a[4] = 4.0616;
   s[1]*a[4] = 1.7172;
   3*s[2] + 2*s[3] + s[4] = 3.9701;
   a[1] = s[2] - s[3] - s[4];
   a[2] = c[2] + c[3] + c[4];
   a[3] = s[2] + s[3] + s[4];
   a[4] = 3*c[2]+2*c[3]+c[4];

------------------------------------------------------------ */

#include <ilsolver/ilosolverfloat.h>

ILOSTLBEGIN

int main(int argc, char* argv[]) {
 IloEnv env;
 try {
  IloModel model(env);

    IloNumVarArray s(env,6, -1, 1);
    IloNumVarArray c(env,6, -1, 1);
    IloNumVarArray a(env,4, -1e8, 1e8);
    IloNumVarArray vars(env,16);

    IloInt i;
    for(i = 0; i < s.getSize(); i++)
      vars[i] = s[i];
    for(i = 0; i < c.getSize(); i++)
      vars[i+s.getSize()] = c[i];
    for(i = 0; i < a.getSize(); i++)
      vars[i+s.getSize()+c.getSize()] = a[i];

    for(i = 0; i < s.getSize(); i++)
      model.add(IloSquare(s[i]) + IloSquare(c[i]) == 1);

    model.add( c[4]*s[5]*a[0] + c[5]*a[1] == 0.4077);
    model.add( c[0]*s[4]*a[1] + s[0]*c[4] == 1.9115);
    model.add( s[4]*a[2] == 1.9791);
    model.add( c[0]*a[3] == 4.0616);
    model.add( s[0]*a[3] == 1.7172);
    model.add( 3*s[1] + 2*s[2] + s[3] == 3.9701);
    model.add( a[0] == s[1] - s[2] - s[3]);
    model.add( a[1] == c[1] + c[2] + c[3]);
    model.add( a[2] == s[1] + s[2] + s[3]);
    model.add( a[3] == 3*c[1] + 2*c[2] + c[3]);

    IloSolver solver(env);

    solver.useNonLinConstraint();
    solver.out().precision(12);

    solver.extract(model);

    solver.startNewSearch(IloSplit(env, vars,IloFalse));

    IloInt nbSol = 0;
    if (solver.next()) {
      nbSol++;
      solver.out() << "------------------------------" << endl;
      solver.out() << "Sol #" << nbSol << "   ";
      if (solver.isNonLinSafe())
	solver.out() << "[SAFE] ";
      solver.out() << endl;
      for (i=0; i < s.getSize(); i++)
	solver.out() << "s[" << i << "] = " << solver.getFloatVar(s[i]) << endl;
      for (i=0; i < c.getSize(); i++)
	solver.out() << "c[" << i << "] = " << solver.getFloatVar(c[i]) << endl;
      for (i=0; i < a.getSize(); i++)
	solver.out() << "a[" << i << "] = " << solver.getFloatVar(a[i]) << endl;
    }
    solver.out() << "------------------------------" << endl;
    solver.endSearch();

    solver.printInformation();
  }
  catch (IloException& ex) {
    cout << "Error: " << ex << endl;
  }
  env.end();
  return 0;
}
