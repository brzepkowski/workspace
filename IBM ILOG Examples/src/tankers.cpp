// -------------------------------------------------------------- -*- C++ -*-
// File: ./examples/src/tankers.cpp
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

// The problem :
//
//    Given a set of customer orders, the problem is to
//    compute the minimal set of trucks needed to deliver
//    the orders to the customers.
//
//    The company is able to deliver 5 types of products
//    {1, 2, 3, 4, 5}. Each order defines the quantity
//    to deliver for each type of product.
//
//    A truck is composed of a set of tanks. A truck can
//    be configured with at most 5 different tanks. There
//    are 5 types of tanks that correspond to 5 different
//    capacities {1000, 2000, 3000, 4000, 5000}.
//
//    An order is assigned to one and only one truck.
//    The products of the order must be assigned to the
//    tanks of the truck.
//    A product in an order is assigned to one and only
//    one tank. One tank can mix the products of different
//    orders, but all these products must be of the same
//    type.
//
//    A truck cannot be loaded over 12000.
//    A tank cannot be loaded over its capacity.
//
//    A truck cannot contain more than 3 different types
//    of products.
//
//    Products 3 and 4 are incompatible. They cannot be
//    transported in the same truck.

ILOSTLBEGIN


class OrderI;
class ProductI;

typedef OrderI* Order;
typedef ProductI* Product;

//-----------------------------------------------------
// Data
//-----------------------------------------------------

const IloInt NbProducts = 5;
const IloInt NbOrders   = 11;

//Sorted in decreasing order of total quantity
const IloInt Quantities[NbOrders][NbProducts] = {
  { 2000, 3000, 5000,    0,    0 },
  {    0, 3000, 2000,    0, 4000 },
  { 2000,    0,    0,    0, 4000 },
  {    0, 2500,    0,  500, 1000 },
  { 1000,    0,  500,    0, 2000 },
  {    0,  500,    0, 2000,  500 },
  {    0,    0,    0,    0, 3000 },
  {    0, 1000,    0,    0, 1000 },
  { 1000,  500,    0,    0,    0 },
  { 1500,    0,    0,    0,    0 },
  {    0,  500,  500,    0,    0 }
};

Order   Orders[NbOrders];
Product Products[NbOrders*NbProducts];


//-----------------------------------------------------
class ProductI {
protected:
  Order    _order;
  IloInt   _type;
  IloInt   _quantity;
public:
  ProductI(Order order, IloInt type, IloInt quantity)
    :_order(order) ,_type(type), _quantity(quantity) {}

  Order    getOrder() const    { return _order; }
  IloInt   getType() const     { return _type; }
  IloInt   getQuantity() const { return _quantity; }

  void printName(ostream& out) const;
};

IloFunction<IloAny,IloInt> PType(IloEnv env);
IloFunction<IloAny,IloInt> PQuantity(IloEnv env);

//-----------------------------------------------------
class OrderI {
private:
  IloInt       _id;
  IloInt       _quantity;
  IloAnySetVar _products;
public:
  OrderI(IloInt id) : _id(id), _quantity(0) {}

  IloInt       getId()       const { return _id;  }
  IloInt       getQuantity() const { return _quantity; }
  void         setQuantity(IloInt quantity) { _quantity = quantity; }
  IloAnySetVar getProducts() const { return _products; }

  void makeProductVar(IloEnv env, IloAnyArray prods);

  void printName(ostream& out) const;
};

IloFunction<IloAny,IloInt> OQuantity(IloEnv env);
IloFunction<IloAny,IloAnySetVar> OrderProducts(IloEnv env);

//-----------------------------------------------------
void OrderI::makeProductVar(IloEnv env, IloAnyArray prods) {
  _products = IloAnySetVar(env, prods);
  for(IloInt i = 0; i < NbOrders; i++) {
    for(IloInt j = 0; j < NbProducts; j++) {
      Product p = Products[i*NbProducts + j];
      if (p->getQuantity() == 0)
        _products.removePossible(p);
      else if (_id == i+1)
        _products.addRequired(p);
      else
        _products.removePossible(p);
    }
  }
}

