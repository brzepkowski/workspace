import matplotlib.pyplot as plt
import numpy as np
import ipywidgets as widgets
from IPython.display import display

def modified_firework(beam_1,beam_2,firework):
    
    if not firework:
        alpha1=np.sqrt(beam_1/100)
        alpha2=np.sqrt(beam_2/100)
        beta1=np.sqrt(1-beam_1/100)
        beta2=np.sqrt(1-beam_2/100)
        
        #multiply matrices
        Q1=np.array([[alpha1,beta1],[beta1,-alpha1]])
        Q2=np.array([[alpha2,beta2],[beta2,-alpha2]])
        state=np.array([[1],[0]])
        Q=Q2.dot(Q1)
        result=Q.dot(state)
        
        #calculate probabilities
        detect_A=np.round((result[0][0])**2,2)
        detect_B=np.round((result[1][0])**2,2)
        fig=plt.figure(figsize=(6.35,5))
        plt.bar(['A','B',''],[detect_A,detect_B,0],color=['royalblue','royalblue','royalblue'])
        plt.ylim(0,1)
        plt.title('Measurement Probabilities')
        plt.show()
    
    else:
        alpha1=np.sqrt(beam_1/100)
        alpha2=np.sqrt(beam_2/100)
        beta1=np.sqrt(1-beam_1/100)
        beta2=np.sqrt(1-beam_2/100)
        
        #multiply matrices
        Q1=np.array([[alpha1,beta1],[beta1,-alpha1]])
        state=np.array([[1],[0]])
        
        result=Q1.dot(state)
        firework=result[1][0]**2
        
        
        Q2=np.array([[alpha2,beta2],[beta2,-alpha2]])
        result2=Q2.dot(state)
        
        #calculate probabilities
        detect_A=np.round((result2[0][0])**2,2)*(1-firework)
        detect_B=np.round((result2[1][0])**2,2)*(1-firework)
        
        #calculate base case-----------------------------
        state_base=np.array([[1],[0]])
        Q=Q2.dot(Q1)
        result_base=Q.dot(state)

        #calculate probabilities
        detect_A_base=np.round((result_base[0][0])**2,2)
        detect_B_base=np.round((result_base[1][0])**2,2)
        
        
        #compare----------------------------
        difference_A=detect_A-detect_A_base
        difference_B=detect_B-detect_B_base
        detection_score=abs(difference_B)+abs(difference_A)-2*firework
        
        scores=[difference_A,difference_B,detection_score]
        colors=['royalblue','royalblue']
        
        if detection_score<0:
            colors.append('crimson')
        else:colors.append('forestgreen')
        
        fig=plt.figure(figsize=(14,5))
        a=plt.subplot(121)
        b=plt.subplot(122)
        a.bar(['A','B','firework',],[detect_A,detect_B,firework],color=['royalblue','royalblue','crimson'])
        a.set_ylim(0,1)
        b.set_ylim(-0.9,0.9)
        b.bar(['difference A','difference B','detection score'],scores,color=colors)
        a.set_title('Measurement Probabilities')
        b.set_title('Comparison to No-Firework Case')
        plt.show()
        
beam_1=widgets.IntSlider(min=0,max=100,step=10,value=50,
                         description='Beam-splitter 1',
                         style={'description_width': 'initial'},
                        continuous_update=False)
beam_2=widgets.IntSlider(min=0,max=100,step=10,value=50,
                         description='Beam-splitter 2',
                         style={'description_width': 'initial'},
                        continuous_update=False)
firework=widgets.Checkbox(value=False,description='Firework Present')
l=widgets.VBox([beam_1,beam_2])
ui=widgets.HBox([l,firework])
out=widgets.interactive_output(modified_firework,{'beam_1':beam_1,'beam_2':beam_2,'firework':firework})
out.layout.height='325px'
display(ui,out)