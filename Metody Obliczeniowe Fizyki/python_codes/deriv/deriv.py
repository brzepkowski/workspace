#!/usr/bin/python
# M. P. Polak & P. Scharoch 2017

def fp3(x,h,fun):
    return (fun(x+h)-fun(x-h))/2./h

def fpp3(x,h,fun):
    return (fun(x+h)+fun(x-h)-2.*fun(x))/h**2