void OrderI::printName(ostream& out) const {
  out << "Order#" << _id;
}

//-----------------------------------------------------

void ProductI::printName(ostream& out) const {
  _order->printName(out);
  out << ".P" << _type;
}

//-----------------------------------------------------

IloAnyArray MakeOrders(IloEnv env) {
  IloInt i;

  for(i = 0; i < NbOrders; i++) {
    Orders[i] = new (env) OrderI(i+1);
    IloInt quantity = 0;
    for (IloInt j = 0; j < NbProducts; j++) {
      IloInt q = (IloInt)Quantities[i][j];
      quantity += q;
      Products[i*NbProducts + j] = new (env) ProductI(Orders[i], j+1, q);
    }
    Orders[i]->setQuantity(quantity);
  }

  IloAnyArray prodOrdersArray(env, NbOrders*NbProducts);
  for(i = 0; i < NbOrders*NbProducts; i++)
    prodOrdersArray[i] = (IloAny)Products[i];

  for(i = 0; i < NbOrders; i++)
    Orders[i]->makeProductVar(env,prodOrdersArray);

  IloAnyArray ordersArray(env, NbOrders);
  for(i = 0; i < NbOrders; i++)
    ordersArray[i] = (IloAny)Orders[i];

  return ordersArray;
}

//-----------------------------------------------------

class TankI {
protected:
  IloInt       _id;
  IloIntVar    _type;
  IloIntVar    _load;
  IloInt       _capaMax;
  IloAnySetVar _prodOrders;
public:
  TankI(IloModel model, IloInt id, IloInt capaMax);

  IloInt       getId() const            { return _id; }
  IloIntVar    getType() const          { return _type; }
  IloIntVar    getLoad() const          { return _load; }
  IloInt       getCapacityMax() const   { return _capaMax; }
  IloAnySetVar getProductOrders() const { return _prodOrders; }

  void display  (IloSolver solver) const;
  void printName(ostream& out) const {
    out << "Tank#" << _id;
  }
};

typedef TankI* Tank;

IloFunction<IloAny,IloIntVar>     TankLoad(IloEnv env);
IloFunction<IloAny,IloIntVar>     TankType(IloEnv env);
IloFunction<IloAny,IloAnySetVar>  TankProducts(IloEnv env);

//-----------------------------------------------------
TankI::TankI(IloModel model, IloInt id, IloInt capaMax)
  :_id(id), _capaMax(capaMax) {

  IloEnv env = model.getEnv();

  _type = IloIntVar(env, 1, 5);
  _load = IloIntVar(env, 0, capaMax);

  IloAnyArray prodOrdersArray = IloAnyArray(env, NbOrders*NbProducts);
  for (IloInt i = 0; i < NbOrders*NbProducts; i++)
    prodOrdersArray[i] = (IloAny)Products[i];

  _prodOrders = IloAnySetVar(env, prodOrdersArray);

  //All the products assigned to a tank must belong
  //to the same type of product

  model.add( IloEqMin(env,  _prodOrders, _type, PType(env)) );
  model.add( IloEqMax(env,  _prodOrders, _type, PType(env)) );

  //The loading of the tank cannot exceed its capacity

  model.add( IloEqSum(env,  _prodOrders, _load, PQuantity(env)) );
}

void TankI::display(IloSolver solver) const {
  if (solver.getAnySetVar(_prodOrders).getCardinality().getMax() == 0)
    return; //Empty
  solver.out() << "       <";
  printName(solver.out());
  solver.out() << ">" << endl
      << "              --> Capacity  = " << _capaMax << endl
      << "              --> Type      = " << solver.getIntVar(_type) << endl
      << "              --> Load      = " << solver.getIntVar(_load) << endl
      << "              --> Orders    = (";
  for(IlcAnySetIterator it(solver.getAnySetVar(_prodOrders).getRequiredSet());
      it.ok(); ++it) {
    ((Product)(*it))->printName(solver.out());
    solver.out() << " ";
  }
  solver.out() << ")" << endl ;
}

