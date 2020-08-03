# -*- coding: utf-8 -*-
"""
Created on Mon Jun 29 16:48:24 2020

@author: Erik
"""

import ipywidgets as widgets
from IPython.display import display
import matplotlib.pyplot as plt
import random

outcomes=[widgets.IntText(value=0,continuous_update=False) for _ in range(6)]

def update_data(num_rolls=1):
    weights=[7,10,7,4,2,1]
    indices=range(6)
    data=[0]*6
    for _ in range(num_rolls):
        i=random.choices(indices,weights)[0]
        data[i]+=1

    for j in range(6):
        outcomes[j].value=outcomes[j].value+data[j]

def reset_data():
    for o in outcomes:
        o.value=0
        
def testy(o1,o2,o3,o4,o5,o6):
    fig=plt.figure(figsize=(10,5))
    a=plt.subplot(121)
    b=plt.subplot(122)
    b.set_ylim(0,1)
    o=[o1,o2,o3,o4,o5,o6]
    a.bar(['1','2','3','4','5','6'],o)
    if sum(o)>0:
        b.bar(['1','2','3','4','5','6'],[i/sum(o) for i in o],color=['y' for _ in range(6)])

idict={'o{}'.format(i+1):outcomes[i] for i in range(6)}
out=widgets.interactive_output(testy,idict)

button = widgets.Button(description="Roll Once")
button2=widgets.Button(description="Roll Ten Times")
button3=widgets.Button(description="Roll A Hundred Times")
button4=widgets.Button(description="Reset")
output = widgets.Output()
l=widgets.VBox([button,button2,button3])
r=widgets.VBox([button4])
ui=widgets.HBox([l,r])
display(ui, out, output)
def on_button_clicked(b):
    with output:
        update_data()
def on_button2_clicked(b):
    with output:
        update_data(10)
def on_button3_clicked(b):
    with output:
        update_data(100)
def on_button4_clicked(b):
    with output:
        reset_data()
button.on_click(on_button_clicked)
button2.on_click(on_button2_clicked)
button3.on_click(on_button3_clicked)
button4.on_click(on_button4_clicked)