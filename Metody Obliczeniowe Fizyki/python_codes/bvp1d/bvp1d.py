from math import exp

def numerow(a,b,ndiv,y0,y1,funk,funs):
	h=(b-a)/ndiv
	xp=[a,a+h]
	yp=[y0,y1]
	for i in range(1,ndiv):
		xp.append(xp[i]+h)
		yp.append((2.*yp[i]*(1.-5.*h*h*funk(xp[i])/12.)-yp[i-1]*(1.+h*h*funk(xp[i-1])/12.)+h*h/12.*(funs(xp[i+1])+10.*funs(xp[i])+funs(xp[i-1])))/(1.+h*h*funk(xp[i+1])/12.))
	return xp,yp

def k2(x):
	return 0.
def s(x):
	return -x*exp(-x)/2.

r0=0.
rk=100.
n=1000

hh=(rk-r0)/n
yp1=0.
yp2=1.-((hh+2.)/2.)*exp(-hh)

xx,psi=numerow(r0,rk,n,yp1,yp2,k2,s)