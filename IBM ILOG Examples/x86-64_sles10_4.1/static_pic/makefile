SYSTEM = x86-64_sles10_4.1
LIBFORMAT = static_pic

CCC = g++

# System/lib-format specific compile/link flags
CSYSFLAGS  = -fPIC -DIL_STD 
LDSYSFLAGS = 

# Compilation and linking flags enabling Multi-threading
CMTFLAGS = -DILOUSEMT -D_REENTRANT
LDMTFLAGS = -lpthread

# Code optimization/debugging options
DEBUG = -O3 -fomit-frame-pointer -DNDEBUG

# JDK environment
JDK = 
JDKOPTIONS = 
JDKINCLUDE = linux

#------------------------------------------------------------
#
# When you adapt this makefile to compile your SOLVER programs
# please copy this makefile and set SOLVERDIR to the directory
# where SOLVER was installed. Please do the same for the CONCERTDIR directory
#
#------------------------------------------------------------

SUFFIX=cpp
SOLVERDIR=/opt/ibm/ILOG/CP17/solver
CONCERTDIR=$(SOLVERDIR)/../concert

EXDIR=$(SOLVERDIR)/examples
#EXDIR="/home/bartas/workspace/IBM ILOG Examples"
EXSRC=$(EXDIR)/src
EXINC=$(EXDIR)/include
EXDATA=$(EXDIR)/data

SOLVERIncDir=$(SOLVERDIR)/include
CONCERTIncDir=$(CONCERTDIR)/include

#DEMODIR=$(SOLVERDIR)/examples
DEMODIR=/home/bartas/workspace/IBM\ ILOG\ Examples
DEMOSRCDIR=$(DEMODIR)/src

CFLAGS=$(CSYSFLAGS) $(DEBUG) $(OPTIONS) -I$(SOLVERIncDir) -I$(CONCERTIncDir) -I$(EXINC) $(CMTFLAGS)
LDFLAGS=-L$(SOLVERDIR)/lib/$(SYSTEM)/$(LIBFORMAT) -lsolver -lsolverfloat -L$(CONCERTDIR)/lib/$(SYSTEM)/$(LIBFORMAT) -lconcert $(LDSYSFLAGS) $(LDMTFLAGS) $(LIBS)

include ../../UserMake.iim

#---------------------------------------------------------
# FILES
#---------------------------------------------------------
TEST = addsoft binpacking binpacking_ex3 carseq \
          carseq_basic carseq_basic_ex3 carseq_basic_ex4 carseq_limit color \
          color_ex3 color_ex4 colormin colormin_ex3 colormin_ex4 \
          crews crews_ex3 crews_ex4 donald equation \
          facility_solver float folium freq graph \
          intro kin magicsq magicsq_ex3 magicsq_ex4 \
          magicsq_impact money money_ex3 money_ex4 narin \
          nqueen rack replan replan_store sports_basic \
          squares squeen storecpx storecpx_ex3 storesbs \
          storesbs_ex3 storesbs_ex4 tankers usetrace whouse_mt \
          whouse_mt2 wilkins 

execute: $(TEST)
	./addsoft
	./binpacking
	./binpacking_ex3
	./carseq
	./carseq_basic
	./carseq_basic_ex3
	./carseq_basic_ex4
	./carseq_limit
	./color
	./color_ex3
	./color_ex4
	./colormin
	./colormin_ex3
	./colormin_ex4
	./crews
	./crews_ex3
	./crews_ex4
	./donald
	./equation
	./facility_solver
	./float
	./folium
	./freq
	./graph
	./intro
	./kin
	./magicsq
	./magicsq_ex3
	./magicsq_ex4
	./magicsq_impact
	./money
	./money_ex3
	./money_ex4
	./narin
	./nqueen
	./rack
	./replan
	./replan_store
	./sports_basic
	./squares
	./squeen
	./storecpx
	./storecpx_ex3
	./storesbs
	./storesbs_ex3
	./storesbs_ex4
	./tankers
	./usetrace
	./whouse_mt
	./whouse_mt2
	./wilkins

build: $(TEST)

clean:
	/bin/rm -rf *.o
	/bin/rm -rf $(TEST)
	touch out.out
	/bin/rm -rf *.out

addsoft.o: $(DEMOSRCDIR)/addsoft.$(SUFFIX)
	$(CCC) $(CFLAGS) $(DEMOSRCDIR)/addsoft.$(SUFFIX) -c
