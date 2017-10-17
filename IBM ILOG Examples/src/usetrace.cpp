// -------------------------------------------------------------- -*- C++ -*-
// File: ./examples/src/usetrace.cpp
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

We Problem description
-------------------

This article presents different uses of the trace mechanism in ILOG Solver.


------------------------------------------------------------ */
#include <ilsolver/ilosolver.h>
#include <ilsolver/ilctrace.h>

ILOSTLBEGIN

//$doc:trace-all2
ILOCPTRACEWRAPPER0(PrintAllVars, solver) {
  solver.setTraceMode(IlcTrue);
  IlcPrintTrace trace(solver, IlcTraceVars);
  solver.setTrace(trace);
}

int TraceAllVariables(){
  IloEnv env;
  try {
    IloModel mdl(env);
    IloNumVar S(env, 0, 9, ILOINT, "S"),
      E(env, 0, 9, ILOINT, "E"),
      N(env, 0, 9, ILOINT, "N"),
      D(env, 0, 9, ILOINT, "D"),
      M(env, 0, 9, ILOINT, "M"),
      O(env, 0, 9, ILOINT, "O"),
      R(env, 0, 9, ILOINT, "R"),
      Y(env, 0, 9, ILOINT, "Y");
    IloIntVarArray vars (env, 8, S, E, N, D, M, O, R, Y);

    mdl.add(S != 0);
    mdl.add(M != 0);

    IloConstraint alldiff=IloAllDiff(env,vars);
    mdl.add(alldiff);
    mdl.add(  1000*S + 100*E + 10*N + D
      + 1000*M + 100*O + 10*R + E
      == 10000*M + 1000*O + 100*N + 10*E + Y);

    IloSolver solver(mdl);
    solver.addTrace(PrintAllVars(env));
    solver.solve(IloGenerate(env, vars));
  }
  catch (IloException ex) {
    cerr << "Error: " << ex << endl;
  }
  env.end();
  return 0;
}
//end:trace-all2

/*
//$doc:trace-output2

>> begin-set-min: S[0..9] 1
>> end-set-min: S[1..9] 1
>> begin-set-min: M[0..9] 1
>> end-set-min: M[1..9] 1
>> begin-set-min: S[1..9] 9
>> end-set-min: S[9] 9
>> begin-set-max: M[1..9] 1
>> end-set-max: M[1] 1
>> begin-set-max: O[0..9] 1
>> end-set-max: O[0..1] 1
>> begin-set-max: E[0..9] 8
>> end-set-max: E[0..8] 8
>> begin-set-max: N[0..9] 8
>> end-set-max: N[0..8] 8
>> begin-set-max: D[0..9] 8
>> end-set-max: D[0..8] 8
>> begin-set-max: R[0..9] 8
>> end-set-max: R[0..8] 8
>> begin-set-max: Y[0..9] 8
>> end-set-max: Y[0..8] 8
>> begin-set-max: O[0..1] 0
>> end-set-max: O[0] 0
>> begin-remove-value: E[0..8] 1
>> end-remove-value: E[0 2..8] 1
>> begin-remove-value: N[0..8] 1
>> end-remove-value: N[0 2..8] 1
>> begin-remove-value: D[0..8] 1
>> end-remove-value: D[0 2..8] 1
>> begin-remove-value: R[0..8] 1
>> end-remove-value: R[0 2..8] 1
>> begin-remove-value: Y[0..8] 1
>> end-remove-value: Y[0 2..8] 1
>> begin-set-min: N[0 2..8] 2
>> end-set-min: N[2..8] 2
>> begin-set-min: D[0 2..8] 2
>> end-set-min: D[2..8] 2
>> begin-set-min: E[0 2..8] 2
>> end-set-min: E[2..8] 2
>> begin-set-min: R[0 2..8] 2
>> end-set-min: R[2..8] 2
>> begin-set-min: Y[0 2..8] 2
>> end-set-min: Y[2..8] 2
>> begin-set-min: N[2..8] 3
>> end-set-min: N[3..8] 3
>> begin-set-max: E[2..8] 7
>> end-set-max: E[2..7] 7
>> begin-set-min: E[2..7] 3
>> end-set-min: E[3..7] 3
>> begin-set-min: N[3..8] 4
>> end-set-min: N[4..8] 4
>> begin-set-min: E[3..7] 4
>> end-set-min: E[4..7] 4
>> begin-set-min: N[4..8] 5
>> end-set-min: N[5..8] 5
IlcOr   : 1
>> begin-set-value: E[4..7] 4
>> end-set-value: E[4] 4
>> begin-remove-value: D[2..8] 4
>> end-remove-value: D[2..3 5..8] 4
>> begin-remove-value: R[2..8] 4
>> end-remove-value: R[2..3 5..8] 4
>> begin-remove-value: Y[2..8] 4
>> end-remove-value: Y[2..3 5..8] 4
>> begin-set-min: D[2..3 5..8] 8
>> end-set-min: D[8] 8
>> begin-set-min: R[2..3 5..8] 8
>> end-set-min: R[8] 8
>> begin-set-max: N[5..8] 5
>> end-set-max: N[5] 5
>> begin-set-max: Y[2..3 5..8] 2
>> end-set-max: Y[2] 2
>> begin-set-min: E[4..7] 5
>> end-set-min: E[5..7] 5
>> begin-set-min: N[5..8] 6
>> end-set-min: N[6..8] 6
IlcOr   : 2
>> begin-set-value: E[5..7] 5
>> end-set-value: E[5] 5
>> begin-remove-value: D[2..8] 5
>> end-remove-value: D[2..4 6..8] 5
>> begin-remove-value: R[2..8] 5
>> end-remove-value: R[2..4 6..8] 5
>> begin-remove-value: Y[2..8] 5
>> end-remove-value: Y[2..4 6..8] 5
>> begin-set-min: D[2..4 6..8] 7
>> end-set-min: D[7..8] 7
>> begin-set-min: R[2..4 6..8] 8
>> end-set-min: R[8] 8
>> begin-set-max: N[6..8] 6
>> end-set-max: N[6] 6
>> begin-set-max: Y[2..4 6..8] 3
>> end-set-max: Y[2..3] 3
>> begin-set-max: D[7..8] 7
>> end-set-max: D[7] 7
>> begin-set-max: Y[2..3] 2
>> end-set-max: Y[2] 2
//end:trace-output2
*/


