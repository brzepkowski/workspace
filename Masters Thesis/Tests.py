import qiskit
from qiskit import QuantumProgram
from qiskit import register, available_backends, get_backend
import Qconfig
import math
from qiskit.tools.visualization import plot_histogram
from qiskit.tools.visualization import latex_drawer, plot_circuit # Needed to generate latex code of circuits
from qiskit.tools.visualization import circuit_drawer
# import QFT
# import Grover
import Shor

###############################################################
# Make a quantum program for the GHZ state.
###############################################################
n = 21
# n = 5
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

# grover(qc, qr, cr, n - 1, 10, 0)
shor_quantum_subroutine(qc, qr, cr, n, 2, 5)

# plot_circuit(qc)
# ------------------- get results -----------------------

qiskit.register(Qconfig.APItoken, Qconfig.config["url"]) # set the APIToken and API url
# latex_string = plot_circuit(qc)
results = qp.execute(["test_circuit"], backend='local_qasm_simulator', shots=1, timeout=90000)
stats = results.get_counts("test_circuit")
print(stats)
print(next(iter(stats.keys())))
# print(continued_fractions(next(iter(stats.keys()))[:5]))

# plot_histogram(stats)