//-----------------------------------------------------

class TruckI {
private:
  IloInt              _id;
  IloBoolVar           _used;
  IloAnySetVar        _orders;
  IloArray<Tank>      _tanks;
  IloAnySetVar        _tankVar;
public:
  TruckI(IloModel model, IloInt id);

  IloInt               getId() const        { return _id; }
  IloBoolVar            getUsed() const      { return _used; }
  IloAnySetVar         getOrders() const    { return _orders; }
  IloInt               getNbTanks() const   { return 5; }
  IloArray<Tank>       getTanks() const     { return _tanks; }
  IloAnySetVar         getTankVar() const   { return _tankVar; }

  void display(IloSolver solver) const;
};

typedef TruckI* Truck;

TruckI::TruckI(IloModel model, IloInt id) :_id(id) {
  IloEnv env = model.getEnv();
  IloInt i;

  IloAnyArray ordersArray = IloAnyArray(env, NbOrders);
  for (i = 0; i < NbOrders; i++)
    ordersArray[i] = (IloAny)Orders[i];

  _orders = IloAnySetVar(env, ordersArray);

  _tanks = IloArray<Tank> (env, getNbTanks());

  IloAnySetVarArray prodVars(env, getNbTanks());
  for(i = 0; i < getNbTanks(); i++) {
    _tanks[i] = new (env) TankI(model, i+1, (i+1)*1000);
    prodVars[i] = _tanks[i]->getProductOrders();
  }

  IloAnyArray tanksArray = IloAnyArray(env, getNbTanks());
  for (i = 0; i < getNbTanks(); i++)
    tanksArray[i] = (IloAny)_tanks[i];

  _tankVar = IloAnySetVar(env, tanksArray);

  //The truck is used if one order is assigned to it
  _used = IloBoolVar(env);
  model.add( _used == (IloCard(_orders) > 0));

  //Maximal loading of the truck
  IloIntVar load(env, 0, 12000);

  model.add( IloEqSum(env,  _orders, load,  OQuantity(env) ));
  model.add( IloEqSum(env,  _tankVar, load, TankLoad(env) ));

  IloAnyArray prodOrdersArray = IloAnyArray(env, NbOrders*NbProducts);
  for (i = 0; i < NbOrders*NbProducts; i++)
    prodOrdersArray[i] = (IloAny)Products[i];

  //Products assigned to the tanks of the truck
  IloAnySetVar tprodVar(env, prodOrdersArray);
  model.add( IloEqUnion(env, _tankVar, tprodVar, TankProducts(env) ));

  //Products of the orders assigned to the truck
  IloAnySetVar oprodVar(env, prodOrdersArray);
  model.add( IloEqUnion(env, _orders, oprodVar,  OrderProducts(env) ));

  //Product assigned to the tanks are the products of the
  //orders assigned to the truck
  model.add( tprodVar == oprodVar );

  //All the products must be assigned to one and only one tank
  model.add( IloEqPartition(env,  oprodVar, prodVars ) );


  //The types of tanks on a truck
  IloIntSetVar ttanks(env, IloIntArray(env, 5, 1, 2, 3, 4, 5));
  model.add(IloEqUnion(env, _tankVar, ttanks, TankType(env)));

  //A truck can be assigned at most 3 different types of products
  model.add( IloCard( ttanks ) <= 3 );
  //Product 3 and 4 are incompatibe
  model.add( ! (IloMember(env, 3, ttanks) && IloMember(env, 4, ttanks)) );
}

//-----------------------------------------------------

