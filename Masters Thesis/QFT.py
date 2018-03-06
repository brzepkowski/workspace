from qiskit import QuantumProgram
import Qconfig
import math
# Import basic plotting tools
from qiskit.tools.visualization import plot_histogram

# Quantum Fourier Transform
# qr - quantum register, n - number of qubits
def qft(circuit, qr, n):
    π = math.pi
    λ = 2 * π / float(2**n)
    for i in range(n):
        circuit.h(qr[i])
        for j in range(i + 1, n):
            circuit.cu1(λ, qr[j], qr[i])

# Inverse Quantum Fourier Transform
def inv_qft(circuit, qr, n):
    π = math.pi
    λ = 2 * π / float(2**n)
    for i in reversed(range(n)):
        circuit.h(qr[i]) # Hadamard gate is hermitian
        for j in range(i):
            circuit.cu1(λ, qr[i], qr[j]) # cu1 gate is not hermitian

n = 13
qp = QuantumProgram()
qp.set_api(Qconfig.APItoken, Qconfig.config["url"]) # set the APIToken and API url

# set up registers and program
qr = qp.create_quantum_register('qr', n)
cr = qp.create_classical_register('cr', n)
qc = qp.create_circuit('fourier_transform', [qr], [cr])

qc.x(qr[1])

qft(qc, qr, n)
# inv_qft(qc, qr, n)

#put barrier before measurement
qc.barrier(qr)
# measure
for j in range(n):
    qc.measure(qr[j], cr[j])

# run and get results
# results = qp.execute(["smiley_writer"], backend='ibmqx5', shots=1024) # Real quantum computer
results = qp.execute(["fourier_transform"], backend='ibmqx_qasm_simulator', shots=10, wait=10, timeout=240) # Simulator
stats = results.get_counts("fourier_transform")
print(stats)
plot_histogram(stats)
