#!/usr/bin/python

# M. P. Polak & P. Scharoch 2018 # NOT TESTED, NOT SURE IF WORKS PROPERLY

def GaussEbs(Am,Ao,Ap,b,fi,n):
    beta=n*[0.0]
    alfa=n*[0.0]
    beta[-2]=fi[-1]
    for i in range(n-1,0,-1):
        gamma=-1.0/(Ao[i]+Ap[i]*alfa[i])
        alfa[i-1]=Am[i]*gamma
        beta[i-1]=(Ap[i]*beta[i]-b[i])*gamma
    for i in range(0,n-1):
        fi[i+1]=alfa[i]*fi[i]+beta[i]
    return fi

n=10000
fi=n*[0]
fi[0]=20.0
fi[-1]=1.0
xn=1.0
h=xn/n
D=[]
Dprim=[]
x=[0.0]
Am=[0.0]
Ao=[0.0]
Ap=[0.0]
b=[0.0]
for i in range (0,n+1):
    D.append(0.1+10.0*i*h)
    Dprim.append(10.0)
for i in range (1,n):
    Am.append(D[i]+h*Dprim[i]/2.0)
    Ao.append(-2.0*D[i])
    Ap.append(D[i]-h*Dprim[i]/2.0)
    b.append(0.0)
    x.append(i*h)
fi=GaussEbs(Am,Ao,Ap,b,fi,n)