void TruckI::display(IloSolver solver) const {
  solver.out();
  if (! solver.getIntVar(_used).isBound())
    return;
  if (solver.getIntVar(_used).getValue() == 0)
        return;
  solver.out() << "<Truck T#" << _id << ">" << endl
     << "       --> Orders = (";
  for(IlcAnySetIterator it(solver.getAnySetVar(_orders).getRequiredSet());
      it.ok(); ++it) {
    ((Order)(*it))->printName(solver.out());
    solver.out() << " ";
  }
  solver.out() << ")" << endl;
  solver.out() << "       --> Tank      = (";
  for(IlcAnySetIterator itT(solver.getAnySetVar(_tankVar).getRequiredSet());
      itT.ok(); ++itT ) {
    ((Tank)(*itT))->printName(solver.out());
    solver.out() << " ";
  }
  solver.out() << ")" << endl;

  for(IlcAnySetIterator itT2(solver.getAnySetVar(_tankVar).getRequiredSet());
      itT2.ok(); ++itT2 ) {
    ((Tank)(*itT2))->display(solver);
  }
  solver.out() << endl;
}

//-----------------------------------------------------
// Solving goals
//-----------------------------------------------------

// Choose the tank the most hard to fill;
// with the smallest domain.

Tank ChooseTank(IloSolver solver, Product p, IlcAnySet tankset) {
  Tank best=0;
  IloInt eval = IlcIntMax;
  for(IlcAnySetIterator it(tankset); it.ok(); ++it) {
    Tank tank = (Tank)(*it);
    IlcAnySetVar pvar = solver.getAnySetVar(tank->getProductOrders());
    if (pvar.isRequired(p))
      return tank;
    if (! pvar.isPossible(p))
      continue;

    IloInt size = pvar.getPossibleSet().getSize();
    if (size < eval) {
      eval = size;
      best = tank;
    }
  }
  return best;
}

ILCGOAL2(AssignProduct, Product, product, Truck, truck) {
  IloSolver solver = getSolver();
  //Choose an already used tank
  IlcAnySetVar stanks = solver.getAnySetVar(truck->getTankVar());
  Tank tank = ChooseTank(solver, product, stanks.getRequiredSet());
  if (tank == 0)
    //Choose a new tank
    tank = ChooseTank(solver, product, stanks.getPossibleSet());
  if (tank == 0)
    fail();

  IlcAnySetVar stankvar = solver.getAnySetVar(tank->getProductOrders());
  if (stankvar.isRequired(product))
    return 0;
  return IlcOr(IlcMember(product, stankvar),
               IlcAnd(IlcNotMember(product, stankvar),
		      this));
}

ILCGOAL2(AssignProducts, Order, order, Truck, truck) {
  IloSolver solver = getSolver();
  IlcAnySetVar prodVar = solver.getAnySetVar(order->getProducts());
  IlcGoal g = IlcGoalTrue(solver);
  for(IlcAnySetIterator it(prodVar.getRequiredSet()); it.ok(); ++it)
    g = IloAndGoal(solver, g,  AssignProduct(solver, (Product)(*it), truck));
  return g;
}

// Choose the truck the most hard to fill;
// with the smallest domain.
Truck ChooseTruck(IloSolver solver,
		   Order order,
                   IloInt nbTrucks, IloArray<Truck> trucks,
                   IlcBool aNewOne) {
  Truck best=0;
  IloInt eval = IlcIntMax;
  for(IloInt i=0; i < nbTrucks; i++) {
    IlcAnySetVar oVar = solver.getAnySetVar(trucks[i]->getOrders());
    IloInt cardMin = IlcCard(oVar).getMin();
    if (aNewOne) {
      if (cardMin > 0)
	continue;
    } else {
      if (cardMin == 0)
	continue;
      if (oVar.isRequired(order))
	return trucks[i];
    }
    if (! oVar.isPossible(order))
      continue;
    IloInt size = oVar.getPossibleSet().getSize();
    if (size < eval) {
      eval = size;
      best = trucks[i];
    }
  }
  return best;
}

