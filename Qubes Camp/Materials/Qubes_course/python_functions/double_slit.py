# -*- coding: utf-8 -*-
"""
Created on Sat Jul 18 01:34:59 2020

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

def double_slit(d,wavelength,y_val):
    #wavelength=0.125
    #d=1
    l=5
    y = np.arange(-3.0, 3.0, 0.001)
    d1=np.sqrt(l**2+(y+d/2)**2)
    d2=np.sqrt(l**2+(y-d/2)**2)
    phi=2*np.pi*(d1-d2)/wavelength
    y=2*np.cos(phi)

    x = np.zeros(1)
    x,y = np.meshgrid(x,y)

    fig=plt.figure(figsize=(10,10))
    ax=plt.subplot(111)
    im = ax.imshow(y, interpolation='bilinear', cmap=cm.gray,
                   origin='lower', extent=[l,l+1, -3, 3])#, vmax=abs(Z).max(), vmin=-abs(Z).max())

    s=0.1
    rect=Rectangle((0,d/2+s),0.25,3-d/2-s)
    rect2=Rectangle((0,-3),0.25,3-d/2-s)
    rect3=Rectangle((0,-d/2+s),0.25,d-2*s)
    collection=PatchCollection([rect,rect2,rect3])
    ax.add_collection(collection)
    
    
    difference=abs(np.sqrt(l**2+(y_val+d/2)**2)-np.sqrt(l**2+(y_val-d/2)**2))
    difference=np.round(difference,2)
    plt.annotate('$\Delta d={}$'.format(difference),(0.5,2.5),fontsize=16)
    
    plt.plot([0,l],[d/2,y_val])
    plt.plot([0,l],[-d/2,y_val])
    plt.xlim(0,l+1)
    plt.ylim(-3,3)
    plt.show()
    
d=widgets.FloatSlider(min=0,max=4,step=0.25,value=2,description=r'\(a\)',continuous_update=False)
wavelength=widgets.FloatSlider(min=0.1,max=1,value=0.5,step=0.1,description=r'\(\lambda\)',continuous_update=False)
y_val=widgets.FloatSlider(min=-3,max=3,step=0.1,value=0,description=r'\(y\,\)',continuous_update=False)
out=widgets.interactive_output(double_slit,{'d':d,'wavelength':wavelength,'y_val':y_val})
out.layout.height='700px'
display(widgets.VBox([d,wavelength,y_val]),out)