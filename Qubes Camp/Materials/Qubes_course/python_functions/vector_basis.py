# -*- coding: utf-8 -*-
"""
Created on Fri Jun 26 15:55:03 2020

@author: Erik
"""

import matplotlib.pyplot as plt
import ipywidgets as widgets
import numpy as np
from IPython.display import display

def vector_basis(i=1,j=1):
    
    fig = plt.figure(figsize=(10,10))
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
    
    major_ticks=np.arange(-10,11,2)
    minor_ticks=np.arange(-10,11,1)
    
    ax.set_xticks(major_ticks)
    ax.set_xticks(minor_ticks, minor=True)
    ax.set_yticks(major_ticks)
    ax.set_yticks(minor_ticks, minor=True)

    # And a corresponding grid
    ax.grid(which='both')
    ax.set_axisbelow(True)
    
    plt.arrow(0,0,i,0,head_width=0.25,linewidth=3,length_includes_head=True,color='r')
    plt.arrow(i,0,0,j,head_width=0.25,linewidth=3,length_includes_head=True,color='b')
    plt.arrow(0,0,i,j,head_width=0.25,linewidth=3,length_includes_head=True,color='g')
    
    px,py=-3,7
    plt.plot([px], [py], marker='o', markersize=5, color="black")
    plt.annotate('$p$',(px+0.25,py+0.25))
    
    plt.xlim(-10,10)
    plt.ylim(-10,10)
    plt.show()

i=widgets.IntSlider(min=-10,max=10,step=1,value=1,description=r'\(i\)',continuous_update=False)
j=widgets.IntSlider(min=-10,max=10,step=1,value=1,description=r'\(j\)',continuous_update=False)
ui=widgets.VBox([i,j])
out=widgets.interactive_output(vector_basis, {'i':i,'j':j})
out.layout.height = '600px'
display(ui,out)