ILCGOAL3(AssignOrder, Order, order, IloInt, nbTrucks, IloArray<Truck>, trucks) {
  IloSolver solver = getSolver();
  //Choose first an already used truck
  Truck t = ChooseTruck(solver, order, nbTrucks, trucks, IlcFalse);
  if (t == 0) {
    //Choose a new truck
    t = ChooseTruck(solver, order, nbTrucks, trucks, IlcTrue);
  }
  if (t == 0)
    fail();

  IlcAnySetVar tVar = solver.getAnySetVar(t->getOrders());
  if (tVar.isRequired(order))
    return AssignProducts(solver, order, t);
  else
    return IlcOr(IlcAnd(IlcMember(order, tVar),
                        AssignProducts(solver, order, t)),
                 IlcAnd(IlcNotMember(order, tVar),
			this));
}

ILOCPGOALWRAPPER3(IloAssignOrder, solver, Order, ord, IloInt, nbTrucks, IloArray<Truck>, trucks) {
  return AssignOrder(solver, ord, nbTrucks, trucks);
}

IloGoal IloAssignOrders (IloEnv env, IloAnyArray orders, IloInt nbTrucks, IloArray<Truck> trucks) {
  IloGoal goal = IloGoalTrue(env);
  for(IloInt i = 0; i < orders.getSize(); i++)
    goal = goal && IloAssignOrder(env, (Order)orders[i], nbTrucks, trucks);
  return goal;
}

IloGoal IloEliminateTrucks (IloEnv env, IloInt nbTrucks, IloInt nbMax, IloArray<Truck> trucks) {
  IloGoal goal = IloGoalTrue(env);
  for(IloInt i = nbTrucks; i > nbMax; i--)
    goal = goal && IloAddConstraint(trucks[i-1]->getUsed() == 0);
  return goal;
}

//-----------------------------------------------------
// Main program
//-----------------------------------------------------

int main () {
  IloEnv env;
  try {
    IloModel model(env);

    IloAnyArray  ordersArray = MakeOrders(env);
    IloAnySetVar orders(env, ordersArray);

    model.add(IloCard(orders) == NbOrders);

    //At most 1 Truck per order: Create initially NbOrders trucks

    IloArray<Truck> trucks(env, NbOrders);

    IloBoolVarArray usedVars(env, NbOrders);
    IloAnySetVarArray ovars(env, NbOrders);

    IloInt i;
    for(i=0; i < NbOrders; i++) {
      trucks[i] = new (env) TruckI(model, i+1);
      usedVars[i] = trucks[i]->getUsed();
      ovars[i] = trucks[i]->getOrders();
    }

    //All orders must be assigned to a truck

    model.add( IloEqPartition(env, orders, ovars) );

    //Cost Variable

    IloIntVar costVar(env, 0, NbOrders);

    model.add( costVar == IloSum(usedVars) );

    //Optimization

    IloSolver solver(model);

    IloInt nbMax = NbOrders+1;
    IloBool ok = IloTrue;
    IloGoal g;

    while (ok) {

      //Strategy: Eliminate unuseful trucks,
      //          Assign the orders to trucks, choosing the
      //          order with the biggest quantity first.

      g = IloEliminateTrucks(env, NbOrders, nbMax-1, trucks) &&
          IloAssignOrders(env, ordersArray, nbMax-1, trucks);

      ok = solver.solve(g);

      if (ok) {
	nbMax = (IloInt)solver.getValue(costVar);
	solver.out() << "--> Solution found at: "
		     << nbMax << " trucks." << endl;
      }
    }
    if (nbMax > NbOrders)
      solver.out() << "--> No Solution !" << endl;
    else {
      model.add(costVar == nbMax);

      g = IloEliminateTrucks(env, NbOrders, nbMax, trucks) &&
          IloAssignOrders(env, ordersArray, nbMax, trucks);

      solver.solve(g);

      solver.out() << endl;

      for(i = 0; i < NbOrders; i++)
	trucks[i]->display(solver);
    }
    solver.printInformation();
  }
  catch (IloException& ex) {
    cout << "Error: " << ex << endl;
  }
  env.end();
  return 0;

}
//-----------------------------------------------------
// Accessors
//-----------------------------------------------------