addsoft: addsoft.o
	$(CCC) $(CFLAGS) addsoft.o -o addsoft $(LDFLAGS)

binpacking.o: $(DEMOSRCDIR)/binpacking.$(SUFFIX)
	$(CCC) $(CFLAGS) $(DEMOSRCDIR)/binpacking.$(SUFFIX) -c
binpacking: binpacking.o
	$(CCC) $(CFLAGS) binpacking.o -o binpacking $(LDFLAGS)

binpacking_ex3.o: $(DEMOSRCDIR)/binpacking_ex3.$(SUFFIX)
	$(CCC) $(CFLAGS) $(DEMOSRCDIR)/binpacking_ex3.$(SUFFIX) -c
binpacking_ex3: binpacking_ex3.o
	$(CCC) $(CFLAGS) binpacking_ex3.o -o binpacking_ex3 $(LDFLAGS)

carseq.o: $(DEMOSRCDIR)/carseq.$(SUFFIX)
	$(CCC) $(CFLAGS) $(DEMOSRCDIR)/carseq.$(SUFFIX) -c
carseq: carseq.o
	$(CCC) $(CFLAGS) carseq.o -o carseq $(LDFLAGS)

carseq_basic.o: $(DEMOSRCDIR)/carseq_basic.$(SUFFIX)
	$(CCC) $(CFLAGS) $(DEMOSRCDIR)/carseq_basic.$(SUFFIX) -c
carseq_basic: carseq_basic.o
	$(CCC) $(CFLAGS) carseq_basic.o -o carseq_basic $(LDFLAGS)

carseq_basic_ex3.o: $(DEMOSRCDIR)/carseq_basic_ex3.$(SUFFIX)
	$(CCC) $(CFLAGS) $(DEMOSRCDIR)/carseq_basic_ex3.$(SUFFIX) -c
carseq_basic_ex3: carseq_basic_ex3.o
	$(CCC) $(CFLAGS) carseq_basic_ex3.o -o carseq_basic_ex3 $(LDFLAGS)

carseq_basic_ex4.o: $(DEMOSRCDIR)/carseq_basic_ex4.$(SUFFIX)
	$(CCC) $(CFLAGS) $(DEMOSRCDIR)/carseq_basic_ex4.$(SUFFIX) -c
carseq_basic_ex4: carseq_basic_ex4.o
	$(CCC) $(CFLAGS) carseq_basic_ex4.o -o carseq_basic_ex4 $(LDFLAGS)

carseq_limit.o: $(DEMOSRCDIR)/carseq_limit.$(SUFFIX)
	$(CCC) $(CFLAGS) $(DEMOSRCDIR)/carseq_limit.$(SUFFIX) -c
carseq_limit: carseq_limit.o
	$(CCC) $(CFLAGS) carseq_limit.o -o carseq_limit $(LDFLAGS)

color.o: $(DEMOSRCDIR)/color.$(SUFFIX)
	$(CCC) $(CFLAGS) $(DEMOSRCDIR)/color.$(SUFFIX) -c
color: color.o
	$(CCC) $(CFLAGS) color.o -o color $(LDFLAGS)

color_ex3.o: $(DEMOSRCDIR)/color_ex3.$(SUFFIX)
	$(CCC) $(CFLAGS) $(DEMOSRCDIR)/color_ex3.$(SUFFIX) -c
color_ex3: color_ex3.o
	$(CCC) $(CFLAGS) color_ex3.o -o color_ex3 $(LDFLAGS)

color_ex4.o: $(DEMOSRCDIR)/color_ex4.$(SUFFIX)
	$(CCC) $(CFLAGS) $(DEMOSRCDIR)/color_ex4.$(SUFFIX) -c
color_ex4: color_ex4.o
	$(CCC) $(CFLAGS) color_ex4.o -o color_ex4 $(LDFLAGS)

colormin.o: $(DEMOSRCDIR)/colormin.$(SUFFIX)
	$(CCC) $(CFLAGS) $(DEMOSRCDIR)/colormin.$(SUFFIX) -c
colormin: colormin.o
	$(CCC) $(CFLAGS) colormin.o -o colormin $(LDFLAGS)

colormin_ex3.o: $(DEMOSRCDIR)/colormin_ex3.$(SUFFIX)
	$(CCC) $(CFLAGS) $(DEMOSRCDIR)/colormin_ex3.$(SUFFIX) -c
