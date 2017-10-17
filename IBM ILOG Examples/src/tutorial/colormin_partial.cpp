// -------------------------------------------------------------- -*- C++ -*-
// File: ./examples/src/tutorial/colormin_partial.cpp
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

This map coloring problem involves choosing colors for the countries on
a map in such a way that at most two colors (blue and white) are
used and no neighboring countries are the same color, except for Luxembourg.
In this exercise, you will find a solution for a map coloring problem with six
countries: Belgium, Denmark, France, Germany, Luxembourg, and the Netherlands.

In this problem, two colors are not enough to color the map so that no
neighboring countries are the same color. Therefore, you relax the constraints
relating to Luxembourg. You then create an objective that models the following
wish: Try to avoid having Luxembourg share a color with its neighbors in the
following order of priority: Germany, Belgium, and France.

------------------------------------------------------------ */

#include <ilsolver/ilosolverany.h>
ILOSTLBEGIN

char blue[]="blue";
char white[]="white";
char yellow[]="yellow";

void print(IloSolver solver, char* name, IloAnyVar color) {
        solver.out() << name << (char*)solver.getAnyValue(color) << endl;
}

int main(){
  IloEnv env;
  try {
    IloModel model(env);
    //Create the array of values
    //Create the decision variables
    model.add(France != Belgium);
    model.add(France != Germany);
    model.add(Belgium != Netherlands);
    model.add(Germany != Netherlands);
    model.add(Germany != Denmark);
    model.add(Germany != Belgium);
    //Create the objective
    //Add the objective to the model
    //Create an instance of IloSolver
    //Search for a solution
      {
      solver.out() << solver.getStatus() << " Solution" << endl;
      solver.out() << "Objective = " << solver.getValue(obj) << endl;
      print(solver, "Belgium:     ", Belgium);
      print(solver, "Denmark:     ", Denmark);
      print(solver, "France:      ", France);
      print(solver, "Germany:     ", Germany);
      print(solver, "Netherlands: ", Netherlands);
      print(solver, "Luxembourg:  ", Luxembourg);
    }
    else
      solver.out() << "No Solution" << endl;
  }
  catch (IloException& ex) {
    cerr << "Error: " << ex << endl;
  }
  env.end();
  return 0;
}
