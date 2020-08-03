# -*- coding: utf-8 -*-
"""
Created on Tue Jun 30 15:38:45 2020

@author: Erik
"""

import matplotlib.pyplot as plt
import numpy as np
import matplotlib.gridspec as gridspec
import ipywidgets as widgets
from IPython.display import display

def dot_product(v_x,v_y,w_x,w_y):
    
    fig = plt.figure(figsize=(9,6))
    gs=gridspec.GridSpec(2,3,figure=fig,top=0.8)
    ax=fig.add_subplot(gs[:,0:2])
    b=fig.add_subplot(gs[:,2])
    
    
    # Move left y-axis and bottim x-axis to centre, passing through (0,0)
    ax.spines['left'].set_position('center')
    ax.spines['bottom'].set_position('center')
    
    # Eliminate upper and right axes
    ax.spines['right'].set_color('none')
    ax.spines['top'].set_color('none')
    
    # Show ticks in the left and lower axes only
    ax.xaxis.set_ticks_position('bottom')
    ax.yaxis.set_ticks_position('left')
    ax.set_axisbelow(True)
    
    ax.set_xlim(-5,5)
    ax.set_ylim(-5,5)
    ax.grid(linewidth=1,color='gray')

    ax.arrow(0,0,v_x,v_y,lw=3,head_width=0.25,length_includes_head=True,color='red')
    ax.arrow(0,0,w_x,w_y,lw=3,head_width=0.25,length_includes_head=True,color='blue')
    
    def angle(x,y):
        if x==0:
            if y>0: return 90
            elif y==0: return 361
            else: return 270
        elif y==0:
            if x>0: return 0
            else: return 180
        else:
            return np.degrees(np.arctan(y/x))
    
    v=angle(v_x,v_y)
    w=angle(w_x,w_y)
    norm_v=np.sqrt(v_x**2+v_y**2)
    norm_w=np.sqrt(w_x**2+w_y**2)
    
    theta=""
    vdotw=""
    if v<361 and w<361:
        dif1=abs(v-w)
        dif4=abs(w-v)
        dif2=abs(360-w+v)
        dif3=abs(360-v+w)
        theta=np.round(min(dif1,dif2,dif3,dif4),1)
        vdotw=np.round(np.cos(np.radians(theta))*norm_v*norm_w,1)
    
    b.bar([u"v\u00B7w"],[vdotw],width=[0.5])
    if norm_v!=0 and norm_w!=0:
        b.set_ylim(-norm_v*norm_w,norm_v*norm_w)

    print("theta= ",theta)
    print(u"v\u00B7w= ",vdotw)
    plt.show()
    


v_x=widgets.IntSlider(min=-5,max=5,continuous_update=False,description="v_x")
v_y=widgets.IntSlider(min=-5,max=5,continuous_update=False,description="v_y")
w_x=widgets.IntSlider(min=-5,max=5,continuous_update=False,description="w_x")
w_y=widgets.IntSlider(min=-5,max=5,continuous_update=False,description="w_y")
v=widgets.VBox([v_x,v_y])
w=widgets.VBox([w_x,w_y])
ui=widgets.HBox([v,w])

out=widgets.interactive_output(dot_product,{'v_x':v_x,'v_y':v_y,'w_x':w_x,'w_y':w_y})

display(ui,out)