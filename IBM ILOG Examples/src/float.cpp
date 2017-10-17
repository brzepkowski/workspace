// -------------------------------------------------------------- -*- C++ -*-
// File: ./examples/src/float.cpp
// --------------------------------------------------------------------------
// Licensed Materials - Property of IBM
// 5724-Y47
// Copyright IBM Corporation 1990, 2009. All Rights Reserved.
//
// Note to U.S. Government Users Restricted Rights:
// Use, duplication or disclosure restricted by GSA ADP Schedule
// Contract with IBM Corp.
// --------------------------------------------------------------------------

#include <ilsolver/ilosolverfloat.h>

ILOSTLBEGIN


int can1_example() {
  IloEnv env;
  try {
    IloModel model(env);
    IloNumVar x(env, -2, 2);
    IloNumVar y(env, 0, 10);
    IloNumVar z(env, -100, 100);
    model.add(z == x*y - y + 2*x - 2);
    IloSolver solver(model);
    solver.propagate();
    solver.out() << "z = [" << solver.getMin(z)
                          << ", " << solver.getMax(z)
                          << "]" << endl;
  } catch (IloException& ex) {
    cerr << "Error:" << ex << endl;
  }
  env.end();
  return 0;
}
/*
z = [-36, 22]
*/


// --------------------------------------------------------------------------

int can1gen_example() {
  IloEnv env;
  try {
    IloModel model(env);
    IloNumVar x(env, -2, 2);
    IloNumVar y(env, 0, 10);
    IloNumVar z(env, -100, 100);
    model.add(z == x*y - y + 2*x - 2);
    IloSolver solver(model);
    solver.solve(IloGenerateBounds(env, z, 1e-5));
    solver.out() << "z = [" << solver.getMin(z)
                          << ", " << solver.getMax(z)
                          << "]" << endl;
  } catch (IloException& ex) {
    cerr << "Error:" << ex << endl;
  }
  env.end();
  return 0;
}
/*
z = [-36, 15.04]
*/


// --------------------------------------------------------------------------

int can2_example() {
  IloEnv env;
  try {
    IloModel model(env);
    IloNumVar x(env, -2, 2);
    IloNumVar y(env, -100, 100);
    model.add(x*x + x - 2 == y);
    IloSolver solver(model);
    solver.propagate();
    cout << "y = [" << solver.getMin(y)
		<< ", " << solver.getMax(y)
		<< "]" << endl;
  } catch (IloException& ex) {
    cerr << "Error:" << ex << endl;
  }
  env.end();
  return 0;
}
/*
y = [-4, 4]
*/


// --------------------------------------------------------------------------

int fac1_example() {
  IloEnv env;
  try {
    IloModel model(env);
    IloNumVar x(env, -2, 2);
    IloNumVar y(env, 0, 10);
    IloNumVar z(env, -100, 100);
    model.add((x - 1)*(y + 2) == z);
    IloSolver solver(model);
    solver.propagate();
    cout <<  "z = [" << solver.getMin(z)
		<< ", " << solver.getMax(z)
		<< "]" << endl;
  } catch (IloException& ex) {
    cerr << "Error:" << ex << endl;
  }
  env.end();
  return 0;
}
/*
z = [-36, 12]
*/


// --------------------------------------------------------------------------

int fac2_example() {
  IloEnv env;
  try {
    IloModel model(env);
    IloNumVar x(env, -2, 2);
    IloNumVar y(env, -100, 100);
    model.add((x - 1)*(x + 2) == y );
    IloSolver solver(model);
    solver.propagate();
    cout << "y = [" << solver.getMin(y)
		<< ", " << solver.getMax(y)
		<< "]" << endl;
  } catch (IloException& ex) {
    cerr << "Error:" << ex << endl;
  }
  env.end();
  return 0;
}
/*
y = [-12, 4]
*/


// --------------------------------------------------------------------------

int float_example() {
  IloEnv env;
  try {
    IloModel model(env);
    IloNumVar x(env, -2, 2);
    IloNumVar y(env, -100, 100);
    model.add((x - 1)*(x + 2) == y );
    IloSolver solver(model);
    solver.propagate();
    IlcFloatVar ilcy = solver.getFloatVar(y);
    cout << "y = [" << ilcy.getMin()
		<< ", " << ilcy.getMax()
		<< "]" << endl;
  } catch (IloException& ex) {
    cerr << "Error:" << ex << endl;
  }
  env.end();
  return 0;
}
/*
y = [-12, 4]
*/


// --------------------------------------------------------------------------

int main(int argc, char** argv) {
  can1_example();
  can1gen_example();
  can2_example();
  fac1_example();
  fac2_example();
  float_example();
  return(0);
}

