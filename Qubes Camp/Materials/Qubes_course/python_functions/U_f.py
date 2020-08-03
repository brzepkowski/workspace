from qiskit import QuantumCircuit, QuantumRegister
import numpy as np

def U_f(circ_global, qubits):  
    #the first argument is a circuit to which we want U_f to be applied,
    #the second argument is a list of qubit to which U_f is to be applied.
    
    ##### OR gate #####
    
    n=4 
    m=n
    out_qubit = n+m-1
    
    # Build a sub-circuit
    circ = QuantumCircuit(2, name='U_f') 
    
    circ.cx(0,1)   # a simple circuit implementing the function f such that f(0) = 1 and f(1) = 0.

    sub_inst = circ.to_instruction()
    
    q = circ_global.qubits
    circ_global.append(sub_inst, [q[qubits[i]] for i in range(len(qubits))])



