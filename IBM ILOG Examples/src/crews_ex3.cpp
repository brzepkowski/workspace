// -------------------------------------------------------------- -*- C++ -*-
// File: ./examples/src/crews_ex3.cpp
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

An airline has to assign flight attendants to flights. There are 10
flights a day. Each flight requires a certain number of flight attendants,
which varies with the size of the plane. The company employs 20 flight
attendants.

There are certain constraints on the composition of each crew. Each crew
must contain a certain minimum number of senior and junior staff and a
certain minimum numbers of attendants fluent in the following languages:
French, German, Italian, and Spanish. Attendants must rest for at least
two flights between assignments.

------------------------------------------------------------ */
#include <ilsolver/ilosolverset.h>

ILOSTLBEGIN

void TeamConstraints(IloModel model,
                     const IloIntSetVar crews,
                     const IloNum crewSize,
                     const IloNumArray crewRequirements,
                     const IloIntSetVarArray attributeSets,
                     const IloArray<IloNumArray> dataArrays)  {
  IloEnv env = model.getEnv();
  model.add(IloCard(crews) == crewSize);
  IloInt size = crewRequirements.getSize();
  for (IloInt i = 0; i < size ; i++) {
    IloIntSetVar intersection(env, dataArrays[i]);
    model.add(IloEqIntersection(env, intersection, crews, attributeSets[i]));
    model.add(IloCard(intersection) >= crewRequirements[i]);
    }
}

void Print(IloSolver solver, IloIntSetVar crews, char** Names) {
  for(IloIntSet::Iterator iter(solver.getIntSetValue(crews));iter.ok();++iter){
    solver.out() << Names[(IloInt)*iter] << " ";
  }
  solver.out() << endl;
}

