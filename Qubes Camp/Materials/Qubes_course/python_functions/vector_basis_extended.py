# -*- coding: utf-8 -*-
"""
Created on Tue Jul 14 11:07:33 2020

@author: Erik
"""

import matplotlib.pyplot as plt
import numpy as np

def vector_basis_extended(a1=1,a2=1,b1=1,b2=-1,alpha=0,beta=0):
    
    fig = plt.figure(figsize=(10,10))
    ax = fig.add_subplot(1, 1, 1)

    plt.xlim(-10,10)
    plt.ylim(-10,10)
    
    # Move left y-axis and bottim x-axis to centre, passing through (0,0)
    ax.spines['left'].set_position('center')
    ax.spines['bottom'].set_position('center')
    # Eliminate upper and right axes
    ax.spines['right'].set_color('none')
    ax.spines['top'].set_color('none')
    # Show ticks in the left and lower axes only
    ax.xaxis.set_ticks_position('bottom')
    ax.yaxis.set_ticks_position('left')

    major_ticks=np.arange(-10,11,2)
    minor_ticks=np.arange(-10,11,1)
    ax.set_xticks(major_ticks)
    ax.set_xticks(minor_ticks, minor=True)
    ax.set_yticks(major_ticks)
    ax.set_yticks(minor_ticks, minor=True)
    ax.set_axisbelow(True)
    
    x_i=[a1,b1]
    y_i=[a2,b2]
    slopes=[None,None]
    inverse_slopes=[0,0]
    for k in [0,1]:
        if abs(x_i[k])>0:
            if abs(y_i[k])>0:
                slopes[k]=y_i[k]/x_i[k]
                inverse_slopes[k]=x_i[k]/y_i[k]
            else: slopes[k]='horizontal'
        elif abs(x_i[k])==0 and abs(y_i[k])>0:
            slopes[k]='vertical'
    
    max_inverse_slope=max([abs(i) for i in inverse_slopes])
    size=10*max_inverse_slope+10
    x=np.arange(-size,size,1)
    
    for i in x:
        start_x=i
        start_y=0
        #prepare functions
        y=[0,0]
        for k in [0,1]:
            if slopes[k] and not type(slopes[k])==str:
                y[k]=slopes[k]*(x-start_x)+start_y
        
        #plot lines
        if not i%2:
            for k in [0,1]:
                if slopes[k]=='horizontal':
                    plt.axhline(i,color='gray')
                elif slopes[k]=='vertical':
                    plt.axvline(i,color='gray')
                elif slopes[k]:
                    plt.plot(x,y[k],color='gray')
    
    #basis vectors
    x1=a1
    x2=b1
    y1=a2
    y2=b2
    norm1=x1**2+y1**2
    norm2=x2**2+y2**2
    
    if norm1 !=0:
        plt.arrow(0,0,x1/norm1,y1/norm1,head_width=0.25,linewidth=3,length_includes_head=True,color='r')
        vector1_x=x1*alpha
        vector1_y=y1*alpha
        plt.arrow(0,0,vector1_x,vector1_y,head_width=0.25,linewidth=2,length_includes_head=True,color='r')
        
        if norm2 !=0:
            plt.arrow(0,0,x2/norm2,y2/norm2,head_width=0.25,linewidth=3,length_includes_head=True,color='b')
            vector2_x=x2*beta
            vector2_y=y2*beta
            plt.arrow(vector1_x,vector1_y,vector2_x,vector2_y,head_width=0.25,linewidth=2,length_includes_head=True,color='b')
            plt.arrow(0,0,vector1_x+vector2_x,vector1_y+vector2_y,head_width=0.25,linewidth=2,length_includes_head=True,color='g')
    
    ax.set_axisbelow(True)
    plt.show()

#from python_functions.vector_basis_extended import vector_basis_extended
import ipywidgets as widgets
a1=widgets.IntSlider(min=-3,max=3,value=1,description=r'\(a_1\)',continuous_update=False)
a2=widgets.IntSlider(min=-3,max=3,description=r'\(a_2\)',continuous_update=False)
b1=widgets.IntSlider(min=-3,max=3,description=r'\(b_1\)',continuous_update=False)
b2=widgets.IntSlider(min=-3,max=3,value=1,
                     description=r'\(b_2\)',continuous_update=False)
alpha=widgets.IntSlider(min=-10,max=10,step=1,description=r'\(\alpha\,\)',continuous_update=False)
beta=widgets.IntSlider(min=-10,max=10,step=1,description=r'\(\beta\, \)',continuous_update=False)
a=widgets.VBox([a1,a2])
b=widgets.VBox([b1,b2])
c=widgets.VBox([alpha,beta])
ui=widgets.HBox([a,b,c])
out=widgets.interactive_output(vector_basis_extended,{'a1': a1,'a2':a2,'b1':b1,'b2':b2,'alpha':alpha,'beta':beta})
display(ui,out)