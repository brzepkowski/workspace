// -------------------------------------------------------------- -*- C++ -*-
// File: ./examples/src/carseq.cpp
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

Car sequencing problems arise on assembly lines in factories
in the automotive industry.

There, an assembly line makes it possible to build many different
types of cars, where the types correspond to a basic model with added
options.  In that context, one type of vehicle can be seen as a particular
configuration of options.  Even without loss of generality, we
can assume that it is possible to put multiple options on the same
vehicle while it is on the line.  In that way, virtually any
configuration (taken as an isolated case) could be produced
on the assembly line.  In contrast, for practical reasons (such
as the amount of time needed to do so), a given option really
cannot be installed on every vehicle on the line.  This constraint
is defined by what we call the ``capacity'' of an option.
The capacity of an option is usually represented as a ratio
p/q where for any sequence of q cars on the line, at most p of
them will have that option.

The problem in car sequencing then consists of determining in which
order cars corresponding to each configuration should be assembled,
while keeping in mind that we must build a certain number of cars per
configuration.

In this example, we'll consider the following version of the problem:
  10 cars to build;
  5 options available for installation;
  6 configurations required.


The following chart indicates which options belong to which
configuration: 1 indicates that configuration j requires option i;
0 means not so.  The chart also shows the capacity of each option
as well as the number of cars to build for each configuration.

option capacity       configurations
                   0  1  2  3  4  5

  0      1/2       1  0  0  0  1  1
  1      2/3       0  0  1  1  0  1
  2      1/3       1  0  0  0  1  0
  3      2/5       1  1  0  1  0  0
  4      1/5       0  0  1  0  0  0

 number of cars    1  1  2  2  2  2

For example, the chart indicates that option 1 can be put on
at most two cars for any sequence of three cars.  Option 1
is required by configurations 2, 3, and 5.

*/
#include <ilsolver/ilosolverint.h>

ILOSTLBEGIN

//
// First Model
//


IloModel IloCarSequencing( IloEnv env,
                           IloInt maxCar,
                           IloIntArray maxConfs,
                           IloIntArray confs,
                           IloIntVarArray sequence){


  const IloInt abstractValue=-1; // we assume that confs[i] != -1
  const IloInt confs_size=confs.getSize();
  const IloInt sequence_size = sequence.getSize();
  IloIntArray nval(env, confs_size+1);
  IloIntVarArray ncard(env, confs_size+1);

  nval[OL]=abstractValue;
  ncard[OL]=IloIntVar(env, sequence_size - maxCar, sequence_size);

  IloInt i;
  for(i=0;i<confs_size;i++){
    nval[i+1]=confs[i];
    if (maxConfs[i] > maxCar)
      ncard[i+1]=IloIntVar(env, 0, maxCar);
    else
      ncard[i+1]=IloIntVar(env, 0, maxConfs[i]);
  }

  IloIntVarArray nvars(env,sequence_size);
  for (i = 0; i < sequence_size ; i++)
    nvars[i] = IloIntVar(env, nval);


  IloModel carseq_model(env);
  carseq_model.add(IloAbstraction(env, nvars, sequence, confs,abstractValue));
  carseq_model.add(IloDistribute(env, ncard, nval, nvars));
  return carseq_model;
}


