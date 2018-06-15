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

# We are using this name to indicate, that initially it was just a CNOT gate,
# but controlled-incrementer needs to add another control qubit to each gate,
# so in fact this gate becomes a Toffoli gate.
# We also leave the notation n, but it will now keep the information about how
# many qubits a circuit has in total (not only how big the register, on which we
# are performing incrementation, is)
# m - total number of qubits in the whole circuit (not only in the incremented register)
def extended_CNOT_gate(circuit, qr, m, control, target, external_control):
    circuit.ccx(qr[control], qr[external_control], qr[target])
    for k in range(m):
        if k != control and k != external_control and k != target:
            circuit.iden(qr[k])

def extended_NOT_gate(circuit, qr, m, target, external_control):
    circuit.cx(qr[external_control], qr[target])
    for k in range(m):
        if k != external_control and k != target:
            circuit.iden(qr[k])

def extended_Toffoli_gate(circuit, qr, m, n, control1, control2, target, external_control):
    Toffoli_gate(circuit, qr, m, control2, n - 1, target)
    Toffoli_gate(circuit, qr, m, control1, external_control, n - 1)
    Toffoli_gate(circuit, qr, m, control2, n - 1, target)
    Toffoli_gate(circuit, qr, m, control1, external_control, n - 1)

def extended_MAJ_gate(circuit, qr, m, n, top_qubit, middle_qubit, bottom_qubit, external_control):
    extended_Toffoli_gate(circuit, qr, m, n, top_qubit, middle_qubit, bottom_qubit, external_control)
    extended_CNOT_gate(circuit, qr, m, bottom_qubit, top_qubit, external_control)
    extended_CNOT_gate(circuit, qr, m, bottom_qubit, middle_qubit, external_control)

def extended_UMA_gate(circuit, qr, m, n, top_qubit, middle_qubit, bottom_qubit, external_control):
    extended_CNOT_gate(circuit, qr, m, top_qubit, middle_qubit, external_control)
    extended_CNOT_gate(circuit, qr, m, bottom_qubit, top_qubit, external_control)
    extended_Toffoli_gate(circuit, qr, m, n, top_qubit, middle_qubit, bottom_qubit, external_control)

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

def controlled_incrementer(circuit, qr, m, n, external_control):
    # Initial gates (CNOTs and NOTs) before first subtraction widget
    for i in range(1, n - 2, 2):
        extended_CNOT_gate(circuit, qr, m, 0, i, external_control)
    for i in range(2, n - 1, 2):
        extended_NOT_gate(circuit, qr, m, i, external_control)
    extended_NOT_gate(circuit, qr, m, n - 1, external_control)
    # First Subtraction Widget
    for i in range(0, n - 3, 2):
        extended_UMA_gate(circuit, qr, m, n, i, i + 1, i + 2, external_control)
    extended_CNOT_gate(circuit, qr, m, n - 2, n - 1, external_control)
    for i in reversed(range(0, n - 3, 2)):
        extended_MAJ_gate(circuit, qr, m, n, i, i + 1, i + 2, external_control)
    # Binary negation of one of the number, which we are subtracting (written on all ancilla qubits except the 0-th one)
    for i in range(2, n - 1, 2):
        extended_NOT_gate(circuit, qr, m, i, external_control)
    # Second Subtraction Widget
    for i in range(0, n - 3, 2):
        extended_UMA_gate(circuit, qr, m, n, i, i + 1, i + 2, external_control)
    extended_CNOT_gate(circuit, qr, m, n - 2, n - 1, external_control)
    for i in reversed(range(0, n - 3, 2)):
        extended_MAJ_gate(circuit, qr, m, n, i, i + 1, i + 2, external_control)
    # Last CNOT gates controlled on the 0-th qubit
    for i in range(1, n - 2, 2):
        extended_CNOT_gate(circuit, qr, m, 0, i, external_control)

# n - number of qubits in register, which will be incremented
# m - total number of quibts used in the circuit
def shor(circuit, qr, cr, n, m):
    NOT_gate(circuit, qr, m, m - 1)
    # incrementer(circuit, qr, n)
    controlled_incrementer(circuit, qr, m, n, m - 1)
    controlled_incrementer(circuit, qr, m, n, m - 1)
    controlled_incrementer(circuit, qr, m, n, m - 1)
    controlled_incrementer(circuit, qr, m, n, m - 1)
    controlled_incrementer(circuit, qr, m, n, m - 1)
    controlled_incrementer(circuit, qr, m, n, m - 1)
    controlled_incrementer(circuit, qr, m, n, m - 1)
    controlled_incrementer(circuit, qr, m, n, m - 1)
    # -------------Barrier before measurement------------
    circuit.barrier(qr)
    # measure
    for j in range(0, m):
        circuit.measure(qr[j], cr[j])