class PTypeI : public IloFunctionI<IloAny,IloInt> {
public:
  PTypeI(IloEnvI* e) : IloFunctionI<IloAny,IloInt>(e)  {}
  IloInt getValue(IloAny elt);
};

IloInt PTypeI::getValue(IloAny elt) {
  return ((Product)elt)->getType();
}

IloFunction<IloAny,IloInt> PType(IloEnv env) {
  return new (env) PTypeI(env.getImpl());
}


class PQuantityI : public IloFunctionI<IloAny,IloInt> {
public:
  PQuantityI(IloEnvI* e) : IloFunctionI<IloAny,IloInt>(e) {}
  IloInt getValue(IloAny elt);
};

IloInt PQuantityI::getValue(IloAny elt) {
  return ((Product)elt)->getQuantity();
}

IloFunction<IloAny,IloInt> PQuantity(IloEnv env) {
  return new (env) PQuantityI(env.getImpl());
}


class OQuantityI : public IloFunctionI<IloAny,IloInt> {
public:
  OQuantityI(IloEnvI* e) : IloFunctionI<IloAny,IloInt>(e) {}
  IloInt getValue(IloAny elt);
};

IloInt OQuantityI::getValue(IloAny elt) {
  return ((Order)elt)->getQuantity();
}

IloFunction<IloAny,IloInt> OQuantity(IloEnv env) {
  return new (env) OQuantityI(env.getImpl());
}

class OrderProductI : public IloFunctionI<IloAny,IloAnySetVar> {
public:
  OrderProductI(IloEnvI* e) : IloFunctionI<IloAny, IloAnySetVar>(e)  {}
  IloAnySetVar getValue(IloAny elt);
};

IloAnySetVar OrderProductI::getValue(IloAny elt) {
  return ((Order)elt)->getProducts();
}

IloFunction<IloAny,IloAnySetVar> OrderProducts(IloEnv env) {
  return new (env) OrderProductI(env.getImpl());
}


class TankLoadI : public IloFunctionI<IloAny,IloIntVar> {
public:
  TankLoadI(IloEnvI* e) : IloFunctionI<IloAny,IloIntVar>(e) {}
  IloIntVar getValue(IloAny elt);
};

IloIntVar TankLoadI::getValue(IloAny elt) {
  return ((Tank)elt)->getLoad();
}

IloFunction<IloAny,IloIntVar> TankLoad(IloEnv env) {
  return new (env) TankLoadI(env.getImpl());
}


class TankTypeI : public IloFunctionI<IloAny,IloIntVar> {
public:
  TankTypeI(IloEnvI* e) : IloFunctionI<IloAny,IloIntVar>(e) {}
  IloIntVar getValue(IloAny elt);
};

IloIntVar TankTypeI::getValue(IloAny elt) {
  return ((Tank)elt)->getType();
}

IloFunction<IloAny,IloIntVar> TankType(IloEnv env) {
  return new (env) TankTypeI(env.getImpl());
}


class TankProductI : public IloFunctionI<IloAny,IloAnySetVar> {
public:
  TankProductI(IloEnvI* e) : IloFunctionI<IloAny, IloAnySetVar>(e) {}
  IloAnySetVar getValue(IloAny elt);
};

IloAnySetVar TankProductI::getValue(IloAny elt) {
  return ((Tank)elt)->getProductOrders();
}

IloFunction<IloAny,IloAnySetVar> TankProducts(IloEnv env) {
  return new (env) TankProductI(env.getImpl());
}
