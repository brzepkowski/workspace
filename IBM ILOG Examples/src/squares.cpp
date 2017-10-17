// -------------------------------------------------------------- -*- C++ -*-
// File: ./examples/src/squares.cpp
// --------------------------------------------------------------------------
// Licensed Materials - Property of IBM
// 5724-Y47
// Copyright IBM Corporation 1990, 2009. All Rights Reserved.
//
// Note to U.S. Government Users Restricted Rights:
// Use, duplication or disclosure restricted by GSA ADP Schedule
// Contract with IBM Corp.
// --------------------------------------------------------------------------

#include <ilsolver/ilosolver.h>

ILCSTLBEGIN

/* The goal IlcGenerateOverlap instantiates all overlap
   relations in dimension k. */

ILCGOAL2(IlcGenerateOverlap, IlcInt, k, IlcBox, container) {
  IlcBoxIterator boxi(container);
  IlcBox a;
  while(boxi.next(a)) {
    IlcBoxIterator boxj(container);
    IlcBox b;
    while (boxj.next(b)) {
      if (a.getImpl() == b.getImpl())
	break;
      if (!container.overlapInDimensionKnown(a,b,k))
	return IlcAnd(IlcOr(container.notOverlapInDimension(a,b,k),
			    container.overlapInDimension(a,b,k)),
		      this);
    }
  }
  return 0;
}

ILOCPGOALWRAPPER2(IloGenerateOverlap, solver, IloInt, k, IloBox, container) {
  return IlcGenerateOverlap(solver, k, solver.getBox(container));
}

/* The goal IlcGeneratePrecedence is similar to IlcGenerateOverlap but
   instantiates the precedence relations. */

ILCGOAL2(IlcGeneratePrecedence, IlcInt, k, IlcBox, container) {
  IlcBoxIterator boxi(container);
  IlcBox a;
  while(boxi.next(a)) {
    IlcBoxIterator boxj(container);
    IlcBox b;
    while (boxj.next(b)) {
      if (a.getImpl() == b.getImpl())
	break;
      if (container.doesNotOverlapInDimension(a,b,k) &&
	  !container.precedenceInDimensionKnown(a,b,k))
	return IlcAnd(IlcOr(container.precedenceInDimension(a,b,k),
			    container.precedenceInDimension(b,a,k)),
		      this);
    }
  }
  return 0;
}

ILOCPGOALWRAPPER2(IloGeneratePrecedence, solver, IloInt, k, IloBox, container) {
  return IlcGeneratePrecedence(solver, k, solver.getBox(container));
}

/* Finally, the goal IlcGenerateOrigin is equivalent to IlcGenerate on
   the origin variables in dimension k. */

ILCGOAL2(IlcGenerateOrigin, IlcInt, k, IlcBox, container) {
  IlcBoxIterator conti(container);
  IlcBox box;
  while(conti.next(box)) {
    if (!box.getOrigin(k).isBound())
      return IlcAnd(IlcInstantiate(box.getOrigin(k)),this);
  }
  return 0;
}

ILOCPGOALWRAPPER2(IloGenerateOrigin, solver, IloInt, k, IloBox, container) {
  return IlcGenerateOrigin(solver, k, solver.getBox(container));
}

/* shorthand for creating squares */

IloBox makeSquare(IloEnv env, IloInt origin_min, IloInt origin_max, IloInt square_size) {
  IloIntVarArray origin(env, 2, IloIntVar(env, origin_min, origin_max),
			        IloIntVar(env, origin_min, origin_max));
  IloIntArray size(env, 2, square_size, square_size);
  return IloBox(env, 2, origin, size);
}

int main ()
{
  /* Here we consider a 2-dimensional packing problem.  The problem
     consists of placing a number of squares in a square area so
     that none of the squares overlap. */

  IloEnv env;
  try {
    IloModel model(env);

    IloInt container_size = 33;
    IloBox container = makeSquare(env, 0, 0, container_size);

    IloInt boxNb = 9;
    IloIntArray square_size(env, boxNb, 18, 15, 14, 10, 9, 8, 7, 4, 1);

    IloArray<IloBox> box(env,boxNb);
    IloInt i;

    for (i = 0; i < boxNb; i++)
      box[i] = makeSquare(env, 0, container_size, square_size[i]);

    for (i = 0; i < boxNb; i++)
      model.add(container.contains(box[i]));
    model.add(container.getNotOverlapConstraint());

    IloSolver solver(model);

    // The order in which the goals are posted is important.  For a
    // packing problem like this one, it is better to first consider the
    // overlap relations.

    IloGoal goal =
      IloGenerateOverlap(env, 0, container) &&
      IloGenerateOverlap(env, 1, container) &&
      IloGeneratePrecedence(env, 0, container) &&
      IloGeneratePrecedence(env, 1, container) &&
      IloGenerateOrigin(env, 0, container) &&
      IloGenerateOrigin(env, 1, container);

    solver.startNewSearch(goal);

    IloInt solutionNb = 0;

    while (solver.next()) {
      solutionNb++;
      solver.out() << "Solution " << solutionNb << endl;
      for (i = 0; i < boxNb; i++)
	solver.out() << "Box[" << i << "] = " << solver.getBox(box[i]) << endl;
      break;
    }
    solver.endSearch();

    solver.printInformation();
  }
  catch (IloException& ex) {
    cout << "Error: " << ex << endl;
  }
  env.end();
  return 0;
}

