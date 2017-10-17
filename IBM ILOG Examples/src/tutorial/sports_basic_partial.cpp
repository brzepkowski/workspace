// -------------------------------------------------------------- -*- C++ -*-
// File: ./examples/src/tutorial/sports_basic_partial.cpp
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
ILOSTLBEGIN

IloInt nbTeams;
IloInt nbWeeks;
IloInt nbPeriods;
IloInt nbSlots;

int main(int argc, char** argv){
  IloInt i;
  IloInt j;
  IloInt m;
  IloInt p;
  IloInt w;

  nbTeams = (argc>1? atol(argv[1] ) : 8 );
  if ( nbTeams % 2 ) ++nbTeams;
  nbWeeks = nbTeams-1;
  nbPeriods = nbTeams /2;
  nbSlots = nbWeeks * nbPeriods;
  IloEnv env;
    try {
      IloModel model(env);
//Create the matrix of slots
//Declare the match variables
//Declare the home team and away team variables
//Add the constraint that all matches are different
//Add the constraint that every team must play each week, but only once
//Declare the teamValues array
//Declare the periodTeamVars array
//Declare the cards array
//Add the distribute constraint
//Create the tuple set
//Add the table constraint
//Create an instance of IloSolver
//Search for a solution
      }
    catch (IloException& ex) {
       cerr << "Error: " << ex << endl;
    }
  env.end();
  return 0;
}