void firstModel(){
  IloEnv env;
  try {
    IloModel model(env);

    const IloInt nbOptions = 5;
    const IloInt nbConfs   = 6;
    const IloInt nbCars    = 10;

    IloIntVarArray cars(env, nbCars,0,nbConfs-1);

    IloIntArray confs(env, 6, 0, 1, 2, 3, 4, 5);
    IloIntArray nbRequired(env, 6, 1, 1, 2, 2, 2, 2);

    IloIntVarArray cards(env, 6);
    for(IloInt conf=0;conf<nbConfs;conf++) {
      cards[conf]=IloIntVar(env, nbRequired[conf],nbRequired[conf]);
    }

    model.add (IloDistribute(env, cards, confs, cars));

    IloArray<IloIntArray> optConf(env, nbOptions);

    optConf[0] = IloIntArray(env, 3, 0, 4, 5);
    optConf[1] = IloIntArray(env, 3, 2, 3, 5);
    optConf[2] = IloIntArray(env, 2, 0, 4);
    optConf[3] = IloIntArray(env, 3, 0, 1, 3);
    optConf[4] = IloIntArray(env, 1, 2);

    IloIntArray maxSeq(env, 5, 1, 2, 1, 2, 1);
    IloIntArray overSeq(env, 5, 2, 3, 3, 5, 5);

    IloArray<IloIntArray> optCard(env, nbOptions);
    optCard[0] = IloIntArray(env, 3, 1, 2, 2);
    optCard[1] = IloIntArray(env, 3, 2, 2, 2);
    optCard[2] = IloIntArray(env, 2, 1, 2);
    optCard[3] = IloIntArray(env, 3, 1, 1, 2);
    optCard[4] = IloIntArray(env, 1, 2);

    for (IloInt opt=0; opt < nbOptions; opt++) {
      for (IloInt i=0; i < nbCars-overSeq[opt]+1; i++) {
        IloIntVarArray sequence(env,(IloInt)overSeq[opt]);
        for (IloInt j=0; j < overSeq[opt]; j++)
          sequence[j] = cars[i+j];
        model.add(IloCarSequencing(env,
                                   (IloInt)maxSeq[opt],
                                   optCard[opt],
                                   optConf[opt],
                                   sequence));
      }
    }


    IloSolver solver(model);

    solver.solve(IloGenerate(env,cars,IlcChooseMinSizeInt));
    solver.out() << "cars = " << solver.getIntVarArray(cars) << endl;

    solver.printInformation();
  }
  catch (IloException& ex) {
    cout << "Error: " << ex << endl;
  }
  env.end();
}

// Second Model
//


IloArray<IloIntArray> readData(IloEnv env,
                               IloInt example,
                               IloInt& nbCars,
                               IloInt& nbOpt,
                               IloIntArray& nbMax,
                               IloIntArray& seqWidth,
                               IloIntArray& confs,
                               IloIntArray& nbCarsByConf){
  char globalName[200];
  switch(example){
  case 1:{
    strcpy(globalName,"../../../examples/data/carseq1.dat");
  } break;
  case 2:{
    strcpy(globalName,"../../../examples/data/carseq2.dat");
  } break;
  }
  IloInt nbConfs;
  ifstream fin(globalName,ios::in);
  if (!fin) env.out() << "problem with file:" << globalName << endl;
  fin >> nbCars >> nbOpt >> nbConfs;

  IloInt i;
  confs = IloIntArray(env, nbConfs); // required configurations
  for(i=0;i<nbConfs;i++){
    confs[i]=i;
  }
  IloArray<IloIntArray> confsByOption(env, nbConfs);
  for (i=0;i<nbConfs;i++){
    confsByOption[i] = IloIntArray(env, nbOpt);
  }
  IloIntArray nbConfsByOption(env, nbOpt);
  for (i=0;i<nbOpt;i++){
    nbConfsByOption[i]=0;
  }

  // read the maximum number of cars of each sequence that can take the
  // invoked option
  nbMax=IloIntArray(env, nbOpt);
  for(i=0;i<nbOpt;i++){
    fin >> nbMax[i];
  }

  // read the size of a sequence for each option
  seqWidth=IloIntArray(env, nbOpt);
  for(i=0;i<nbOpt;i++){
    fin >> seqWidth[i];
  }

  // read the options required by each configuration
  nbCarsByConf=IloIntArray(env, nbConfs);
  IloInt dummy;
  IloInt j;
  for(i=0;i<nbConfs;i++){
    fin >> dummy;
    fin >> nbCarsByConf[i];
    for (j=0;j<nbOpt;j++){
      fin >> confsByOption[i][j];
      if (confsByOption[i][j] == 1){
        nbConfsByOption[j]++;
      }
    }
  }
  // compute the configurations required by each option
  IloArray<IloIntArray> optConf(env, nbOpt);
  for(i=0;i<nbOpt;i++){
    optConf[i]=IloIntArray(env, (IloInt)nbConfsByOption[i]);
  }
  IloIntArray ind(env, nbOpt);
  for(i=0;i<nbOpt;i++){
    ind[i]=0;
  }
  for(i=0;i<nbConfs;i++){
    for (j=0;j<nbOpt;j++){
      if (confsByOption[i][j] == 1){
        optConf[j][(IloInt)ind[j]]=i;
        ind[j]++;
      }
    }
  }
  // print the configuration required by each option
  env.out() << "number of times each configuration is required:" << endl;
  for(i=0;i<nbConfs;i++){
    env.out() << nbCarsByConf[i] << " ";
  }
  env.out() << endl;
  env.out() << "configuration required by each option:" << endl;
  for(i=0;i<nbOpt;i++){
    env.out() << "option:" << i << " ";
    for (j=0;j<optConf[i].getSize();j++){
      env.out() << optConf[i][j] << " ";
    }
    env.out() << endl;
  }
  env.out() << "number of times each option is required" << endl;
  for(i=0;i<nbOpt;i++){
    IloInt cpt=0;
    for(j=0;j<optConf[i].getSize();j++){
      cpt += nbCarsByConf[(IloInt)optConf[i][j]];
    }
    env.out() << "option:" << i << " " << cpt << " x ";
    env.out() << nbMax[i] << "/" << seqWidth[i] << endl;
  }

  return optConf;
}