//$doc:trace-all3

ILOCPTRACEWRAPPER1(TraceVarArray, solver, IloIntVarArray, vars) {
  IlcIntVarArray svars=solver.getIntVarArray(vars);
  solver.setTraceMode(IlcTrue);
  IlcPrintTrace trace(solver, IlcTraceVars);
  trace.trace(svars[1]);
}


int TraceOneVariable(){
  IloEnv env;
  try {
    IloModel mdl(env);
    IloIntVar S(env, 0, 9, "S"),
      E(env, 0, 9, "E"),
      N(env, 0, 9, "N"),
      D(env, 0, 9, "D"),
      M(env, 0, 9, "M"),
      O(env, 0, 9, "O"),
      R(env, 0, 9, "R"),
      Y(env, 0, 9, "Y");
    IloIntVarArray vars (env, 8, S, E, N, D, M, O, R, Y);

    mdl.add(S != 0);
    mdl.add(M != 0);

    IloConstraint alldiff=IloAllDiff(env,vars);
    mdl.add(alldiff);
    mdl.add(  1000*S + 100*E + 10*N + D
      + 1000*M + 100*O + 10*R + E
      == 10000*M + 1000*O + 100*N + 10*E + Y);

    IloSolver solver(mdl);
    solver.addTrace(TraceVarArray(env, vars));
    solver.solve(IloGenerate(env, vars));
  }
  catch (IloException ex) {
    cerr << "Error: " << ex << endl;
  }
  env.end();
  return 0;
}

//end:trace-all3


/*
//$doc:trace-output3
>> begin-set-max: E[0..9] 8
>> end-set-max: E[0..8] 8
>> begin-remove-value: E[0..8] 1
>> end-remove-value: E[0 2..8] 1
>> begin-set-min: E[0 2..8] 2
>> end-set-min: E[2..8] 2
>> begin-set-max: E[2..8] 7
>> end-set-max: E[2..7] 7
>> begin-set-min: E[2..7] 3
>> end-set-min: E[3..7] 3
>> begin-set-min: E[3..7] 4
>> end-set-min: E[4..7] 4
IlcOr   : 1
>> begin-set-value: E[4..7] 4
>> end-set-value: E[4] 4
IlcFail : 1
>> begin-set-min: E[4..7] 5
>> end-set-min: E[5..7] 5
IlcOr   : 2
>> begin-set-value: E[5..7] 5
>> end-set-value: E[5] 5
//end:trace-output3
*/