colormin_ex3: colormin_ex3.o
	$(CCC) $(CFLAGS) colormin_ex3.o -o colormin_ex3 $(LDFLAGS)

colormin_ex4.o: $(DEMOSRCDIR)/colormin_ex4.$(SUFFIX)
	$(CCC) $(CFLAGS) $(DEMOSRCDIR)/colormin_ex4.$(SUFFIX) -c
colormin_ex4: colormin_ex4.o
	$(CCC) $(CFLAGS) colormin_ex4.o -o colormin_ex4 $(LDFLAGS)

crews.o: $(DEMOSRCDIR)/crews.$(SUFFIX)
	$(CCC) $(CFLAGS) $(DEMOSRCDIR)/crews.$(SUFFIX) -c
crews: crews.o
	$(CCC) $(CFLAGS) crews.o -o crews $(LDFLAGS)

crews_ex3.o: $(DEMOSRCDIR)/crews_ex3.$(SUFFIX)
	$(CCC) $(CFLAGS) $(DEMOSRCDIR)/crews_ex3.$(SUFFIX) -c
crews_ex3: crews_ex3.o
	$(CCC) $(CFLAGS) crews_ex3.o -o crews_ex3 $(LDFLAGS)

crews_ex4.o: $(DEMOSRCDIR)/crews_ex4.$(SUFFIX)
	$(CCC) $(CFLAGS) $(DEMOSRCDIR)/crews_ex4.$(SUFFIX) -c
crews_ex4: crews_ex4.o
	$(CCC) $(CFLAGS) crews_ex4.o -o crews_ex4 $(LDFLAGS)

donald.o: $(DEMOSRCDIR)/donald.$(SUFFIX)
	$(CCC) $(CFLAGS) $(DEMOSRCDIR)/donald.$(SUFFIX) -c
donald: donald.o
	$(CCC) $(CFLAGS) donald.o -o donald $(LDFLAGS)

equation.o: $(DEMOSRCDIR)/equation.$(SUFFIX)
	$(CCC) $(CFLAGS) $(DEMOSRCDIR)/equation.$(SUFFIX) -c
equation: equation.o
	$(CCC) $(CFLAGS) equation.o -o equation $(LDFLAGS)

facility_solver.o: $(DEMOSRCDIR)/facility_solver.$(SUFFIX)
	$(CCC) $(CFLAGS) $(DEMOSRCDIR)/facility_solver.$(SUFFIX) -c
facility_solver: facility_solver.o
	$(CCC) $(CFLAGS) facility_solver.o -o facility_solver $(LDFLAGS)

float.o: $(DEMOSRCDIR)/float.$(SUFFIX)
	$(CCC) $(CFLAGS) $(DEMOSRCDIR)/float.$(SUFFIX) -c
float: float.o
	$(CCC) $(CFLAGS) float.o -o float $(LDFLAGS)

folium.o: $(DEMOSRCDIR)/folium.$(SUFFIX)
	$(CCC) $(CFLAGS) $(DEMOSRCDIR)/folium.$(SUFFIX) -c
folium: folium.o
	$(CCC) $(CFLAGS) folium.o -o folium $(LDFLAGS)

freq.o: $(DEMOSRCDIR)/freq.$(SUFFIX)
	$(CCC) $(CFLAGS) $(DEMOSRCDIR)/freq.$(SUFFIX) -c
freq: freq.o
	$(CCC) $(CFLAGS) freq.o -o freq $(LDFLAGS)

graph.o: $(DEMOSRCDIR)/graph.$(SUFFIX)
	$(CCC) $(CFLAGS) $(DEMOSRCDIR)/graph.$(SUFFIX) -c
graph: graph.o
	$(CCC) $(CFLAGS) graph.o -o graph $(LDFLAGS)

intro.o: $(DEMOSRCDIR)/intro.$(SUFFIX)
	$(CCC) $(CFLAGS) $(DEMOSRCDIR)/intro.$(SUFFIX) -c
intro: intro.o
	$(CCC) $(CFLAGS) intro.o -o intro $(LDFLAGS)

kin.o: $(DEMOSRCDIR)/kin.$(SUFFIX)
	$(CCC) $(CFLAGS) $(DEMOSRCDIR)/kin.$(SUFFIX) -c
kin: kin.o
	$(CCC) $(CFLAGS) kin.o -o kin $(LDFLAGS)

