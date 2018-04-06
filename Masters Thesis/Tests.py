from qiskit import QuantumProgram
import Qconfig
import math
# Import basic plotting tools
from qiskit.tools.visualization import plot_histogram
import QFT
import Grover

qp = QuantumProgram()
qp.set_api(Qconfig.APItoken, Qconfig.config["url"]) # set the APIToken and API url
# -------------------------------------------------------
n = 6

# set up registers and program
qr = qp.create_quantum_register('qr', n)
cr = qp.create_classical_register('cr', n)
qc = qp.create_circuit('fourier_transform', [qr], [cr])

# QFT.qft(qc, qr, n)
grover_first(qc, qr)

# -------------put barrier before measurement------------
qc.barrier(qr)
# measure
for j in range(n):
    qc.measure(qr[j], cr[j])

# run and get results
# results = qp.execute(["smiley_writer"], backend='ibmqx5', shots=1024) # Real quantum computer
results = qp.execute(["fourier_transform"], backend='ibmqx_qasm_simulator', shots=1024, wait=10, timeout=240) # Simulator
stats = results.get_counts("fourier_transform")
print(stats)
plot_histogram(stats)
