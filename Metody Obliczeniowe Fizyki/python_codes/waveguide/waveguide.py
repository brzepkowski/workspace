#!/usr/bin/python
# M. P. Polak & P. Scharoch 2017

from methods import bisek, numerow2
from math import sqrt

x0=0.001
xE=1.0
n=1000
h=(xE-x0)/n

def fiE(K):
	x=x0
	yp1=sqrt(x0)
	yp2=sqrt(x0+h)
	xx,psi=numerow2(x0,xE,n,yp1,yp2,k2,K)
	return psi[-1]

def k2(x,K):
	return K*K+1./4./x/x

Kmax=20.

dK=Kmax/100.
tol=Kmax/10000.
K1=0.001*Kmax
K2=K1+dK
zero=[]

while (K2<Kmax):
	    if (fiE(K1)*fiE(K2)<0.):
		zero.append(bisek(K1,K2,tol,fiE))
	    K1=K2
	    K2=K1+dK