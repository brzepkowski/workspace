# -*- coding: utf-8 -*-
"""
Created on Mon Jun 29 18:51:30 2020

@author: Erik
"""

import matplotlib.pyplot as plt
import numpy as np
import ipywidgets as widgets
from IPython.display import display

def particle_box(n=1,p=False):
    SUB = str.maketrans("0123456789", "₀₁₂₃₄₅₆₇₈₉")
    script_n=str(n).translate(SUB)
    
    x=np.linspace(0,1,100)
    y=np.sqrt(2)*np.sin(n*np.pi*x)
    
    fig=plt.figure(figsize=(7,4))
    plt.axhline(0,color='black',linewidth=1)
    plt.plot(x,y,label=chr(968)+script_n+'(x)')
    if p:
        plt.plot(x,y**2,label='p(x)')
    plt.ylim(-2,2)
    
    plt.legend(prop={'size':16})

    plt.show()

n_slider=widgets.IntSlider(min=1,max=10,continuous_update=False)
p_widg=widgets.Checkbox(value=False,description='Show p(x)')
ui=widgets.HBox([n_slider,p_widg])
out=widgets.interactive_output(particle_box,{'n':n_slider,'p':p_widg})
out.layout.height = '300px'
display(ui,out)