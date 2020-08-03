# -*- coding: utf-8 -*-
"""
Created on Sat Jul 18 12:01:27 2020

@author: Erik
"""

import numpy as np
import matplotlib.cm as cm
import matplotlib.pyplot as plt
import matplotlib.cbook as cbook
from matplotlib.path import Path
from matplotlib.patches import PathPatch, Rectangle
from matplotlib.collections import PatchCollection
import ipywidgets as widgets
from numpy import pi

def interference(d,wavelength):
    #wavelength=0.125
    #d=1
    l=5
    if wavelength<0.5:
        delta=200
    else: delta=100
    y=np.linspace(-3,3,delta)
    theta=np.arctan(y/l)
    intensity=np.cos(pi*d*np.sin(theta)/wavelength)**2
    plt.ylim(-0.1,1.2)
    plt.ylabel('p(x)')
    plt.xlabel('distance from center of slits')
    plt.plot(y,intensity)
    
d=widgets.FloatSlider(min=0,max=4,step=0.25,value=2,description=r'\(a\)',continuous_update=False)
wavelength=widgets.FloatSlider(min=0.1,max=3,value=0.5,step=0.1,description=r'\(\lambda\)',continuous_update=False)
out=widgets.interactive_output(interference,{'d':d,'wavelength':wavelength})
out.layout.height='275px'
display(widgets.VBox([d,wavelength]),out)