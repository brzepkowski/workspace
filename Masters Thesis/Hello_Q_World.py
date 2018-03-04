from qiskit import QuantumProgram
import Qconfig
import matplotlib.pyplot as plt

qp = QuantumProgram()
qp.set_api(Qconfig.APItoken, Qconfig.config["url"]) # set the APIToken and API url

# set up registers and program
qr = qp.create_quantum_register('qr', 16)
cr = qp.create_classical_register('cr', 16)
qc = qp.create_circuit('smiley_writer', [qr], [cr])

# rightmost eight (qu)bits have ')' = 00101001
qc.x(qr[0])
qc.x(qr[3])
qc.x(qr[5])

# second eight (qu)bits have superposition of
# '8' = 00111000
# ';' = 00111011
# these differ only on the rightmost two bits
qc.h(qr[9]) # create superposition on 9
qc.cx(qr[9],qr[8]) # spread it to 8 with a cnot
qc.x(qr[11])
qc.x(qr[12])
qc.x(qr[13])

#put barrier before measurement
qc.barrier(qr)
# measure
for j in range(16):
    qc.measure(qr[j], cr[j])

# run and get results
# results = qp.execute(["smiley_writer"], backend='ibmqx5', shots=1024) # Real quantum computer
results = qp.execute(["smiley_writer"], backend='ibmqx_qasm_simulator', shots=1024) # Simulator
stats = results.get_counts("smiley_writer")

# Printing using PyPlot
plt.rc('font', family='monospace')
for bitString in stats:
    char = chr(int( bitString[0:8] ,2)) # get string of the leftmost 8 bits and convert to an ASCII character
    char += chr(int( bitString[8:16] ,2)) # do the same for string of rightmost 8 bits, and add it to the previous character
    prob = stats[bitString] / 1024 # fraction of shots for which this result occurred
    # create plot with all characters on top of each other with alpha given by how often it turned up in the output
    plt.annotate( char, (0.5,0.5), va="center", ha="center", color = (0,0,0, prob ), size = 300)
    if (prob>0.05): # list prob and char for the dominant results (occurred for more than 5% of shots)
        print(str(prob)+"\t"+char)
plt.axis('off')
plt.show()
