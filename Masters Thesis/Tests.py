from qiskit import QuantumProgram
from qiskit import register, available_backends, get_backend
import Qconfig
import math
from qiskit.tools.visualization import plot_histogram
from qiskit.tools.visualization import latex_drawer, plot_circuit # Needed to generate latex code of circuits
import QFT
import Grover

###############################################################
# Make a quantum program for the GHZ state.
###############################################################
n = 6
QPS_SPECS = {
    "circuits": [
        {
            "name": "test_circuit",
            "quantum_registers": [{
                "name": "qr",
                "size": n
            }],
            "classical_registers": [{
                "name": "cr",
                "size": n
            }]
        }
    ]
}

backends = available_backends()
print(backends)

qp = QuantumProgram(specs=QPS_SPECS)
# -------------------------------------------------------

# set up registers and program
qr = qp.get_quantum_register('qr')
cr = qp.get_classical_register('cr')
qc = qp.get_circuit('test_circuit')

# QFT.qft(qc, qr, n)
grover_first(qc, qr)

# -------------put barrier before measurement------------
qc.barrier(qr)
# measure
for j in range(n):
    qc.measure(qr[j], cr[j])

# ------------------- get results -----------------------

qiskit.register(Qconfig.APItoken, Qconfig.config["url"]) # set the APIToken and API url
# latex_string = plot_circuit(qc)
results = qp.execute(["test_circuit"], backend='local_qasm_simulator', shots=1024, timeout=600)
stats = results.get_counts("test_circuit")
print(stats)
plot_histogram(stats)