magicsq.o: $(DEMOSRCDIR)/magicsq.$(SUFFIX)
	$(CCC) $(CFLAGS) $(DEMOSRCDIR)/magicsq.$(SUFFIX) -c
magicsq: magicsq.o
	$(CCC) $(CFLAGS) magicsq.o -o magicsq $(LDFLAGS)

magicsq_ex3.o: $(DEMOSRCDIR)/magicsq_ex3.$(SUFFIX)
	$(CCC) $(CFLAGS) $(DEMOSRCDIR)/magicsq_ex3.$(SUFFIX) -c
magicsq_ex3: magicsq_ex3.o
	$(CCC) $(CFLAGS) magicsq_ex3.o -o magicsq_ex3 $(LDFLAGS)

magicsq_ex4.o: $(DEMOSRCDIR)/magicsq_ex4.$(SUFFIX)
	$(CCC) $(CFLAGS) $(DEMOSRCDIR)/magicsq_ex4.$(SUFFIX) -c
magicsq_ex4: magicsq_ex4.o
	$(CCC) $(CFLAGS) magicsq_ex4.o -o magicsq_ex4 $(LDFLAGS)

magicsq_impact.o: $(DEMOSRCDIR)/magicsq_impact.$(SUFFIX)
	$(CCC) $(CFLAGS) $(DEMOSRCDIR)/magicsq_impact.$(SUFFIX) -c
magicsq_impact: magicsq_impact.o
	$(CCC) $(CFLAGS) magicsq_impact.o -o magicsq_impact $(LDFLAGS)

money.o: $(DEMOSRCDIR)/money.$(SUFFIX)
	$(CCC) $(CFLAGS) $(DEMOSRCDIR)/money.$(SUFFIX) -c
money: money.o
	$(CCC) $(CFLAGS) money.o -o money $(LDFLAGS)

money_ex3.o: $(DEMOSRCDIR)/money_ex3.$(SUFFIX)
	$(CCC) $(CFLAGS) $(DEMOSRCDIR)/money_ex3.$(SUFFIX) -c
money_ex3: money_ex3.o
	$(CCC) $(CFLAGS) money_ex3.o -o money_ex3 $(LDFLAGS)

money_ex4.o: $(DEMOSRCDIR)/money_ex4.$(SUFFIX)
	$(CCC) $(CFLAGS) $(DEMOSRCDIR)/money_ex4.$(SUFFIX) -c
money_ex4: money_ex4.o
	$(CCC) $(CFLAGS) money_ex4.o -o money_ex4 $(LDFLAGS)

narin.o: $(DEMOSRCDIR)/narin.$(SUFFIX)
	$(CCC) $(CFLAGS) $(DEMOSRCDIR)/narin.$(SUFFIX) -c
narin: narin.o
	$(CCC) $(CFLAGS) narin.o -o narin $(LDFLAGS)

nqueen.o: $(DEMOSRCDIR)/nqueen.$(SUFFIX)
	$(CCC) $(CFLAGS) $(DEMOSRCDIR)/nqueen.$(SUFFIX) -c
nqueen: nqueen.o
	$(CCC) $(CFLAGS) nqueen.o -o nqueen $(LDFLAGS)

rack.o: $(DEMOSRCDIR)/rack.$(SUFFIX)
	$(CCC) $(CFLAGS) $(DEMOSRCDIR)/rack.$(SUFFIX) -c
rack: rack.o
	$(CCC) $(CFLAGS) rack.o -o rack $(LDFLAGS)

replan.o: $(DEMOSRCDIR)/replan.$(SUFFIX)
	$(CCC) $(CFLAGS) $(DEMOSRCDIR)/replan.$(SUFFIX) -c
replan: replan.o
	$(CCC) $(CFLAGS) replan.o -o replan $(LDFLAGS)

replan_store.o: $(DEMOSRCDIR)/replan_store.$(SUFFIX)
	$(CCC) $(CFLAGS) $(DEMOSRCDIR)/replan_store.$(SUFFIX) -c
replan_store: replan_store.o
	$(CCC) $(CFLAGS) replan_store.o -o replan_store $(LDFLAGS)

sports_basic.o: $(DEMOSRCDIR)/sports_basic.$(SUFFIX)
	$(CCC) $(CFLAGS) $(DEMOSRCDIR)/sports_basic.$(SUFFIX) -c