ILCGOAL1(IlcGenerateAbstractVars, IlcIntVarArray, vars) {
  IlcInt index = IlcChooseFirstUnboundInt(vars);
  if (index == -1) return 0;
  return IlcAnd(IlcDichotomize(vars[index]), this); // first tries vars[index] == 1
}

ILOCPGOALWRAPPER1(IloGenerateAbstractVars, solver, IloIntVarArray, vars) {
  return IlcGenerateAbstractVars(solver, solver.getIntVarArray(vars));
}

void secondModel(IloInt mode, IloInt example)
{
  IloEnv env;
  try {
    IloModel model(env);

    IloInt nbOptions;
    IloInt nbCars;
    IloIntArray confs(env); // array of required configurations
    IloIntArray nbCarsByConf(env); // number of cars to assign
                                    // to each configuration
    IloIntArray nbMax(env);
    IloIntArray seqWidth(env);

    IloArray<IloIntArray> optConf = readData(env,
                                             example,
                                             nbCars,
                                             nbOptions,
                                             nbMax,
                                             seqWidth,
                                             confs,
                                             nbCarsByConf);


    cout << "Pass 1 " << endl;
    const IlcInt nbConfs   = confs.getSize();

    IloInt i,j;
    IloIntVarArray cars(env,nbCars,0,nbConfs-1);
    IloIntVarArray cards(env,nbConfs);
    for (i = 0; i < nbConfs; i++){
      cards[i] = IloIntVar(env,nbCarsByConf[i],nbCarsByConf[i]);
    }
    cout << "Pass 2 " << endl;

    model.add(IloDistribute(env, cards, confs, cars));

    IloInt opt;
    for (opt = 0; opt < nbOptions; opt++) {
      IloIntVarArray ncard(env,optConf[opt].getSize());
      for(i = 0; i < optConf[opt].getSize(); i++){
        ncard[i] = cards[(IloInt)optConf[opt][i]];
      }
      model.add(IloSequence(env,
                            0,
                            (IloInt)nbMax[opt],
                            (IloInt)seqWidth[opt],
                            cars,
                            optConf[opt],
                            ncard));
    }
    cout << "Pass 3 " << endl;
    IloIntArray order(env, nbOptions);
    switch(example){
    case 1:{
      order[OL]=0;
      order[1]=1;
      order[2]=4;
      order[3]=2;
      order[4]=3;
    } break;
    case 2:{
      order[OL]=0;
      order[1]=4;
      order[2]=1;
      order[3]=2;
      order[4]=3;
    }break;
    }

    IloBoolVarArray bvars(env,nbOptions*nbCars);
    for(i=0;i < nbOptions;i++){
      opt=(IloInt)order[i];
      IloBoolVarArray bv(env,nbCars);
      for (j = 0; j < nbCars; j++)
        bv[j] = IloBoolVar(env);
      model.add(IloBoolAbstraction(env, bv, cars, optConf[opt]));
      for(j=0;j<nbCars;j++){
        bvars[i*nbCars + j] = bv[j];
      }
    }
    cout << "Pass 4 " << endl;

    IloBoolVarArray tbvars(env,nbOptions*nbCars);
    for(j=0;j<nbOptions*nbCars;j+=nbCars){
      IlcInt mid=nbCars/2 - 1;
      for(i=0;i<mid+1;i++){
        tbvars[2*i+j]=bvars[mid-i+j];
        tbvars[2*i+1+j]=bvars[mid+i+1+j];
      }
    }

    IloSolver solver(model);
    if (mode) {
      solver.setDefaultFilterLevel(IlcSequenceCt,IlcExtended);
      solver.setDefaultFilterLevel(IlcDistributeCt,IlcExtended);
    }
    if (solver.solve(IloGenerateAbstractVars(env,tbvars)))
      solver.out() << "cars = " << solver.getIntVarArray(cars) << endl;
    else
    solver.out() << "No solution" << endl;
    solver.printInformation();
  }
  catch (IloException& ex) {
    cout << "Error: " << ex << endl;
  }
  env.end();
}


