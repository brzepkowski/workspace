# -*- coding: utf-8 -*-
"""
Created on Fri Jul 17 21:20:49 2020

@author: Erik
"""

import matplotlib.pyplot as plt
import numpy as np
from matplotlib.gridspec import GridSpec
from fractions import Fraction
import ipywidgets as widgets

def interference(phi1,phi2):
    
    fig=plt.figure(figsize=(10,5))
    gs=GridSpec(3,3,figure=fig,top=0.8)
    a=plt.subplot(gs[0,:])
    b=plt.subplot(gs[1:,:])

    x = np.arange(0,4*np.pi, 0.1)
    y=np.cos(x-phi1)
    y2=np.cos(x-phi2)
    b.plot(x,y+y2)
    b.hlines(min(y+y2),min(x),max(x),color='r',linestyles='dashed')
    b.hlines(max(y+y2),min(x),max(x),color='r',linestyles='dashed')
    a.plot(x,y,color='green')
    a.plot(x,y2,color='orange')
    plt.ylim(-2, 2)
    
    phi_frac1=Fraction(phi1/np.pi).limit_denominator(9)
    phi_frac2=Fraction(phi2/np.pi).limit_denominator(9)
    print("phi_1: {}*pi/{}".format(phi_frac1.numerator,phi_frac1.denominator))
    print("phi_2: {}*pi/{}".format(phi_frac2.numerator,phi_frac2.denominator))
    
    plt.show()
    
phi1=widgets.FloatSlider(min=0,max=2*np.pi,value=0,step=np.pi/8,description=r'\(\phi_1\)',continuous_update=False)
phi2=widgets.FloatSlider(min=0,max=2*np.pi,value=np.pi/8,step=np.pi/8,description=r'\(\phi_2\)',continuous_update=False)
out=widgets.interactive_output(interference,{'phi1':phi1,'phi2':phi2})
out.layout.height='325px'
display(widgets.VBox([phi1,phi2]),out)