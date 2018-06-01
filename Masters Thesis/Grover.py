def grover_first(circuit, qr):
    # Początkowe bramki Hadamarda
    circuit.h(qr[0])
    circuit.h(qr[1])
    circuit.h(qr[2])
    circuit.h(qr[3])
    # Uᵥ - wyrocznia
    circuit.x(qr[1])
    circuit.ccx(qr[1], qr[2], qr[4])
    circuit.ccx(qr[3], qr[4], qr[5])
    circuit.cz(qr[5], qr[0])
    circuit.ccx(qr[3], qr[4], qr[5]) # "Odkręcanie"
    circuit.ccx(qr[1], qr[2], qr[4])
    circuit.x(qr[1])
    # -Uₛ - dyfuzja
    circuit.h(qr[0])
    circuit.h(qr[1])
    circuit.h(qr[2])
    circuit.h(qr[3])
    circuit.x(qr[0])
    circuit.x(qr[1])
    circuit.x(qr[2])
    circuit.x(qr[3])
    circuit.ccx(qr[1], qr[2], qr[4])
    circuit.ccx(qr[3], qr[4], qr[5])
    circuit.cz(qr[5], qr[0])
    circuit.ccx(qr[3], qr[4], qr[5]) # "Odkręcanie"
    circuit.ccx(qr[1], qr[2], qr[4])
    circuit.x(qr[0])
    circuit.x(qr[1])
    circuit.x(qr[2])
    circuit.x(qr[3])
    circuit.h(qr[0])
    circuit.h(qr[1])
    circuit.h(qr[2])
    circuit.h(qr[3])
    # Uᵥ - wyrocznia
    circuit.x(qr[1])
    circuit.ccx(qr[1], qr[2], qr[4])
    circuit.ccx(qr[3], qr[4], qr[5])
    circuit.cz(qr[5], qr[0])
    circuit.ccx(qr[3], qr[4], qr[5]) # "Odkręcanie"
    circuit.ccx(qr[1], qr[2], qr[4])
    circuit.x(qr[1])
    # -Uₛ - dyfuzja
    circuit.h(qr[0])
    circuit.h(qr[1])
    circuit.h(qr[2])
    circuit.h(qr[3])
    circuit.x(qr[0])
    circuit.x(qr[1])
    circuit.x(qr[2])
    circuit.x(qr[3])
    circuit.ccx(qr[1], qr[2], qr[4])
    circuit.ccx(qr[3], qr[4], qr[5])
    circuit.cz(qr[5], qr[0])
    circuit.ccx(qr[3], qr[4], qr[5]) # "Odkręcanie"
    circuit.ccx(qr[1], qr[2], qr[4])
    circuit.x(qr[0])
    circuit.x(qr[1])
    circuit.x(qr[2])
    circuit.x(qr[3])
    circuit.h(qr[0])
    circuit.h(qr[1])
    circuit.h(qr[2])
    circuit.h(qr[3])

# n - number of qubits, m - how many times Uf and Us operators have to be applied
# Note: user has to have in fact one ancilla qubit, so the actual number of qubits used is equal to n + 1
def grover(circuit, qr, n, m, searched_number):
    # ----Hadamard gates for uniform superposition----
    for i in range(1, n + 1): # The 0-th qubit is borrowed qubit
        circuit.h(qr[i])
    for j in range(m):
        # ---------------Uf operator---------------
        # X gates
        searched_number_bin = bin(searched_number)[2:]
        for (i, bit) in enumerate(searched_number_bin):
            if bit == '0':
                circuit.x(qr[n - i])
        # C^{n-1} Z gate
        # Toffoli gates going up (ascending)
        if n == 1:
            circuit.z(qr[n])
        elif n == 2:
            circuit.cz(qr[n-1], qr[n])
        else:
            ancilla = n - 1
            if (n - 1 % 2 != 0):
                circuit.cx(qr[n-1], qr[n-2])
                ancilla = n - 2
                while ancilla > 0:
                    circuit.ccx(qr[ancilla], qr[ancilla - 1], qr[ancilla - 2])
                    ancilla -= 2
            else:
                while ancilla > 0:
                    circuit.ccx(qr[ancilla], qr[ancilla - 1], qr[ancilla - 2])
                    ancilla -= 2
        # Toffoli gates going down (descending)
        if n > 2:
            ancilla = 0
            if (n - 1 % 2 != 0):
                while ancilla <= n - 3:
                    circuit.ccx(qr[ancilla + 2], qr[ancilla + 1], qr[ancilla])
                    ancilla += 2
                circuit.cx(qr[n - 1], qr[n - 2])
            else:
                while ancilla <= n - 3:
                    circuit.ccx(qr[ancilla + 2], qr[ancilla + 1], qr[ancilla])
                    ancilla += 2
        # X gates
        for (i, bit) in enumerate(searched_number_bin):
            if bit == '0':
                circuit.x(qr[n - i - 1])
        # --------------Us operator---------------------
        # Hadamard gates
        for i in range(1, n + 1): # The 0-th qubit is borrowed qubit
            circuit.h(qr[i])
        # X gates
        for i in range(1, n + 1): # The 0-th qubit is zeroed qubit
            circuit.x(qr[i])
        # C^{n-1} Z gate
        # Toffoli gates going up (ascending)
        if n == 1:
            circuit.z(qr[n])
        elif n == 2:
            circuit.cz(qr[n-1], qr[n])
        else:
            ancilla = n - 1
            if (n - 1 % 2 != 0):
                circuit.cx(qr[n-1], qr[n-2])
                ancilla = n - 2
                while ancilla > 0:
                    circuit.ccx(qr[ancilla], qr[ancilla - 1], qr[ancilla - 2])
                    ancilla -= 2
            else:
                while ancilla > 0:
                    circuit.ccx(qr[ancilla], qr[ancilla - 1], qr[ancilla - 2])
                    ancilla -= 2
        # Toffoli gates going down (descending)
        if n > 2:
            ancilla = 0
            if (n - 1 % 2 != 0):
                while ancilla <= n - 3:
                    circuit.ccx(qr[ancilla + 2], qr[ancilla + 1], qr[ancilla])
                    ancilla += 2
                circuit.cx(qr[n - 1], qr[n - 2])
            else:
                while ancilla <= n - 3:
                    circuit.ccx(qr[ancilla + 2], qr[ancilla + 1], qr[ancilla])
                    ancilla += 2
        # X gates
        for i in range(1, n + 1): # The 0-th qubit is zeroed qubit
            circuit.x(qr[i])
        # Hadamard gates
        for i in range(1, n + 1): # The 0-th qubit is borrowed qubit
            circuit.h(qr[i])



grover(0, 0, 0, 0, 256)
