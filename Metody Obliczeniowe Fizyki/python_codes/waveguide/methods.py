#!/usr/bin/python
# M. P. Polak & P. Scharoch 2017

def numerow2(a,b,ndiv,y0,y1,funk,K):
	h=(b-a)/ndiv
	xp=[a,a+h]
	yp=[y0,y1]
	for i in range(1,ndiv):
		xp.append(xp[i]+h)
		yp.append((2.*yp[i]*(1.-5.*h*h*funk(xp[i],K)/12.)/(1.+h*h*funk(xp[i+1],K)/12.)-yp[i-1]*(1.+h*h*funk(xp[i-1],K)/12.)/(1.+h*h*funk(xp[i+1],K)/12.)))
	return xp,yp

def bisek(xl, xr, tolx, fun):
    if (fun(xr)*fun(xl)>0.):
        return 'no zero'
    while (xr-xl)/2.>tolx:
        x=(xr+xl)/2.
        if fun(xr)*fun(x)<0.:
            xl=x
        else:
            xr=x
    return (xr+xl)/2