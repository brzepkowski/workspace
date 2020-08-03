import matplotlib.pyplot as plt
import ipywidgets as widgets
import numpy as np
from IPython.display import display
from fractions import Fraction

def complex_unit_circle(phi_1,phi_2):

    t = np.arange(0, 2*np.pi+np.pi/32, np.pi/32)
    x=np.cos(t)
    y=np.sin(t)

    fig = plt.figure(figsize=(4,4))
    ax = fig.add_subplot(1,1,1)
    
    # Move left y-axis and bottim x-axis to centre, passing through (0,0)
    ax.spines['left'].set_position('center')
    ax.spines['bottom'].set_position('center')
    
    # Eliminate upper and right axes
    ax.spines['right'].set_color('none')
    ax.spines['top'].set_color('none')
    
    # Show ticks in the left and lower axes only
    ax.xaxis.set_ticks_position('bottom')
    ax.yaxis.set_ticks_position('left')
    
    plt.xticks([])
    plt.yticks([])
    
    plt.annotate('$1$',(1,0),size=20,color='r')
    plt.annotate('$i$',(0,1),size=20,color='r')
    plt.annotate('$-1$',(-1,0),size=20,color='r')
    plt.annotate('$-i$',(0,-1),size=20,color='r')
    
    phi1=Fraction(phi_1/np.pi).limit_denominator(9)
    phi2=Fraction(phi_2/np.pi).limit_denominator(9)
    
    a=abs(phi1-phi2)
    b=abs(phi1+Fraction(2,1)-phi2)
    c=abs(phi2+Fraction(2,1)-phi1)
    diff=min(a,b,c)
    
    print("phi 1: {}*pi/{}".format(phi1.numerator,phi1.denominator))
    print("phi 2: {}*pi/{}".format(phi2.numerator,phi2.denominator))
    print("difference: {}*pi/{}".format(diff.numerator,diff.denominator))
    
    plt.arrow(0,0,np.cos(phi_1),np.sin(phi_1),head_width=0.05,length_includes_head=True)
    plt.arrow(0,0,np.cos(phi_2),np.sin(phi_2),head_width=0.05,length_includes_head=True)
    
    plt.plot(x,y)
    plt.show()
    
phi_1=widgets.FloatSlider(min=0.0,max=2*np.pi,step=np.pi/8,value=0.0,description=r'\(\phi_1\)',continuous_update=False)
phi_2=widgets.FloatSlider(min=0.0,max=2*np.pi,step=np.pi/8,value=0.0,description=r'\(\phi_2\)',continuous_update=False)
ui=widgets.VBox([phi_1,phi_2])
out=widgets.interactive_output(complex_unit_circle, {'phi_1':phi_1,'phi_2':phi_2})
out.layout.height = '325px'
display(ui,out)