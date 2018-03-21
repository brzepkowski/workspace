#!/usr/bin/python

# M. P. Polak & P. Scharoch 2018 # NOT TESTED, SHOULD BE OK THOUGH

def fii():
	for i in range (1,N-1):
	    finew[i]=(0.5*(fi[i+1]+fi[i-1])+h*(fi[i+1]-fi[i-1])/(4.*(r[0]+h*i)))
	for i in range (1,N-1):
	    fi[i]=finew[i]*omega+fi[i]*(1.0-omega)
	F=0.0
	for i in range (1,N):
	    F=F+((fi[i]-fi[i-1])**2) *(r[0]+i*h-0.5*h)
	F=F/(2.0*h)
	return F


N=100
fi=N*[0]
finew=N*[0]
r=N*[0]
r[0]=0.0
r[N-1]=10.0
omega=1.0
epsilon=0.000001
fi[0]=2.0
fi[N-1]=10.0
h=(r[N-1]-r[0])/N
Fold=1.0e6
FF=[]
for i in range (1,N-1):
    fi[i]=fi[0]+(fi[N-1]-fi[0])/(r[N-1]-r[0])*i*h + (i-1)*(N-1-i)*0.001

F=fii()

while(abs(F-Fold)>epsilon):
    Fold=F
    FF.append(F)
    F=fii()

ixy=[r[0]]
for i in range (1,N-1):
    ixy.append(r[0]+h*i)