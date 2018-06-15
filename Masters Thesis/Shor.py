# import QFT

# def gcd(a, b):
#     if b == 0:
#         return a
#     else:
#         return gcd(b, a % b)

def Toffoli_gate(circuit, qr, n, control1, control2, target):
    circuit.ccx(qr[control1], qr[control2], qr[target])
    for k in range(n):
        if k != control1 and k != control2 and k != target:
            circuit.iden(qr[k])

def CNOT_gate(circuit, qr, n, control, target):
    circuit.cx(qr[control], qr[target])
    for k in range(n):
        if k != control and k != target:
            circuit.iden(qr[k])

def NOT_gate(circuit, qr, n, target):
    circuit.x(qr[target])
    for k in range(n):
        if k != target:
            circuit.iden(qr[k])

# In fact, we will implement MAJ in reverse order, because we want to subtract instead of add
def MAJ_gate(circuit, qr, n, top_qubit, middle_qubit, bottom_qubit):
    Toffoli_gate(circuit, qr, n, top_qubit, middle_qubit, bottom_qubit)
    CNOT_gate(circuit, qr, n, bottom_qubit, top_qubit)
    CNOT_gate(circuit, qr, n, bottom_qubit, middle_qubit)

# In fact, we will implement UMA in reverse order, because we want to subtract instead of add
def UMA_gate(circuit, qr, n, top_qubit, middle_qubit, bottom_qubit):
    CNOT_gate(circuit, qr, n, top_qubit, middle_qubit)
    CNOT_gate(circuit, qr, n, bottom_qubit, top_qubit)
    Toffoli_gate(circuit, qr, n, top_qubit, middle_qubit, bottom_qubit)

# qr - quantum register. It has to interleave qubits, on which we want to act with the ancilla qubits
# e.g. [qr[0], qr[2], qr[4], ...] - ancilla (borrowed) qubits, [qr[1], qr[3], qr[5], ...] - actual qubits,
# which we want to increment
# n - total number of qubits in register, which we will be incrementing (combined ancilla
# and actual qubits), always will be even, because we are using n/2 ancilla qubits (half in total)
# TODO możliwe że trzeba będzie przekazać też całkowitą liczbę qubitów (nie tylko n),
# TODO żeby dodać IDENTITY gates na całej szerokości obwodu
def incrementer(circuit, qr, n):
    # Initial gates (CNOTs and NOTs) before first subtraction widget
    for i in range(1, n - 2, 2):
        CNOT_gate(circuit, qr, n, 0, i)
    for i in range(2, n - 1, 2):
        NOT_gate(circuit, qr, n, i)
    NOT_gate(circuit, qr, n, n - 1)
    # First Subtraction Widget
    for i in range(0, n - 3, 2):
        UMA_gate(circuit, qr, n, i, i + 1, i + 2)
    CNOT_gate(circuit, qr, n, n - 2, n - 1)
    for i in reversed(range(0, n - 3, 2)):
        MAJ_gate(circuit, qr, n, i, i + 1, i + 2)
    # Binary negation of one of the number, which we are subtracting (written on all ancilla qubits except the 0-th one)
    for i in range(2, n - 1, 2):
        NOT_gate(circuit, qr, n, i)
    # Second Subtraction Widget
    for i in range(0, n - 3, 2):
        UMA_gate(circuit, qr, n, i, i + 1, i + 2)
    CNOT_gate(circuit, qr, n, n - 2, n - 1)
    for i in reversed(range(0, n - 3, 2)):
        MAJ_gate(circuit, qr, n, i, i + 1, i + 2)
    # Last CNOT gates controlled on the 0-th qubit
    for i in range(1, n - 2, 2):
        CNOT_gate(circuit, qr, n, 0, i)

# n - number of qubits, m - how many times Uf and Us operators have to be applied
# Note: user needs to provide one ancilla qubit, so the actual number of qubits used in quantum circuit is equal to n + 1
def shor(circuit, qr, cr, n):
    # NOT_gate(circuit, qr, n, 1)
    # NOT_gate(circuit, qr, n, 3)
    # NOT_gate(circuit, qr, n, 5)
    incrementer(circuit, qr, n)
    incrementer(circuit, qr, n)
    incrementer(circuit, qr, n)
    incrementer(circuit, qr, n)
    # -------------Barrier before measurement------------
    circuit.barrier(qr)
    # measure
    for j in range(0, n):
        circuit.measure(qr[j], cr[j])
