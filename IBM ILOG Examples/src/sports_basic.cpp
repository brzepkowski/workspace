// -------------------------------------------------------------- -*- C++ -*-
// File: ./examples/src/sports_basic.cpp
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
 n teams and n-1 weeks and n/2 periods
 every two teams play each other exactly once
 every team plays one game in each week
 no team plays more than twice in the same period

------------------------------------------------------------ */

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

      IloArray<IloIntVarArray> slotVars(env, nbPeriods);

      for(i = 0; i < nbPeriods; i++)
        slotVars[i] = IloIntVarArray(env, nbWeeks, 0, nbSlots-1);
// ie : NumVarMatrix slotVars = IloArray<IloNumVarArray>(env, nbPeriods);
// build copy vector of match variables matrix
      IloIntVarArray matchVars(env, nbSlots);
      IloInt s = 0;
      for (p = 0; p < nbPeriods; ++p) {
        for (w = 0; w < nbWeeks; ++w) {
          matchVars[s] = slotVars[p][w];
          ++s;
        }
      }
      IloArray<IloIntVarArray> homeTeamVars(env, nbPeriods);
      IloArray<IloIntVarArray> awayTeamVars(env, nbPeriods);

      for (p = 0; p < nbPeriods; ++p) {
        homeTeamVars[p]  = IloIntVarArray(env, nbWeeks, 0, nbTeams-1);
        awayTeamVars[p] = IloIntVarArray(env, nbWeeks, 0, nbTeams-1);
      }
// ---------------------------------
// Constraint 1 : each team plays each other exactly once,
// i.e. a match (T1, T2) occurs exactly once.
// ---------------------------------
      model.add( IloAllDiff(env, matchVars));
// ---------------------------------
// Constraint 2 : each week, all teams do play.
// remember 2*nbPeriods == nbTeams.
// ---------------------------------
      for (w = 0; w < nbWeeks; ++w) {
        IloIntVarArray weekTeams(env, nbTeams);
        for (p = 0; p < nbPeriods; ++p) {
          weekTeams[2*p]   = homeTeamVars [p][w];
          weekTeams[2*p+1] = awayTeamVars[p][w];
        }
        model.add( IloAllDiff(env, weekTeams) );
      }
// ---------------------------------
// constraint 3 : in each period, each team plays no more than twice.
// one can demonstrate that the number of occurences of each team in
// one period is at least 1, so we constrain it to be in [1..2]
// ---------------------------------
      IloIntArray teams(env, nbTeams);
        for (m = 0; m < nbTeams; ++m)
          teams[m] = m;
          for (p = 0; p < nbPeriods; ++p) {
            IloIntVarArray periodTeams(env, 2*nbWeeks);
            for ( w = 0; w < nbWeeks; ++w) {
              periodTeams[ 2*w ]   = homeTeamVars [p][w];
              periodTeams[ 2*w+1 ] = awayTeamVars[p][w];
            };
            IloIntVarArray cardVars(env, nbTeams, 1, 2);
            model.add( IloDistribute(env, cardVars, teams, periodTeams) );
          }
// ---------------------------------
// Constraint 4: link team and match vars with table constraint
// ---------------------------------
      IloIntTupleSet tuple(env, 3);
      IloInt m = 0;
        for (i=0; i < nbTeams; ++i) {
          for ( j=i+1; j < nbTeams; ++j) {
            tuple.add(IloIntArray(env, 3, i, j, m));
            ++m;
          }
        }
      assert( m == nbSlots );
      for (p = 0; p < nbPeriods; ++p) {
        for (w = 0; w < nbWeeks; ++w) {
          IloIntVarArray slotConsistencyTest(env, 3);
          slotConsistencyTest[0] = homeTeamVars[p][w];
          slotConsistencyTest[1] = awayTeamVars[p][w];
          slotConsistencyTest[2] = slotVars[p][w];
          model.add(IloTableConstraint(env,
                                       slotConsistencyTest,
                                       tuple,
                                       IloTrue));
        }
      }
      IloSolver solver(model);
      if (solver.solve()) {
        cout << endl << "SOLUTION" << endl;
        for (p=0; p < nbPeriods; ++p) {
             cout << "period " << p << " : ";
               for (w=0; w < nbWeeks; ++w) {
               cout << solver.getValue(homeTeamVars[p][w]) << " vs "
                   << solver.getValue(awayTeamVars[p][w]) << " - " ;
             }
         cout << endl;
        }
        solver.printInformation();
          }
      else
        cout << "**** NO SOLUTION ****" << endl;
      }
    catch (IloException& ex) {
       cerr << "Error: " << ex << endl;
    }
  env.end();
  return 0;
}

/* ------------------------------------------------------------
%
* sports scheduling teams: 8 weeks: 7 periods: 4

SOLUTION
period 0 : 0 vs 1 - 0 vs 2 - 1 vs 2 - 3 vs 4 - 3 vs 5 - 4 vs 5 - 6 vs 7 -
period 1 : 2 vs 3 - 3 vs 6 - 4 vs 6 - 0 vs 7 - 0 vs 4 - 2 vs 7 - 1 vs 5 -
period 2 : 4 vs 7 - 5 vs 7 - 0 vs 5 - 1 vs 6 - 2 vs 6 - 1 vs 3 - 0 vs 3 -
period 3 : 5 vs 6 - 1 vs 4 - 3 vs 7 - 2 vs 5 - 1 vs 7 - 0 vs 6 - 2 vs 4 -
%
------------------------------------------------------------ */
