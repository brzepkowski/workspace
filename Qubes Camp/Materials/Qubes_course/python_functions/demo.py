import matplotlib.pyplot as plt
import numpy as np
from matplotlib.gridspec import GridSpec
from fractions import Fraction
import ipywidgets as widgets

def demo(phi):
    
    fig=plt.figure(figsize=(10,5))
    gs=GridSpec(3,3,figure=fig,top=0.8)
    a=plt.subplot(gs[0,:])
    b=plt.subplot(gs[1:,:])

    x = np.arange(0,4*np.pi, 0.1)
    y=np.cos(x)
    y2=np.cos(x+phi)
    b.plot(x,y+y2)
    b.hlines(min(y+y2),min(x),max(x),color='r',linestyles='dashed')
    b.hlines(max(y+y2),min(x),max(x),color='r',linestyles='dashed')
    a.plot(x,np.cos(x),color='green')
    a.plot(x,np.cos(x-phi),color='orange')
    plt.ylim(-2, 2)
    
    phi_frac=Fraction(phi/np.pi).limit_denominator(9)
    print("phi: {}*pi/{}".format(phi_frac.numerator,phi_frac.denominator))
    
    plt.show()
    
phi=widgets.FloatSlider(min=0,max=2*np.pi,value=np.pi/8,step=np.pi/8,description=r'\(\phi\)',continuous_update=False)
out=widgets.interactive_output(demo,{'phi':phi})
out.layout.height='325px'
display(phi,out)