int main() {
  IloEnv env;
  try {
    IloModel model(env);
    IloInt i;

    char* Names[20];

    for (i=0; i < 20; i++){
      Names[i]=new (env) char[10];
    }
    strcpy(Names[0],"Bill");
    strcpy(Names[1],"Bob");
    strcpy(Names[2],"Carol");
    strcpy(Names[3],"Carolyn");
    strcpy(Names[4],"Cathy");
    strcpy(Names[5],"David");
    strcpy(Names[6],"Ed");
    strcpy(Names[7],"Fred");
    strcpy(Names[8],"Heather");
    strcpy(Names[9],"Inez");
    strcpy(Names[10],"Janet");
    strcpy(Names[11],"Jean");
    strcpy(Names[12],"Jeremy");
    strcpy(Names[13],"Joe");
    strcpy(Names[14],"Juliet");
    strcpy(Names[15],"Marilyn");
    strcpy(Names[16],"Mario");
    strcpy(Names[17],"Ron");
    strcpy(Names[18],"Tom");
    strcpy(Names[19],"Tracy");

    enum Employee {Bill, Bob, Carol, Carolyn, Cathy,
                   David, Ed, Fred, Heather, Inez,
                   Janet, Jean, Jeremy, Joe, Juliet,
                   Marilyn, Mario, Ron, Tom, Tracy};

    IloNumArray Staff(env, 20,
                      Bill, Bob, Carol, Carolyn, Cathy,
                      David, Ed, Fred, Heather, Inez,
                      Janet, Jean, Jeremy, Joe, Juliet,
                      Marilyn, Mario, Ron, Tom, Tracy);
    const IloInt nAttributes = 6;
    const IloInt nCrews = 10;

    IloIntSetVarArray crews(env, nCrews);
    for(i = 0; i < nCrews; i++)
      crews[i] = IloIntSetVar(env, Staff);


    IloNumArray SeniorArray(env, 10,
                            Bill, Bob, Carol, Carolyn, Ed,
                            Fred, Janet, Marilyn, Mario, Tracy);

    IloNumArray JuniorArray(env, 10,
                            Cathy, David, Heather, Inez, Jean,
                            Jeremy, Joe, Juliet, Ron, Tom);

    IloNumArray FrenchArray(env, 5, Bill, Inez, Jean, Juliet, Ron);

    IloNumArray GermanArray(env, 5, Cathy, Jeremy, Juliet, Mario, Tom);

    IloNumArray SpanishArray(env, 7, Bill, Fred, Heather, Inez, Joe,
                             Marilyn, Mario);
    IloNumArray ItalianArray(env, 5, Bob, Carol, Janet, Marilyn, Tracy);
    IloArray<IloNumArray> dataArrays(env, nAttributes);
    dataArrays[0] = SeniorArray;
    dataArrays[1] = JuniorArray;
    dataArrays[2] = FrenchArray;
    dataArrays[3] = GermanArray;
    dataArrays[4] = SpanishArray;
    dataArrays[5] = ItalianArray;
    IloIntSetVar Senior(env, SeniorArray, SeniorArray);
    IloIntSetVar Junior(env, JuniorArray, JuniorArray);
    IloIntSetVar French(env, FrenchArray, FrenchArray);
    IloIntSetVar German(env, GermanArray, GermanArray);
    IloIntSetVar Spanish(env, SpanishArray, SpanishArray);
    IloIntSetVar Italian(env, ItalianArray, ItalianArray);
    IloNumArray crewSize(env, nCrews, 4, 5, 5, 6, 7, 4, 5, 6, 6, 7);
    IloIntSetVarArray attributeSets(env, nAttributes, Senior, Junior,
                                    French, German, Spanish, Italian);

    enum CrewRequirementsElements {SeniorSet, JuniorSet, FrenchSet,
                                   GermanSet, SpanishSet, ItalianSet};

   IloArray<IloNumArray> crewRequirements(env, nCrews);
   for (i =0; i < nCrews; i++)
     crewRequirements[i] = IloNumArray(env, nAttributes,
                                       1, 1, 1, 1, 1, 1);

     crewRequirements[3][SeniorSet] = 2;
     crewRequirements[3][JuniorSet] = 2;
     crewRequirements[3][FrenchSet] = 2;
     crewRequirements[3][SpanishSet] = 2;
     crewRequirements[3][ItalianSet] = 2;
     crewRequirements[4][SeniorSet] = 3;
     crewRequirements[4][JuniorSet] = 2;
     crewRequirements[4][FrenchSet] = 2;
     crewRequirements[4][SpanishSet] = 2;
     crewRequirements[8][SeniorSet] = 2;
     crewRequirements[8][JuniorSet] = 2;
     crewRequirements[9][SeniorSet] = 3;
     crewRequirements[9][JuniorSet] = 3;

     for(i=0; i< nCrews;i++)
       TeamConstraints(model, crews[i], crewSize[i],
                       crewRequirements[i], attributeSets, dataArrays);

     for(i = 0 ; i < nCrews-1 ; i++) {
       model.add(IloNullIntersect(env, crews[i], crews[i+1]));
       if (i < nCrews-2)
         model.add(IloNullIntersect(env, crews[i], crews[i+2]));
     }


     IloSolver solver(model);
     if (solver.solve(IloGenerate(env, crews))){
       solver.out() << "Solution" << endl;
       for (IloInt j=0 ; j< crews.getSize() ; j++) {
         solver.out() << "Crew #" << (j+1) << ": " ;
         Print(solver, crews[j], Names);
       }
     }
     else
       solver.out() << "No solution" << endl;
     solver.printInformation();

  }
  catch (IloException& ex) {
    cerr << "Error: " << ex << endl;
  }
  env.end();
  return 0;
}
/*
Solution
Crew #1: Bill Bob Carol Cathy
Crew #2: Carolyn David Ed Juliet Marilyn
Crew #3: Fred Heather Inez Janet Jeremy
Crew #4: Bill Bob Carol Cathy Jean Joe
Crew #5: Carolyn David Ed Juliet Marilyn Mario Ron
Crew #6: Fred Inez Janet Jeremy
Crew #7: Bill Bob Carol Cathy Heather
Crew #8: Carolyn David Ed Jean Juliet Marilyn
Crew #9: Fred Inez Janet Jeremy Joe Mario
Crew #10: Bill Bob Carol Cathy Heather Ron Tom
Number of fails               : 44
Number of choice points       : 94
Number of variables           : 152
Number of constraints         : 273
Reversible stack (bytes)      : 24144
Solver heap (bytes)           : 128664
Solver global heap (bytes)    : 4044
And stack (bytes)             : 4044
Or stack (bytes)              : 4044
Search Stack (bytes)          : 4044
Constraint queue (bytes)      : 11152
Total memory used (bytes)     : 180136
Elapsed time since creation   : 0.07
*/
