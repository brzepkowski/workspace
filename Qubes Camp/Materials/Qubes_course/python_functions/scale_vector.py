# -*- coding: utf-8 -*-
"""
Created on Tue Jul 14 12:33:50 2020

@author: Erik
"""

import ipywidgets as widgets
import numpy as np
import matplotlib.pyplot as plt
from IPython.display import display

def scale_vector(angle,scale):
    
    fig = plt.figure(figsize=(6,6))
    ax = fig.add_subplot(1, 1, 1)

    # Move left y-axis and bottim x-axis to centre, passing through (0,0)
    ax.spines['left'].set_position('center')
    ax.spines['bottom'].set_position('center')

    # Eliminate upper and right axes
    ax.spines['right'].set_color('none')
    ax.spines['top'].set_color('none')

    # Show ticks in the left and lower axes only
    ax.xaxis.set_ticks_position('bottom')
    ax.yaxis.set_ticks_position('left')

    #plt.xticks([])
    #plt.yticks([])
    
    major_ticks=np.arange(-6,7,2)
    minor_ticks=np.arange(-6,7)
    
    ax.set_xticks(major_ticks)
    ax.set_xticks(minor_ticks, minor=True)
    ax.set_yticks(major_ticks)
    ax.set_yticks(minor_ticks, minor=True)

    # And a corresponding grid
    ax.grid(which='both')
    ax.set_axisbelow(True)
    
    phi=np.deg2rad(angle)
    x=np.cos(phi)
    y=np.sin(phi)
    
    plt.arrow(0,0,scale*x,scale*y,head_width=0.25,linewidth=3,length_includes_head=True,color='r')
    
    plt.xlim(-6,6)
    plt.ylim(-6,6)
    plt.show()

    
    
angle=widgets.IntSlider(min=0,max=360, step=1,value=45,description='angle',continuous_update=False)
scale=widgets.IntSlider(min=0,max=10,step=1,value=1,description='scale',continuous_update=False)
ui=widgets.VBox([angle,scale])
out=widgets.interactive_output(scale_vector, {'angle':angle,'scale':scale})
out.layout.height = '375px'
display(ui,out)