//$doc:trace-all4
ILOCPTRACEWRAPPER0(PrintAllTrace, solver) {
  solver.setTraceMode(IlcTrue);
  IlcPrintTrace trace(solver, IlcTraceAllEvents);
  solver.setTrace(trace);
}

int TraceAllEvents(){
  IloEnv env;
  try {
    IloModel mdl(env);
    IloIntVar S(env, 0, 9, "S"),
      E(env, 0, 9, "E"),
      N(env, 0, 9, "N"),
      D(env, 0, 9, "D"),
      M(env, 0, 9, "M"),
      O(env, 0, 9, "O"),
      R(env, 0, 9, "R"),
      Y(env, 0, 9, "Y");
    IloIntVarArray vars (env, 8, S, E, N, D, M, O, R, Y);

    mdl.add(S != 0);
    mdl.add(M != 0);

    IloConstraint alldiff=IloAllDiff(env,vars);
    mdl.add(alldiff);
    mdl.add(  1000*S + 100*E + 10*N + D
      + 1000*M + 100*O + 10*R + E
      == 10000*M + 1000*O + 100*N + 10*E + Y);

    IloSolver solver(mdl);
    solver.addTrace(PrintAllTrace(env));
    solver.solve(IloGenerate(env, vars));
  }
  catch (IloException ex) {
    cerr << "Error: " << ex << endl;
  }
  env.end();
  return 0;
}

//end:trace-all4

/*
//$doc:trace-output4
>> begin-constraint-propagate ((S[0..9] != 0))
>> begin-set-min: S[0..9] 1
>> end-set-min: S[1..9] 1
>> end-constraint-propagate ((S[1..9] != 0))
>> begin-constraint-propagate ((M[0..9] != 0))
>> begin-set-min: M[0..9] 1
>> end-set-min: M[1..9] 1
>> end-constraint-propagate ((M[1..9] != 0))
>> begin-constraint-post (IlcAllDiff(00ACED48) {S[1..9], E[0..9], N[0..9],
D[0..
9], M[1..9], O[0..9], R[0..9], Y[0..9], })
>> end-constraint-post (IlcAllDiff(00ACED48) {S[1..9], E[0..9], N[0..9],
D[0..9]
, M[1..9], O[0..9], R[0..9], Y[0..9], })
>> begin-constraint-propagate (IlcAllDiff(00ACED48) {S[1..9], E[0..9],
N[0..9],
D[0..9], M[1..9], O[0..9], R[0..9], Y[0..9], })
>> end-constraint-propagate (IlcAllDiff(00ACED48) {S[1..9], E[0..9],
N[0..9], D[
0..9], M[1..9], O[0..9], R[0..9], Y[0..9], })
>> begin-constraint-post ((IlcArraySumI(00ACF070)[1000..9099] ==
(IlcArraySumI(0
0ACEF38)[9000..89919] - 91 * E[0..9])))
>> end-constraint-post ((IlcArraySumI(00ACF070)[1000..9099] ==
(IlcArraySumI(00A
CEF38)[9000..89919] - 91 * E[0..9])))
>> begin-constraint-propagate ((IlcArraySumI(00ACF070)[1000..9099] ==
(IlcArrayS
umI(00ACEF38)[9000..89919] - 91 * E[0..9])))
>> begin-set-min: S[1..9] 9
>> end-set-min: S[9] 9
>> begin-set-max: M[1..9] 1
>> end-set-max: M[1] 1
>> begin-set-max: O[0..9] 1
>> end-set-max: O[0..1] 1
>> end-constraint-propagate ((IlcArraySumI(00ACF070)[9000..9099] ==
(IlcArraySum
I(00ACEF38)[9000..10719] - 91 * E[0..9])))
>> begin-var-process: S[9]
>> begin-demon-process (IlcAllDiffDemon)
>> begin-set-max: E[0..9] 8
>> end-set-max: E[0..8] 8
>> begin-set-max: N[0..9] 8
>> end-set-max: N[0..8] 8
>> begin-set-max: D[0..9] 8
>> end-set-max: D[0..8] 8
>> begin-set-max: R[0..9] 8
>> end-set-max: R[0..8] 8
>> begin-set-max: Y[0..9] 8
>> end-set-max: Y[0..8] 8
>> end-demon-process (IlcAllDiffDemon)
>> begin-demon-process ((IlcArraySumI(00ACF070)[9000..9088] ==
(IlcArraySumI(00
ACEF38)[9000..10628] - 91 * E[0..8])))
>> begin-set-max: O[0..1] 0
>> end-set-max: O[0] 0
>> end-demon-process ((IlcArraySumI(00ACF070)[9000..9088] ==
...
//end:trace-output4
*/


