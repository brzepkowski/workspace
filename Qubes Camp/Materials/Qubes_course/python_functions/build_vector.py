# -*- coding: utf-8 -*-
"""
Created on Tue Jul 14 10:48:58 2020

@author: Erik
"""

import numpy as np
import matplotlib.pyplot as plt
from matplotlib import pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
from matplotlib.patches import FancyArrowPatch
from mpl_toolkits.mplot3d import proj3d
import ipywidgets as widgets
from IPython.display import display

class Arrow3D(FancyArrowPatch):
    def __init__(self, xs, ys, zs, *args, **kwargs):
        FancyArrowPatch.__init__(self, (0,0), (0,0), *args, **kwargs)
        self._verts3d = xs, ys, zs

    def draw(self, renderer):
        xs3d, ys3d, zs3d = self._verts3d
        xs, ys, zs = proj3d.proj_transform(xs3d, ys3d, zs3d, renderer.M)
        self.set_positions((xs[0],ys[0]),(xs[1],ys[1]))
        FancyArrowPatch.draw(self, renderer)

def build_vector(x,y,z):

    fig = plt.figure(figsize=(10,8))
    ax = fig.gca(projection='3d')
    
    #add axes
    length=10
    axes_list=[(length,0,0),(-length,0,0),(0,length,0),(0,-length,0),(0,0,length),(0,0,-length)]
    for i,j,k in axes_list:
        a= Arrow3D([0,i], [0,j], 
                    [0,k], mutation_scale=10, 
                    lw=1, arrowstyle="-|>", color="black")
        ax.add_artist(a)
    
    #add resulting vector
    a= Arrow3D([0,x], [0,y], 
                [0,z], mutation_scale=10, 
                lw=3, arrowstyle="-|>", color="grey")
    ax.add_artist(a)
    
    #add unit vectors
    for i in range(abs(x)):
        if x<0: sign=-1
        else: sign=1
        a = Arrow3D( [i*sign,(i+1)*sign], [0,0], [0,0],mutation_scale=10, 
                lw=3, arrowstyle="-|>", color="r")
        ax.add_artist(a)
    for i in range(abs(y)):
        if y<0: sign=-1
        else: sign=1
        a = Arrow3D([0,0],  
                [i*sign,(i+1)*sign], [0,0], mutation_scale=10, 
                lw=3, arrowstyle="-|>", color="b")
        ax.add_artist(a)
    for i in range(abs(z)):
        if z<0: sign=-1
        else: sign=1
        a = Arrow3D([0,0], [0,0], 
                [i*sign,(i+1)*sign], mutation_scale=10, 
                lw=3, arrowstyle="-|>", color="g")
        ax.add_artist(a)
    
    # make the panes transparent
    #ax.xaxis.set_pane_color((1.0, 1.0, 1.0, 0.0))
    #ax.yaxis.set_pane_color((1.0, 1.0, 1.0, 0.0))
    #ax.zaxis.set_pane_color((1.0, 1.0, 1.0, 0.0))
    # make the grid lines transparent
    #ax.xaxis._axinfo["grid"]['color'] =  (1,1,1,0)
    #ax.yaxis._axinfo["grid"]['color'] =  (1,1,1,0)
    #ax.zaxis._axinfo["grid"]['color'] =  (1,1,1,0)
    #remove ticks
    ax.set_axis_off()
    ax.use_sticky_edges=False
    
    ax.text(9.7,0,0.3,'x',fontsize=14)
    ax.text(0,9.7,0.3,'y',fontsize=14)
    ax.text(0.2,0,9.7,'z',fontsize=14)
    
    ax.set_xlim(-5,5)
    ax.set_ylim(-5,5)
    ax.set_zlim(-5,5)
    ax.grid()
    
    total_arrows=abs(x)+abs(y)+abs(z)
    print("Arrows Used: ",total_arrows)
    if total_arrows<=10:
        print("Arrows Left: ",10-total_arrows)
    else: print("ArrowsLeft: Used {} too many arrows!".format(total_arrows-10))
    print("Vector Length: ",np.round(np.sqrt(x**2+y**2+z**2),2))
    plt.show()
    
    

x=widgets.IntSlider(min=-10,max=10,value=1,description='x',continuous_update=False)
y=widgets.IntSlider(min=-10,max=10,value=1,description='y',continuous_update=False)
z=widgets.IntSlider(min=-10,max=10,value=1,description='z',continuous_update=False)
ui=widgets.VBox([x,y,z])

out=widgets.interactive_output(build_vector,{'x':x,'y':y,'z':z})
out.layout.height = '500px'
display(ui,out)