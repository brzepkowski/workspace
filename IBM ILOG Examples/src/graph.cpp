// -------------------------------------------------------------- -*- C++ -*-
// File: ./examples/src/graph.cpp
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

void testGraph(IloInt nTest)
{
  IloEnv env;
  try {
    IloModel model(env);
    IloInt n = (nTest%2)?nTest+1:nTest;
    env.out() << "--------------------------------------" << endl;
    IloInt nbNodes = n*(n-1)/2;
    IloInt nbColors = n-1;
    IloInt i, j;
    IloIntVarArray vars(env,nbNodes,0,nbColors-1);
    for(i = 0; i < n-2; i++){
      model.add(vars[i] < vars[i+1]);
    }
    for(i = 0; i < n; i++){
      IloIntVarArray clique(env,n-1);
      for(j = 0; j < n-1; j++){
            clique[j] = vars[createClique(i,j, n)];
      }
      model.add(IloAllDiff(env,clique));
    }
    IloSolver solver(model);
    IloTimer timer(env);
    // IloExtendedLevel
    solver.setDefaultFilterLevel(IloAllDiffCt,IloExtendedLevel);
    timer.start();
    if (!solver.solve(IloGenerate(env,vars,IloChooseMinSizeInt)))
      solver.out() << "No solution" << endl;
    solver.out() << "IloExtendedLevel \t clique size:" << n-1;
    solver.out() << "\t#fails:\t" << solver.getNumberOfFails() << endl;
    solver.out() << "elapsed time: " << timer.getTime() << " s" << endl;
    // Redundant constraint for other levels
    IloIntVarArray cards(env,nbColors,nbNodes/nbColors,
                         nbNodes/nbColors);
    IloConstraint distribute=IloDistribute(env,cards,vars);
    model.add(distribute);
    // IloMediumLevel
    solver.setDefaultFilterLevel(IloAllDiffCt,IloMediumLevel);
    timer.restart();
    if (! solver.solve(IloGenerate(env,vars,IloChooseMinSizeInt)))
       solver.out() << "No solution" << endl;
    solver.out() << "IloMediumLevel \t \t clique size:" << n-1;
    solver.out() << "\t#fails:\t" << solver.getNumberOfFails() << endl;
    solver.out() << "elapsed time: " <<  timer.getTime() << " s" << endl;
    // IloBasicLevel
    solver.setDefaultFilterLevel(IloAllDiffCt,IloBasicLevel);
    timer.restart();
    if (! solver.solve(IloGenerate(env,vars,IloChooseMinSizeInt)))
      solver.out() << "No solution" << endl;
    solver.out() << "IloBasicLevel \t \t clique size:" << n-1;
    solver.out() << "\t#fails:\t" << solver.getNumberOfFails() << endl;
    solver.out() << "elapsed time: " << timer.getTime() << " s" << endl;
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
/*
--------------------------------------
IloExtendedLevel      clique size:27 #fails: 0       cpu time: 0.651 s
IloMediumLevel        clique size:27 #fails: 1       cpu time: 0.09 s
IloBasicLevel         clique size:27 #fails: 1       cpu time: 0.05 s

--------------------------------------
IloExtendedLevel      clique size:29 #fails: 4       cpu time: 0.541 s
IloMediumLevel        clique size:29 #fails: 7       cpu time: 0.11 s
IloBasicLevel         clique size:29 #fails: 23      cpu time: 0.07 s

--------------------------------------
IloExtendedLevel      clique size:31 #fails: 4       cpu time: 0.751 s
IloMediumLevel        clique size:31 #fails: 49      cpu time: 0.16 s
IloBasicLevel         clique size:31 #fails: 65      cpu time: 0.11 s

--------------------------------------
IloExtendedLevel      clique size:51 #fails: 501     cpu time: 9.754 s
IloMediumLevel        clique size:51 #fails: 10906   cpu time: 13.299 s
IloBasicLevel         clique size:51 #fails: 24512   cpu time: 19.578 s

--------------------------------------
IloExtendedLevel      clique size:53 #fails: 2       cpu time: 10.896 s
IloMediumLevel        clique size:53 #fails: 368     cpu time: 1.121 s
IloBasicLevel         clique size:53 #fails: 13159   cpu time: 9.063 s

--------------------------------------
IloExtendedLevel      clique size:55 #fails: 2       cpu time: 13.35 s
IloMediumLevel        clique size:55 #fails: 27      cpu time: 1.221 s
IloBasicLevel         clique size:55 #fails: 94      cpu time: 0.892 s

--------------------------------------
IloExtendedLevel      clique size:61 #fails: 5       cpu time: 22.502 s
IloMediumLevel        clique size:61 #fails: 120408491       cpu time: 172926 s

--------------------------------------
IloExtendedLevel      clique size:71 #fails: 5       cpu time: 49.471 s
IloMediumLevel        clique size:71 #fails: 586     cpu time: 5.217 s
IloBasicLevel         clique size:71 #fails: 1167    cpu time: 4.977 s

--------------------------------------
IloExtendedLevel      clique size:73 #fails: 54      cpu time: 57.642 s
IloMediumLevel        clique size:73 #fails: 7195    cpu time: 22.192 s
IloBasicLevel         clique size:73 #fails: 234671  cpu time: 198.446 s

--------------------------------------
IloExtendedLevel      clique size:75 #fails: 5       cpu time: 69.119 s

--------------------------------------
IloExtendedLevel      clique size:27 #fails: 0       cpu time: 0.54 s



*/































