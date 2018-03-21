#!/usr/bin/python
# M. P. Polak & P. Scharoch 2017

from math import cos,sin,pi,sqrt
from quadrat import simpson

def real_amp(x):
	r=sqrt(d*d+(y-x)**2)
	return ampl*cos(k*r)/(sqrt(r))

def imag_amp(x):
	r=(d*d+(y-x)**2)**(1./2.)
	return ampl*sin(k*r)/(sqrt(r))

ndiv=100
ampl=1.
lmbd=1.
k=2.*pi/lmbd
d=50.
a=2.
yl=-50.
yr=50.
ny=100

hy=(yr-yl)/ny

yarr=[]
iarr=[]

for iy in range(1,ny):
	y=yl+(iy-1.)*hy
	i=simpson(-a/2.,a/2.,ndiv,real_amp)**2+simpson(-a/2,a/2,ndiv,imag_amp)**2
	yarr.append(y)
	iarr.append(i)