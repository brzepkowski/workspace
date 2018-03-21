#!/usr/bin/python
# M. P. Polak & P. Scharoch 2017

from math import sin,cos,sqrt
from fzero import bisek

def feven(E):
    k=sqrt(2.*(Vo+E))
    return sin(k*a/2.)-cos(k*a/2.)*sqrt(-2.*E)/k

def fodd(E):
    k=sqrt(2.*(Vo+E))
    return sin(k*a/2.)+cos(k*a/2.)*k/sqrt(-2.*E)

Vo=10.
a=2.
np=1000

h=Vo/np
dE=Vo/100.
tol=Vo/1000.
E1=-Vo+0.00001*Vo
E2=E1+dE

zero=[]
oddplot=[]
evenplot=[]
energies=[]

for i in range(1,np-1):
    energies.append(-Vo+i*h)
    oddplot.append(fodd(-Vo+i*h))
    evenplot.append(feven(-Vo+i*h))

while (E2<0.):
    if (feven(E1)*feven(E2)<0.):
        zero.append(bisek(E1,E2,tol,feven))
    if (fodd(E1)*fodd(E2)<0.):
    	zero.append(bisek(E1,E2,tol,fodd))
    E1=E2
    E2=E1+dE

print zero
