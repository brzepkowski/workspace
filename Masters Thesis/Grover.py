# serached_elem is decimal representation of element, that we are searching for
# def grover(circuit, qr, n, searched_elem):
#     bin_searched_elem = list(bin(searched_elem))[2:n+2]
#     iterations = (2**n)**(1/2) # √N = √2ⁿ
#     for i in range(n):
#         circuit.h(qr[i])
#     for it in range(iterations):
#         for (i, elem) in enumerate(reversed(bin_searched_elem)):
#             print(i, " -> ", elem)
#             if elem == '1':
#                 print("Git")
#             else:
#                 print("Nie git


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
