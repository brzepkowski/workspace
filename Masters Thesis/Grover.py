def get_binary(number, n):
    number_bin = bin(number)[2:]
    l = len(number_bin)
    while l < n:
        number_bin = '0' + number_bin
        l += 1
    return number_bin

def add_controlled_2_NOT_gate(circuit, qr, n, control1, control2, target):
    circuit.ccx(qr[control1], qr[control2], qr[target])
    for k in range(n+1):
        if k != control1 and k != control2 and k != target:
            circuit.iden(qr[k])

def add_controlled_NOT_gate(circuit, qr, n, control, target):
    circuit.cx(qr[control], qr[target])
    for k in range(n+1):
        if k != control and k != target:
            circuit.iden(qr[k])

def add_controlled_Z_gate(circuit, qr, n):
    # Toffoli gates going up (ascending) + CZ gate
    if n == 1:
        circuit.z(qr[n])
    elif n == 2:
        circuit.cz(qr[n-1], qr[n])
    else:
        ancilla = n - 1
        if ((n - 1) % 2 != 0):
            while ancilla > 1:
                add_controlled_2_NOT_gate(circuit, qr, n, ancilla, ancilla - 1, ancilla - 2)
                ancilla -= 2
            add_controlled_NOT_gate(circuit, qr, n, 1, 0)
        else:
            while ancilla > 0:
                add_controlled_2_NOT_gate(circuit, qr, n, ancilla, ancilla - 1, ancilla - 2)
                ancilla -= 2
        circuit.cz(qr[0], qr[n])
    # Toffoli gates going down (descending)
    if n > 2:
        if ((n - 1) % 2 != 0):
            ancilla = 1
            add_controlled_NOT_gate(circuit, qr, n, 1, 0)
            while ancilla <= n - 3:
                add_controlled_2_NOT_gate(circuit, qr, n, ancilla + 2, ancilla + 1, ancilla)
                ancilla += 2
        else:
            ancilla = 0
            while ancilla <= n - 3:
                add_controlled_2_NOT_gate(circuit, qr, n, ancilla + 2, ancilla + 1, ancilla)
                ancilla += 2

# n - number of qubits, m - how many times Uf and Us operators have to be applied
# Note: user needs to provide one ancilla qubit, so the actual number of qubits used in quantum circuit is equal to n + 1
def grover(circuit, qr, cr, n, m, searched_number):
    print("n: ", n)
    print("m: ", m)
    # ----Hadamard gates for uniform superposition----
    for i in range(1, n + 1): # The 0-th qubit is borrowed qubit
        circuit.h(qr[i])
    for j in range(m):
        # ---------------Uf operator---------------
        searched_number_bin = get_binary(searched_number, n)
        for (i, bit) in enumerate(searched_number_bin):
            if bit == '0':
                circuit.x(qr[i + 1])
        add_controlled_Z_gate(circuit, qr, n)
        for (i, bit) in enumerate(searched_number_bin):
            if bit == '0':
                circuit.x(qr[i + 1])
        # ---------------Us operator----------------
        for i in range(1, n + 1): # The 0-th qubit is borrowed qubit
            circuit.h(qr[i])
        for i in range(1, n + 1): # The 0-th qubit is borrowed qubit
            circuit.x(qr[i])
        add_controlled_Z_gate(circuit, qr, n)
        for i in range(1, n + 1): # The 0-th qubit is borrowed qubit
            circuit.x(qr[i])
        for i in range(1, n + 1): # The 0-th qubit is borrowed qubit
            circuit.h(qr[i])
    # -------------Barrier before measurement------------
    circuit.barrier(qr)
    # measure
    for j in range(1, n + 1):
        circuit.measure(qr[j], cr[j])