sports_basic: sports_basic.o
	$(CCC) $(CFLAGS) sports_basic.o -o sports_basic $(LDFLAGS)

squares.o: $(DEMOSRCDIR)/squares.$(SUFFIX)
	$(CCC) $(CFLAGS) $(DEMOSRCDIR)/squares.$(SUFFIX) -c
squares: squares.o
	$(CCC) $(CFLAGS) squares.o -o squares $(LDFLAGS)

squeen.o: $(DEMOSRCDIR)/squeen.$(SUFFIX)
	$(CCC) $(CFLAGS) $(DEMOSRCDIR)/squeen.$(SUFFIX) -c
squeen: squeen.o
	$(CCC) $(CFLAGS) squeen.o -o squeen $(LDFLAGS)

storecpx.o: $(DEMOSRCDIR)/storecpx.$(SUFFIX)
	$(CCC) $(CFLAGS) $(DEMOSRCDIR)/storecpx.$(SUFFIX) -c
storecpx: storecpx.o
	$(CCC) $(CFLAGS) storecpx.o -o storecpx $(LDFLAGS)

storecpx_ex3.o: $(DEMOSRCDIR)/storecpx_ex3.$(SUFFIX)
	$(CCC) $(CFLAGS) $(DEMOSRCDIR)/storecpx_ex3.$(SUFFIX) -c
storecpx_ex3: storecpx_ex3.o
	$(CCC) $(CFLAGS) storecpx_ex3.o -o storecpx_ex3 $(LDFLAGS)

storesbs.o: $(DEMOSRCDIR)/storesbs.$(SUFFIX)
	$(CCC) $(CFLAGS) $(DEMOSRCDIR)/storesbs.$(SUFFIX) -c
storesbs: storesbs.o
	$(CCC) $(CFLAGS) storesbs.o -o storesbs $(LDFLAGS)

storesbs_ex3.o: $(DEMOSRCDIR)/storesbs_ex3.$(SUFFIX)
	$(CCC) $(CFLAGS) $(DEMOSRCDIR)/storesbs_ex3.$(SUFFIX) -c
storesbs_ex3: storesbs_ex3.o
	$(CCC) $(CFLAGS) storesbs_ex3.o -o storesbs_ex3 $(LDFLAGS)

storesbs_ex4.o: $(DEMOSRCDIR)/storesbs_ex4.$(SUFFIX)
	$(CCC) $(CFLAGS) $(DEMOSRCDIR)/storesbs_ex4.$(SUFFIX) -c
storesbs_ex4: storesbs_ex4.o
	$(CCC) $(CFLAGS) storesbs_ex4.o -o storesbs_ex4 $(LDFLAGS)

tankers.o: $(DEMOSRCDIR)/tankers.$(SUFFIX)
	$(CCC) $(CFLAGS) $(DEMOSRCDIR)/tankers.$(SUFFIX) -c
tankers: tankers.o
	$(CCC) $(CFLAGS) tankers.o -o tankers $(LDFLAGS)

usetrace.o: $(DEMOSRCDIR)/usetrace.$(SUFFIX)
	$(CCC) $(CFLAGS) $(DEMOSRCDIR)/usetrace.$(SUFFIX) -c
usetrace: usetrace.o
	$(CCC) $(CFLAGS) usetrace.o -o usetrace $(LDFLAGS)

whouse_mt.o: $(DEMOSRCDIR)/whouse_mt.$(SUFFIX)
	$(CCC) $(CFLAGS) $(DEMOSRCDIR)/whouse_mt.$(SUFFIX) -c
whouse_mt: whouse_mt.o
	$(CCC) $(CFLAGS) whouse_mt.o -o whouse_mt $(LDFLAGS)

whouse_mt2.o: $(DEMOSRCDIR)/whouse_mt2.$(SUFFIX)
	$(CCC) $(CFLAGS) $(DEMOSRCDIR)/whouse_mt2.$(SUFFIX) -c
whouse_mt2: whouse_mt2.o
	$(CCC) $(CFLAGS) whouse_mt2.o -o whouse_mt2 $(LDFLAGS)

wilkins.o: $(DEMOSRCDIR)/wilkins.$(SUFFIX)
	$(CCC) $(CFLAGS) $(DEMOSRCDIR)/wilkins.$(SUFFIX) -c
wilkins: wilkins.o
	$(CCC) $(CFLAGS) wilkins.o -o wilkins $(LDFLAGS)

