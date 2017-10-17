// -------------------------------------------------------------- -*- C++ -*-
// File: ./examples/src/addsoft.cpp
// --------------------------------------------------------------------------
// Licensed Materials - Property of IBM
// 5724-Y47
// Copyright IBM Corporation 1990, 2009. All Rights Reserved.
//
// Note to U.S. Government Users Restricted Rights:
// Use, duplication or disclosure restricted by GSA ADP Schedule
// Contract with IBM Corp.
// --------------------------------------------------------------------------

/*
Here is a simple example using soft constraints
*/

#include <ilsolver/ilosolverint.h>
#include <ilsolver/ilosolverany.h>
#include <ilsolver/disjunct.h>

ILOSTLBEGIN

class IlcManageSoftCtI : public IlcConstraintI {
  IlcConstraintArray _cons;
  IlcRevInt _numFails;
  IlcIntVar _obj;
  IlcSoftCtHandler _sh;
public:
  IlcManageSoftCtI(IlcConstraintArray cons, IlcIntVar obj):
    IlcConstraintI(cons.getManager()), _cons(cons), _obj(obj), _sh(cons.getManager(),100){}
  void post();
  void propagate();
  void softCtFailDemon(const IlcInt softCtIndex);
  void softCtVarDemon(const IlcInt softCtIndex);
};

ILCCTDEMON1(mySoftCtVarDemon,IlcManageSoftCtI,softCtVarDemon, IlcInt,i)
ILCCTDEMON1(mySoftCtFailDemon,IlcManageSoftCtI,softCtFailDemon, IlcInt, i)

void IlcManageSoftCtI::softCtFailDemon(const IlcInt softCtIndex){
    // this demon is called when the soft constraint of index softCtIndex is violated
    // the number of fails is incremented, that is
    // the variable _obj, counting the number of violations, is incremented
    // any other action could be done here
  getManager().out() << "constraint: " << _cons[softCtIndex];
  getManager().out() << "is violated" << endl;
  _numFails.setValue(getManager(),_numFails.getValue() +1);
  _obj.setMin(_numFails.getValue());
}

void IlcManageSoftCtI::softCtVarDemon(const IlcInt softCtIndex){
    // this function is called each time a variable involved in the soft constraint is modified
  IlcSoftConstraint softCt=_sh.getSoftConstraint(softCtIndex);
  const IlcInt cvar=softCt.getCopiedVarInProcess();
  getManager().out() << "the copied variable of the variable: ";
  getManager().out() << IlcAnyVar(_sh.getVar(_sh.getVarOfCopiedVar(cvar)).getImpl());
  getManager().out() << " of the constraint: " << _cons[softCtIndex];
  getManager().out() << " has been modified" << endl;
  getManager().out() << "the copied variable is: ";
  getManager().out() << IlcAnyVar(_sh.getCopiedVar(cvar).getImpl()) << endl;
}

void IlcManageSoftCtI::post(){
}

void IlcManageSoftCtI::propagate(){
    // for each constraint in array _cons, a corresponding soft constraint is defined
    // and the demons are linked to the possible modifications
  const IlcInt numCt=_cons.getSize();
  for(IlcInt i=0;i<numCt;i++){
    // creation of the soft constraint
    IlcSoftConstraint softCt=_sh.createSoftConstraint(_cons[i]);
    // addition of demons
    softCt.whenFail(mySoftCtFailDemon(getManager(),this,i));
    softCt.whenDomainReduction(mySoftCtVarDemon(getManager(),this,i));
    // addition of the soft constraint
    add(softCt);
  }
  _sh.getImpl()->printCvars();
  _sh.getImpl()->printSoftCts();
  _sh.getImpl()->printVars();
}

IlcConstraint IlcManageSoftCt(IlcConstraintArray cons,IlcIntVar obj){
  return new (cons.getManager().getHeap()) IlcManageSoftCtI(cons,obj);
}

ILOCPCONSTRAINTWRAPPER2(IloManageSoftCt, solver, IloConstraintArray, _cons, IloIntVar, _obj){
  use(solver, _cons);
  use(solver, _obj);
  return IlcManageSoftCt(solver.getConstraintArray(_cons),
			 solver.getIntVar(_obj));
}

char blue[]="blue";
char white[]="white";
char yellow[]="yellow";

int main(int argc, char** argv) {
  IloEnv env;
  try {
    IloModel model(env);

         // 3 variables are defined
  //  IloIntVarArray vars(env,3,0,1);
    IloAnyArray Colors(env, 2, blue, white);
		IloAnyVar x0(env,Colors), x1(env,Colors), x2(env,Colors);
	  IloAnyVarArray vars(env,3,x0,x1,x2);

    vars[0].setName("x0");
    vars[1].setName("x1");
    vars[2].setName("x2");

         // 3 constraints are defined
    IloConstraintArray cons(env,3);
    cons[0] = (vars[0]!=vars[1]);
    cons[1] = (vars[0]!=vars[2]);
    cons[2] = (vars[1]!=vars[2]);

        // an objective is defined
    IloIntVar obj(env,0,3);

        // in order to define these constraints as soft constraints a new constraint is defined
        // this constraint creates a soft constraint for each constraint of cons array
        // on the other hand, this constraint ensures that the objective corresponds to the number of
        // constraints that are violated
    model.add(IloManageSoftCt(env, cons, obj));

        // we search for a solution minimizing the number of violations
    model.add(IloMinimize(env, obj));

    IloSolver solver(model);
    solver.startNewSearch(IloGenerate(env,vars));
    while(solver.next()){
      solver.out() << "obj = " << solver.getIntVar(obj)
		   << endl << solver.getAnyVarArray(vars) << endl << endl;
    }
    solver.endSearch();
 }
 catch (IloException& ex) {
   cout << "Error: " << ex << endl;
 }
  env.end();
  return 0;
}







