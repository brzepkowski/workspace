# -*- coding: utf-8 -*-
"""
Created on Fri Jul 17 21:37:14 2020

@author: Erik
"""

import matplotlib.pyplot as plt
import numpy as np
from fractions import Fraction
import ipywidgets as widgets

def wave(A,phi,wavelength):
    
    fig=plt.figure(figsize=(10,4))
    x=np.linspace(-np.pi*2,np.pi*2,400)
    y=A*np.cos(2*np.pi*x/wavelength-phi)
    plt.plot(x,y)
    plt.ylim(-3, 3)
    plt.hlines(0,-np.pi*2,np.pi*2,linewidth=1)
    plt.vlines(0,-3,3,linewidth=1)
    plt.margins(0,0)
    plt.show()
    
A=widgets.FloatSlider(min=0,max=3,value=1.5,step=0.25,description=r'\(A\)',continuous_update=False)
phi=widgets.FloatSlider(min=0,max=2*np.pi,step=np.pi/8,description=r'\(\phi\)',continuous_update=False)
wavelength=widgets.FloatSlider(min=np.pi/4,max=4*np.pi,value=np.pi,step=np.pi/8,description=r'\(\lambda\)',continuous_update=False)
out=widgets.interactive_output(wave,{'A':A,'phi':phi,'wavelength':wavelength})
out.layout.height='325px'
display(widgets.VBox([A,phi,wavelength]),out)