//$doc:trace-all5
class MyTraceI : public IlcTraceI {
public:
  MyTraceI(IloSolver solver, IlcUInt flags, const char* name=0) :
      IlcTraceI(solver.getManager().getImpl(),flags,name){}
      ~MyTraceI(){}

      // virtual functions
      void beginSetMinIntVar(const IlcIntExp var, IlcInt min){
        var.getSolver().out() << "mySetMinIntVar";
        print(var,min);
      }
      void beginSetValueIntVar(const IlcIntExp var, IlcInt val){
        var.getSolver().out() << "mySetValue";
        print(var,val);
      }

      void print(const IlcIntExp var, IlcInt val){
        var.getSolver().out()<< var << " val:" << val << endl;
        IlcGoalI* goal=var.getSolver().getActiveGoal().getImpl();
        IlcDemonI* demon=getActiveDemon().getImpl();
        if (goal == 0) var.getSolver().out() << "no active goal" << endl;
        else var.getSolver().out() << "goal:" << *goal << endl;
        if (demon == 0) var.getSolver().out() << "no active demon" << endl;
        else {
          if (demon->isAConstraint()){
            var.getSolver().out() << "the active demon is the constraint:" << *demon << endl;
          } else {
            var.getSolver().out() << "propagation of demon" << endl;
            if (demon->getConstraintI() == 0){
              var.getSolver().out() << "the demon has no constraint associated with" << endl;
            } else {
              var.getSolver().out() << "the associated constraint with the demon is:";
              var.getSolver().out() << *(demon->getConstraintI()) << endl;
            }
          }
        }
      }
};

class MyTrace : public IlcTrace {
public:
  MyTrace(IloSolver solver, IlcUInt flags=4, const char* name=0)
    : IlcTrace() {
    _impl = new (solver.getHeap()) MyTraceI(solver, flags, name);
  }
  MyTrace(MyTraceI* impl) {
    _impl = impl;
  }
  ~MyTrace() { }
  MyTraceI* getImpl() const {
    return (MyTraceI*)_impl;
  }
  void trace(IlcIntVar var) const {
    getImpl()->trace(var);
  }
};

//end:trace-all5

/*
//$doc:trace-output5
mySetMinIntVarE[0 2..8] val:2
no active goal
propagation of demon
the associated constraint with the demon is:IlcAllDiff(00ACED48) {S[9], M[1], N[2..8], D[2..8], E[0 2..8], O[0], R[0 2..8], Y[0 2..8], }
mySetMinIntVarE[2..7] val:3
no active goal
the active demon is the constraint:(IlcArraySumI(00ACF070)[9022..9088] == (IlcArraySumI(00ACEF38)[9272..9728] - 91 * E[2..7]))
mySetMinIntVarE[3..7] val:4
no active goal
the active demon is the constraint:(IlcArraySumI(00ACF070)[9022..9088] ==
(IlcArraySumI(00ACEF38)[9362..9728] - 91 * E[3..7]))
IlcOr   : 1
mySetValueE[4..7] val:4
goal:IlcIntVarSetValue
no active demon
IlcFail : 1
mySetMinIntVarE[4..7] val:5
goal:IlcIntVarRemoveValue
no active demon
IlcOr   : 2
mySetValueE[5..7] val:5
goal:IlcIntVarSetValue
no active demon
//end:trace-output5



*/

int main() {
  TraceAllVariables();
  TraceOneVariable();
  TraceAllEvents();
  return 0;
}


