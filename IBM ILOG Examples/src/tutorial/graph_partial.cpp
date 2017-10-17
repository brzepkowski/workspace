// -------------------------------------------------------------- -*- C++ -*-
// File: ./examples/src/tutorial/graph_partial.cpp
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

In this example, you will solve a graph coloring problem. Graph coloring
problems may seem like puzzles, but they have many applications in real
world settings, including scheduling, testing circuit boards, and assigning
frequencies.

A graph is composed of nodes. Nodes that must be different colors are
connected by lines, called edges. A clique is a set of nodes where every
node is linked to every other node by an edge. Two types of cliques, maximal
cliques and the largest clique, are interesting for this lesson. A maximal
clique is a clique such that no other clique contains it. The largest clique
is simply the clique containing the largest number of nodes.

You will solve a graph coloring problem for a special kind of graph. This
graph has n*(n-1)/2 nodes, where n is an even number. Every node belongs to
exactly two maximal cliques of size n-1.

There is a theorem that states that the minimum number of colors needed to
color a graph so that two nodes that are linked by an edge are different
colors is greater than or equal to the size of the largest clique. In this
problem, the largest clique is the same size as the maximal cliques of size n-1.
Therefore, the minimum number of colors needed to color this graph
is greater than or equal to n-1. In this problem, you will try to color the
graph with n-1 colors and see if it is possible.

For example, if you have a graph with n=4, there are 6 nodes (4 * (4-1)/2).
Every node belongs to exactly two maximal cliques of size 3 (4-1).
Here are the maximal cliques:
clique 0 = {0,1,2}
clique 1 = {0,3,4}
clique 2 = {1,3,5}
clique 3 = {2,4,5}

The largest clique is also size 3 (n-1). Therefore, the minimum number of
colors needed to color this graph is greater than or equal to 3. You can,
in fact, color this graph with 3 colors. None of the nodes connected by an
edge share a color.

------------------------------------------------------------ */
#include <ilsolver/ilosolverint.h>
ILOSTLBEGIN

IloInt createClique(IloInt i, IloInt j, IloInt n){
  if (j >= i) return (i*n-i*(i+1)/2+j-i);
  else return createClique(j,i-1,n);
}

//Declare the testGraph function

{
  IloEnv env;
  try {
    IloModel model(env);
    IloInt n = (nTest%2)?nTest+1:nTest;
    env.out() << "--------------------------------------" << endl;
    IloInt nbNodes = n*(n-1)/2;
    IloInt nbColors = n-1;
    IloInt i, j;

    //Declare the decision variables
    //Create the cliques and add the IloAllDiff constraint

    IloSolver solver(model);

    //Create the timer
    //Set the default filter level
    //Start the timer
    //Search for a solution

    solver.out() << "IloExtendedLevel \t clique size:" << nTest;
    solver.out() << "\t#fails:\t" << solver.getNumberOfFails() << "\t";
    solver.out() << "cpu time: " << timer.getTime() << " s" << endl;

    //Add the redundant IloDistribute constraint

    solver.setDefaultFilterLevel(IloAllDiffCt,IloMediumLevel);
    timer.restart();
    if (! solver.solve(IloGenerate(env,vars,IloChooseMinSizeInt)))
       solver.out() << "No solution" << endl;
    solver.out() << "IloMediumLevel \t \t clique size:" << nTest;
    solver.out() << "\t#fails:\t" << solver.getNumberOfFails() << "\t";
    solver.out() << "cpu time: " <<  timer.getTime() << " s" << endl;

    solver.setDefaultFilterLevel(IloAllDiffCt,IloBasicLevel);
    timer.restart();
    if (! solver.solve(IloGenerate(env,vars,IloChooseMinSizeInt)))
      solver.out() << "No solution" << endl;
    solver.out() << "IloBasicLevel \t \t clique size:" << nTest;
    solver.out() << "\t#fails:\t" << solver.getNumberOfFails() << "\t";
    solver.out() << "cpu time: " << timer.getTime() << " s" << endl;
    solver.out() << endl;
  }
  catch (IloException& ex) {
    cerr << "Error: " << ex << endl;
  }
  env.end();
}
int main(int argc, char** argv){
  IloInt b1=(argc>1)?atoi(argv[1]):27;
  IloInt b2=(argc>2)?atoi(argv[2]):b1+4;
  for(IloInt nTest = b1; nTest < b2+1; nTest+=2){
    testGraph(nTest);
  }
  return 0;
}