int main(int argc, char** argv){
  IloInt pb=(argc>1)?atoi(argv[1]):1;
  if (pb != 1 && pb != 2)
    pb=1;

  switch(pb){
  case 1 :{
    firstModel();
  } break;
  case 2 :{
    IlcInt mode = (argc>2)?atoi(argv[2]):1;
    if (mode != 0 && mode != 1)
      mode=1;
    IlcInt example = (argc>3)?atoi(argv[3]):1;
    secondModel(mode,example);
  } break;

  }

  return 0;
}

/*
carseq 1:
cars = IlcIntVarArrayI[[0] [1] [5] [2] [4] [3] [3] [4] [2] [5]]
Number of fails               : 1
Number of choice points       : 3
Number of variables           : 308
Number of constraints         : 239
Reversible stack (bytes)      : 40224
Solver heap (bytes)           : 168864
Solver global heap (bytes)    : 4044
And stack (bytes)             : 4044
Or stack (bytes)              : 4044
Search Stack (bytes)          : 4044
Constraint queue (bytes)      : 11144
Total memory used (bytes)     : 236408
Elapsed time since creation   : 0.11

carseq 2 1 1:
number of times each configuration is required:
5 3 7 1 10 2 11 5 4 6 12 1 1 5 9 5 12 1
configuration required by each option:
option:0 0 1 2 4 5 6 7 13
option:1 0 1 2 3 4 8 9 10 14
option:2 2 3 7 10 11 12 17
option:3 1 3 6 9 12 16
option:4 0 5 8 11 15
number of times each option is required
option:0 48 x 1/2
option:1 57 x 2/3
option:2 28 x 1/3
option:3 34 x 2/5
option:4 17 x 1/5
cars = IlcIntVarArrayI[[17] [16] [16] [7] [15] [13] [10] [13] [14] [6] [10] [6]
[15] [4] [14] [6] [10] [4] [16] [0] [14] [6] [10] [1] [15] [2] [14] [6] [10] [0]
 [16] [4] [9] [7] [8] [4] [16] [2] [9] [5] [10] [4] [16] [2] [8] [6] [14] [2] [1
6] [0] [3] [13] [14] [2] [15] [1] [10] [6] [14] [0] [12] [4] [9] [7] [8] [4] [16
] [2] [9] [5] [10] [4] [16] [2] [8] [6] [10] [4] [16] [0] [10] [6] [14] [1] [11]
 [4] [9] [7] [9] [13] [10] [6] [14] [6] [10] [13] [15] [7] [16] [16]]
Number of fails               : 0
Number of choice points       : 160
Number of variables           : 4768
Number of constraints         : 2367
Reversible stack (bytes)      : 836184
Solver heap (bytes)           : 1977864
Solver global heap (bytes)    : 4044
And stack (bytes)             : 4044
Or stack (bytes)              : 8064
Search Stack (bytes)          : 4044
Constraint queue (bytes)      : 15152
Total memory used (bytes)     : 2849396
Elapsed time since creation   : 4.677

carseq 2 1 2
number of times each configuration is required:
7 11 1 3 15 2 8 5 3 4 5 2 6 2 2 4 3 5 2 4 1 1 1 1 2
configuration required by each option:
option:0 0 1 3 5 10 11 15 16 17 18 19 20 21
option:1 1 2 4 6 9 11 12 14 15 17 18 19 21 22 23
option:2 2 3 5 7 9 11 12 13 15 18 23
option:3 0 2 5 6 8 9 15 17 20 21 22
option:4 2 11 13 14 16 19 20 21 22 23 24
number of times each option is required
option:0 50 x 1/2
option:1 67 x 2/3
option:2 32 x 1/3
option:3 37 x 2/5
option:4 20 x 1/5
No solution
Number of fails               : 41
Number of choice points       : 40
Number of variables           : 626
Number of constraints         : 0
Reversible stack (bytes)      : 872364
Solver heap (bytes)           : 2126604
Solver global heap (bytes)    : 4044
And stack (bytes)             : 4044
Or stack (bytes)              : 4044
Search Stack (bytes)          : 4044
Constraint queue (bytes)      : 21160
Total memory used (bytes)     : 3036304
Elapsed time since creation   : 5.458
*/





