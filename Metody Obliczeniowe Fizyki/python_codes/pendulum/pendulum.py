#!/usr/bin/python
# M. P. Polak & P. Scharoch 2017

from math import sin,cos,sqrt
from ivp2d import rk4_2d

def dfalfa(t,alfa,omega):
    return omega

def dfomega(t,alfa,omega):
    return -(g/l)*sin(alfa)

g=9.81

frEtop=0.1
tE=20.
np=1000

l=1.
m=1.
E0=frEtop*m*2.*l*g
t0=0.
alfa0=0.
omega0=sqrt(2.*E0/m)/l

h=(tE-t0)/np

t,alfa,omega=rk4_2d(t0,tE,np,alfa0,omega0,dfalfa,dfomega)
Etot=[0.5*m*l*l*om**2+m*g*l*(1.-cos(alf)) for om,alf in zip(omega,alfa)] # this is a clever way of iterating over both omega and alfa and using them to create Etot, instead of using a loop.
