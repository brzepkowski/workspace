import math

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
            circuit.cu1(λ, qr[i], qr[j]).inverse() # cu1 gate is not hermitian
