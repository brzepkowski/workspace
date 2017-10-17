// -------------------------------------------------------------- -*- C++ -*-
// File: ./examples/src/binpacking.cpp
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

In this problem you have components of different types and bins of various
types. There are various constraints on which type of component can go into
which type of bin. You must find the minimum total number of bins required
to contain the components.

In this example there are five types of components: glass, plastic, steel,
wood, copper. There are three types of bins: red, blue, green.

Each type of bin has a different capacity:
red has capacity 3
blue has capacity 1
green has capacity 4

Each type of bin can only contain certain types of components:
red can contain glass, wood, copper
blue can contain glass, steel, copper
green can contain plastic, wood, copper

There are also some special requirements on the mixing of component types:
wood requires plastic (a bin that contains wood must also contain plastic,
but a bin that contains plastic does not necessarily also contain wood)
glass excludes copper (a bin that contains glass cannot also contain copper)
copper excludes plastic (a bin that contains copper cannot also contain plastic)

There are some limits on the amount of each type of component in
certain types of bins:
red can contain at most 1 component of type wood
green can contain at most 2 components of type wood

There is the following demand for each type of component:
2 components of type glass
4 components of type plastic
3 components of type steel
6 components of type wood
4 components of type copper

Find the minimum total number of bins required to contain the components.

------------------------------------------------------------ */

/* ------------------------------------------------------------

Solution for this example.

Create a class bin with constrained integer variables:
- the type (among red, blue, green)
- the capacity
- the number of each component in the bin
  (an array of integer variables)

The "algorithm" is this: take N bins, attempt to allocate the
objects into them and enumerate the booleans (to figure out what
types of bins they are) and the integers (to figure out how many
are in each bin).  If you cannot do that, add one more bin and
enumerate again.

------------------------------------------------------------ */
#include <ilsolver/ilosolverint.h>
ILOSTLBEGIN
enum Component {glass, plastic, steel, wood, copper};
const char* Components[5] = {"Glass", "Plastic", "Steel", "Wood", "Copper"};

enum Type {red, blue, green};
const char* Types[3] = {"Red", "Blue", "Green"};
class Bin {
public:
  IloIntVar       _type;
  IloIntVar       _capacity;
  IloIntVarArray  _contents;
  Bin (IloModel    mod,
       IloIntArray Capacity,
       IloInt      nTypes,
       IloInt      nComponents);
  void display(const IloSolver sol);
};
Bin::Bin (IloModel model,
          IloIntArray Capacity,
          IloInt nTypes,
          IloInt nComponents) {
  IloEnv env = model.getEnv();
  _type = IloIntVar(env, 0, nTypes-1);
  _capacity = IloIntVar(env, 0, IloMax(Capacity));
  _contents = IloIntVarArray(env, nComponents, 0, 4);
  model.add(_capacity == Capacity[_type]);
  model.add(_capacity >= IloSum(_contents));
  model.add(0. < IloSum(_contents));
  model.add(IloIfThen(env, _type == red,
                      ((_contents[plastic] == 0)
                       && (_contents[steel] == 0)
                       && (_contents[wood] <= 1))));
  model.add(IloIfThen(env, _type == blue,
                      ((_contents[plastic] == 0) && (_contents[wood] == 0))));
  model.add(IloIfThen(env, _type == green,
                      ((_contents[glass] == 0)
                       && (_contents[steel] == 0)
                       && (_contents[wood] <= 2))));
  model.add(IloIfThen(env, _contents[wood] != 0, _contents[plastic] != 0));
  model.add(((_contents[glass] == 0)  || (_contents[copper]  == 0)));
  model.add(((_contents[copper] == 0) || (_contents[plastic] == 0)));
}
void Bin::display(const IloSolver solver) {
  _type.getEnv().out() << "Type :\t"
                       << Types[(IloInt)solver.getValue(_type)] << endl;
  _type.getEnv().out() << "Capacity:\t"
                       << solver.getValue(_capacity) << endl;
  IloInt nComponents = _contents.getSize();
  for (IloInt i = 0; i < nComponents; i++)
    if (solver.getValue(_contents[i]) > 0)
      _type.getEnv().out() << Components[i] << " :  \t"
                           << solver.getValue(_contents[i]) << endl;
  _type.getEnv().out() << endl;
}
int main(){
  IloEnv env;
  try {
    IloModel model(env);
    IloInt i;
    const IloInt nComponents = 5;
    const IloInt nTypes = 3;
    IloIntArray Demand(env, nComponents, 2, 4, 3, 6, 4);
    IloIntArray Capacity(env, nTypes, 3, 1, 4);
    IloIntArray coef(env, nComponents, 625, 125, 25, 5, 1);
    IloInt totalDemand = (IloInt)IloSum(Demand);
    const IloInt maxBin = (IloInt)totalDemand;
    Bin* prevBin  =0;
    Bin* newBin;
    Bin** theBins = new Bin*[maxBin];
    IloInt nBins = 1;
    IloBool solution = IloFalse;
    IloRange totalCapacityCon(env, totalDemand, IloIntMax);
    model.add(totalCapacityCon);
    IloRangeArray componentsDemandCon(env, Demand, Demand);
    model.add(componentsDemandCon);
    IloSolver solver(model);
    do{
      newBin = new Bin(model, Capacity, nTypes, nComponents);
      theBins[nBins-1] = newBin;
      totalCapacityCon.setLinearCoef(newBin->_capacity, 1.);
      for (i = 0; i < nComponents; i++)
        componentsDemandCon[i].setLinearCoef(newBin->_contents[i], 1.);
      if (prevBin != 0){
        model.add(newBin->_type >= prevBin->_type);
        model.add(IloIfThen(env, prevBin->_type == newBin->_type,
                            IloScalProd(newBin->_contents, coef)
                            >= IloScalProd(prevBin->_contents, coef)  ));
      }
      solver.out() << "Search with " << nBins << endl;
      if (!solver.solve()){
        prevBin = newBin;
        nBins++;
      }
      else
        solution = IloTrue;
    }
    while (!solution && nBins <= maxBin);
    if (solution) {
      for(i = 0; i < nBins; i++){
        theBins[i]->display(solver);
      }
    }
    for  (i = 0; i < nBins; ++i)
      delete theBins[i];
    delete [] theBins;
  }
  catch (IloException& ex) {
    cerr << "Error: " << ex << endl;
  }
  env.end();
  return 0;
}
/*
Search with 1
Search with 2
Search with 3
Search with 4
Search with 5
Search with 6
Search with 7
Search with 8
Type :  Red
Capacity:       3
Glass :         2

Type :  Blue
Capacity:       1
Steel :         1

Type :  Blue
Capacity:       1
Steel :         1

Type :  Blue
Capacity:       1
Steel :         1

Type :  Green
Capacity:       4
Copper :        4

Type :  Green
Capacity:       4
Plastic :       1
Wood :          2

Type :  Green
Capacity:       4
Plastic :       1
Wood :          2

Type :  Green
Capacity:       4
Plastic :       2
Wood :          2
*/
