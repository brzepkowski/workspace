#!/usr/bin/python
# M. P. Polak & P. Scharoch 2017

def rk4_2d(a,b,n,y1_0,y2_0,fun1,fun2):
    h=(b-a)/n
    yp_1=[y1_0]
    yp_2=[y2_0]
    xp=[a]
    for i in range(0,n):
        k1_1=h*fun1(xp[i],yp_1[i],yp_2[i])
        k1_2=h*fun2(xp[i],yp_1[i],yp_2[i])

        k2_1=h*fun1(xp[i]+h/2.,yp_1[i]+k1_1/2.,yp_2[i]+k1_2/2.)
        k2_2=h*fun2(xp[i]+h/2.,yp_1[i]+k1_1/2.,yp_2[i]+k1_2/2.)

        k3_1=h*fun1(xp[i]+h/2.,yp_1[i]+k2_1/2.,yp_2[i]+k2_2/2.)
        k3_2=h*fun2(xp[i]+h/2.,yp_1[i]+k2_1/2.,yp_2[i]+k2_2/2.)

        k4_1=h*fun1(xp[i]+h,yp_1[i]+k3_1,yp_2[i]+k3_2)
        k4_2=h*fun2(xp[i]+h,yp_1[i]+k3_1,yp_2[i]+k3_2)

        yp_1.append(yp_1[i]+(k1_1+2.*k2_1+2.*k3_1+k4_1)/6.)
        yp_2.append(yp_2[i]+(k1_2+2.*k2_2+2.*k3_2+k4_2)/6.)
        xp.append(xp[i]+h)
    return xp,yp_1,yp_2
